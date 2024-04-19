#pragma once
#include <variant>
#include <string>
#include <sstream> // Needed for std::stringstream
#include <vector>
#include <unordered_map>

class Generator {
public:
    explicit Generator(NodeProg prog)
        : m_prog(std::move(prog)) {}

    void gen_expr(const NodeExpr& expr) {
        struct ExprVisitor {
            Generator* gen;
            ExprVisitor(Generator* gen) : gen(gen) {}

            void operator()(const NodeExprIdent& expr_ident) {
                if (!gen->m_vars.contains(expr_ident.ident.value.value()))
                {
                    std::cerr << "Unknown Identifier" << expr_ident.ident.value.value();
                exit(EXIT_FAILURE);
                } else
                {
                    const auto& var = gen->m_vars.at(expr_ident.ident.value.value());
                    std::stringstream  offset;
                    offset << "QWORD [rsp + " << 8 * (gen->m_stack_size - var.stack_loc) << "]\n";
                    gen->push(offset.str());
                }
                
                }

            void operator()(const NodeExprIntLit& expr_int_lit) {
                gen->m_output << "    mov rax, " << expr_int_lit._int_lit.value.value() << "\n";
                gen->m_output << "    push rax\n";

            }
        };

        ExprVisitor visitor(this);
        std::visit(visitor, expr.var);
    }

    void gen_stmnt(const NodeStmnt& stmnt) {
        struct StmntVisitor {
            Generator* gen;

            void operator()(const NodeStmntExit& stmnt_exit) {
                gen->gen_expr(stmnt_exit.expr);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
            }

            void operator()(const NodeStmntLet& stmnt_let) {
                
                if (gen->m_vars.contains(stmnt_let.ident.value.value())) {
                std::cerr << "Identifier already used: " << stmnt_let.ident.value.value();
                exit(EXIT_FAILURE);
                } else {
                    gen -> m_vars.insert({stmnt_let.ident.value.value(), Var {.stack_loc = gen->m_stack_size}});
                    gen -> gen_expr(stmnt_let.expr);
                }
            }
        };

        StmntVisitor visitor{this};
        std::visit(visitor, stmnt.var);
    }

    std::string gen_prog() {
        m_output << "global _start\n_start:\n";
        for (const NodeStmnt& stmnt : m_prog.stmnts) {
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
