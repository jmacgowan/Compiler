#pragma once
#include <variant>
#include <string>
#include <sstream> 
#include <vector>
#include <map>
#include <cassert>
#include <algorithm>
#include <ranges>

class Generator {
public:
    explicit Generator(NodeProg prog)
        : m_prog(std::move(prog)) {}

    void gen_term(const NodeTerm* term){
        struct TermVisitor
        {
            Generator* gen;
            TermVisitor(Generator* gen) : gen(gen) {}

        void operator()(const NodeTermIdent* term_ident) {

            auto it  = std::find_if(gen->m_vars.cbegin(), gen->m_vars.cend(), [&] (const Var& var){
                return var.name == term_ident->ident.value.value();
            });
            if (it == gen->m_vars.cend()){
                    std::cerr << "Unknown Identifier" << term_ident->ident.value.value();
                    exit(EXIT_FAILURE);
                    } 
                std::stringstream  offset;
                offset << "QWORD [rsp + " << 8 * (gen->m_stack_size - (*it).stack_loc -1) << "]\n";
                gen->push(offset.str()); 
        }
        void operator()(const NodeTermIntLit* term_int_lit) {
                gen->m_output << "    mov rax, " << term_int_lit->_int_lit.value.value() << "\n";
                gen-> push("rax");
        }

        void operator()(const NodeTermParen* bin_expr) {
            gen->gen_expr(bin_expr->expr);
        }
        
        };
        
        TermVisitor visitor(this);
        std::visit(visitor, term->var);
    }

