#pragma once
#include <variant>
#include <string>
#include <sstream> // Needed for std::stringstream
#include <vector>
#include <unordered_map>
#include <cassert>

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
         if (!gen->m_vars.contains(term_ident->ident.value.value()))
                {
                    std::cerr << "Unknown Identifier" << term_ident->ident.value.value();
                exit(EXIT_FAILURE);
                } else
                {
                    const auto& var = gen->m_vars.at(term_ident->ident.value.value());
                    std::stringstream  offset;
                    offset << "QWORD [rsp + " << 8 * (gen->m_stack_size - var.stack_loc -1) << "]\n";
                    gen->push(offset.str());
                }
                
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

void gen_cond(const NodeCond* cond){
    gen_expr(cond->var->expr1);
    gen_expr(cond->var->expr2);
    m_output << "    pop rax\n"; // Right-hand side expression result
    m_output << "    pop rbx\n"; // Left-hand side expression result

    // Compare the values in rax and rbx based on the condition operator
    if (cond->var->bool1.type == TokenType::lt) {
        m_output << "    cmp rbx, rax\n";
        m_output << "    jl condition_true\n"; // Jump if less
    } else if (cond->var->bool1.type == TokenType::gt) {
        m_output << "    cmp rbx, rax\n";
        m_output << "    jg condition_true\n"; // Jump if greater
    } else if (cond->var->bool1.type == TokenType::_equals) {
        m_output << "    cmp rbx, rax\n";
        m_output << "    je condition_true\n"; // Jump if equal
    }

    // If the condition is false, jump to the end of the if statement
    m_output << "    jmp condition_end\n";
    m_output << "condition_true:\n";

}

void gen_if(const NodeIf* if_stmt) {
    gen_cond(if_stmt->cond);

    for (const auto& stmnt : if_stmt->trueStmnts->stmnts) {
        gen_stmnt(stmnt);
    }

    if (!if_stmt->falseStmnts->stmnts.empty()) {
        m_output << "    jmp if_end\n"; 
        m_output << "condition_end:\n";
        for (const auto& stmnt : if_stmt->falseStmnts->stmnts) {
            gen_stmnt(stmnt);
        }
    }

    m_output << "if_end:\n";
}
    void gen_stmnt(const NodeStmnt* stmnt) {
        struct StmntVisitor {
            Generator* gen;

            void operator()(const NodeStmntExit* stmnt_exit) {
                gen->gen_expr(stmnt_exit->expr);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
            }

            void operator()(const NodeStmntLet* stmnt_let) {
                
                if (gen->m_vars.contains(stmnt_let->ident.value.value())) {
                std::cerr << "Identifier already used: " << stmnt_let->ident.value.value();
                exit(EXIT_FAILURE);
                } else {
                    gen -> m_vars.insert({stmnt_let->ident.value.value(), Var {.stack_loc = gen->m_stack_size}});
                    gen -> gen_expr(stmnt_let->expr);
                }
            }
            void operator()(const NodeIf* stmnt_if) {

                gen->gen_if(stmnt_if);
                
 
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

    struct Var
    {
        size_t stack_loc;
    };
    

    const NodeProg m_prog;
    std::stringstream m_output;
    size_t m_stack_size= 0;
    std::unordered_map<std::string, Var> m_vars {};
};