    void gen_bin_expr(const BinExpr* expr){
        struct BinVisitor
        {
            Generator* gen;
            BinVisitor(Generator* gen) : gen(gen) {}

            void operator()(const BinExprAdd* bin_add){
                gen->gen_expr(bin_add->lhs);
                gen->gen_expr(bin_add->rhs);
                gen->pop("rax");
                gen->pop("rbx");
                gen->m_output << "    add rax, rbx\n";
                gen->push("rax");
            }  
            void operator()(const BinExprSub* bin_minus){
                gen->gen_expr(bin_minus->lhs);
                gen->gen_expr(bin_minus->rhs);
                gen->pop("rax");
                gen->pop("rbx");
                gen->m_output << "    sub rax, rbx\n";
                gen->push("rax");
            }  
            
            void operator()(const BinExprMulti* multi){
                gen->gen_expr(multi->lhs);
                gen->gen_expr(multi->rhs);
                gen->pop("rax");
                gen->pop("rbx");
                gen->m_output << "    imul rax, rbx\n";
                gen->push("rax");
            } 
            void operator()(const BinExprDiv* div){
                gen->gen_expr(div->lhs);
                gen->gen_expr(div->rhs);
                gen->pop("rbx"); 
                gen->pop("rax"); 
                gen->m_output << "    cqo\n";
                gen->m_output << "    idiv rbx\n";
                gen->push("rax"); 
            }

        };
        BinVisitor visitor(this);
        std::visit(visitor, expr->var);
    }
    void gen_expr(const NodeExpr* expr) {
        struct ExprVisitor {
            Generator* gen;
            ExprVisitor(Generator* gen) : gen(gen) {}

            void operator()(const NodeTerm* term) {
                gen->gen_term(term);
                }
            
            void operator()(const BinExpr* bin_expr) {
                gen->gen_bin_expr(bin_expr);
            }
        
        };

        ExprVisitor visitor(this);
        std::visit(visitor, expr->var);
    }

void gen_cond(const NodeCond* cond, const std::string& condition_true_label, const std::string& condition_end_label) {
    gen_expr(cond->var->expr1);
    gen_expr(cond->var->expr2);
    m_output << "    pop rax\n"; 
    m_output << "    pop rbx\n"; 

    if (cond->var->bool1.type == TokenType::lt) {
        m_output << "    cmp rbx, rax\n";
        m_output << "    jl " << condition_true_label << "\n"; 
    } else if (cond->var->bool1.type == TokenType::gt) {
        m_output << "    cmp rbx, rax\n";
        m_output << "    jg " << condition_true_label << "\n";
    } else if (cond->var->bool1.type == TokenType::_equals) {
        m_output << "    cmp rbx, rax\n";
        m_output << "    je " << condition_true_label << "\n";
    }

    m_output << "    jmp " << condition_end_label << "\n";
    m_output << condition_true_label << ":\n";
}

void gen_if(const NodeIf* if_stmt) {
    m_if_label_count++;

    std::string condition_true_label = "condition_true_" + std::to_string(m_if_label_count);
    std::string condition_end_label = "condition_end_" + std::to_string(m_if_label_count);
    std::string if_end_label = "if_end_" + std::to_string(m_if_label_count);

    gen_cond(if_stmt->cond, condition_true_label, condition_end_label);

    gen_stmnt(if_stmt->trueStmnts);

    m_output << condition_end_label << ":\n";
}  
void gen_stmnt(const NodeStmnt* stmnt) {
        struct StmntVisitor {
            Generator* gen;

            void operator()(const NodeStmntScope* stmnts) {
            
                gen->begin_scope();
            for (const auto& stmnt : stmnts->stmnts) {
                gen->gen_stmnt(stmnt);
            }
                gen->end_scope();
            }

            void operator()(const NodeStmntExit* stmnt_exit) {
                gen->gen_expr(stmnt_exit->expr);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n"; 
            }

            void operator()(const NodeStmntLet* stmnt_let) {
                
            auto it  = std::find_if(gen->m_vars.cbegin(), gen->m_vars.cend(), [&] (const Var& var){
                return var.name == stmnt_let->ident.value.value();
            });
            if (!(it == gen->m_vars.cend())){
                std::cerr << "Identifier already used" << stmnt_let->ident.value.value();
                exit(EXIT_FAILURE);
                }  
               
            gen -> m_vars.push_back({.name = stmnt_let->ident.value.value(), .stack_loc = gen->m_stack_size});
            gen -> gen_expr(stmnt_let->expr);
    
            }
                    
            void operator()(const NodeStmntUpdate* stmnt_update) {
            auto it = std::find_if(gen->m_vars.begin(), gen->m_vars.end(), [&](const Var& var) {
                return var.name == stmnt_update->ident.value.value();
            });
            if (it == gen->m_vars.end()) {
                std::cerr << "Unknown Identifier: " << stmnt_update->ident.value.value() << std::endl;
                exit(EXIT_FAILURE);
            }
            gen->gen_expr(stmnt_update->expr);
            gen->pop("rax");
            gen->m_output << "    mov[rsp + " << (gen->m_stack_size - it->stack_loc - 1) *8 << "], rax\n";
            
             }
            
            void operator()(const NodeIf* stmnt_if) {
                gen->gen_if(stmnt_if);
            }
            
void operator()(const NodeStmntFor* stmnt_for) {
   assert(false);
}

    void operator()(const NodeComment* stmnt_comment) {
        gen->m_output << "; " << stmnt_comment->string << "\n";
    }


    };

        StmntVisitor visitor{this};
        std::visit(visitor, stmnt->var);
    }

    std::string gen_prog() {
        m_output << "global _start\n_start:\n";
        for (const NodeStmnt* stmnt : m_prog.stmnts) {
            gen_stmnt(stmnt);
        }
        m_output << "    mov rax, 60\n";
        m_output << "    pop rdi\n";
        m_output << "    syscall\n";
        return m_output.str();
    }

private:

    void push(const std::string& reg){
        m_output << "    push " << reg << "\n";
        m_stack_size++;
    }

    void pop(const std::string& reg){
        m_output << "    pop " << reg << "\n";
        m_stack_size--;
    }

    void begin_scope(){
        m_scopes.push_back(m_vars.size());
    }

    void end_scope(){
        size_t pop_count = m_vars.size() - m_scopes.back();
        m_output << "    add rsp," << pop_count * 8 << "\n";
        m_stack_size -= pop_count;
        for (int i = 0; i < pop_count; i++)
        {
            m_vars.pop_back();
        }
        m_scopes.pop_back();
        
    }

    struct Var
    {
        std::string name;
        size_t stack_loc;
    };
    

    const NodeProg m_prog;
    std::stringstream m_output;
    size_t m_stack_size= 0;
    std::vector<Var> m_vars {};
    std::vector<size_t> m_scopes {};
    int m_if_label_count = 0;
};
