#pragma once
#include <optional>
#include <variant>
#include "arena.hpp"

    struct NodeTermIntLit
    {
        Token _int_lit;
    }; 
    
    struct NodeTermIdent
    {
        Token ident;
    };

    struct NodeExpr;
    
    struct BinExprAdd{
        NodeExpr* lhs;
        NodeExpr* rhs;
    };

    struct BinExprMulti{
        NodeExpr* lhs;
        NodeExpr* rhs;
    };

    struct BinExpr{
        std::variant<BinExprAdd*, BinExprMulti*> var;
    };

    struct NodeTerm{
        std::variant<NodeTermIntLit*, NodeTermIdent*> var;
    };
    struct NodeExpr
    {
        std::variant<NodeTerm*, BinExpr*> var;
    };
    
    struct NodeStmntExit
    {
        NodeExpr* expr;
    };
    struct NodeStmntLet
    {
        Token ident;
        NodeExpr* expr;
    };

    struct NodeStmnt
    {
        std::variant<NodeStmntExit*, NodeStmntLet*> var;
    };

    struct NodeProg
    {
        std::vector<NodeStmnt*> stmnts; 
    };
    
class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens)),
        m_allocator(1024 * 1024 * 4) {}

    std::optional<NodeTerm*> parse_term() {
        if (auto int_lit = tryConsume(TokenType::_int_lit)) {
            auto node_term_int_lit = m_allocator.alloc<NodeTermIntLit>();
            node_term_int_lit->_int_lit = int_lit.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = node_term_int_lit;
            return term;
        } else if (auto ident = tryConsume(TokenType::ident)) {
            auto node_term_ident = m_allocator.alloc<NodeTermIdent>();
            node_term_ident->ident = ident.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = node_term_ident;
            return term;
        } else {
            return {};
        }
    }

std::optional<NodeExpr*> parser_expr(int max_prec) {

        std::optional<NodeTerm*> term = parse_term();
        if (!term.has_value()) {
            if (tryConsume(TokenType::openParen)) {
            auto nested_expr = parser_expr(4);
            tryConsume(TokenType::closeParen, "Expected ')' to close nested expression");
            return nested_expr;
        }
            return {};
        }

        while (true)
        {
           
           std::optional<Token> curr_token = peak();
           std::optional<int> prec;
           if (curr_token.has_value())
           {
                prec = bin_prec(curr_token->type);
                if (!prec.has_value() || prec<max_prec)
                {
                    break;
                }   
           } else
           {
            break;
           }
           

        int prec_new_val = prec.value() + 1;
        auto expr_rhs = parser_expr(prec_new_val);

        if (!expr_rhs.has_value())
        {
            perror("No");
            exit(EXIT_FAILURE);
        }
        auto expr = m_allocator.alloc<BinExpr>();
        if (tryConsume(TokenType::plus)) {
            auto bin_expr_add = m_allocator.alloc<BinExprAdd>();
            auto lhs_expr = m_allocator.alloc<NodeExpr>();
            lhs_expr->var = term.value();
            bin_expr_add->lhs = lhs_expr;
            if (auto rhs = parser_expr(1)) {
                bin_expr_add->rhs = rhs.value();
                expr->var = bin_expr_add;
                auto expr2 = m_allocator.alloc<NodeExpr>();
                expr2->var = expr;
                return expr2;
            }
            std::cerr << "Invalid right-hand side expression after '+'" << std::endl;
            exit(EXIT_FAILURE);
        } else if (tryConsume(TokenType::multi))
        {
            auto bin_expr_multi = m_allocator.alloc<BinExprMulti>();
            auto lhs_expr = m_allocator.alloc<NodeExpr>();
            lhs_expr->var = term.value();
            bin_expr_multi->lhs = lhs_expr;
            if (auto rhs = parser_expr(2)) {
                bin_expr_multi->rhs = rhs.value();
                expr->var = bin_expr_multi;
                auto expr2 = m_allocator.alloc<NodeExpr>();
                expr2->var = expr;
                return expr2;
            }
            std::cerr << "Invalid right-hand side expression after '*'" << std::endl;
            exit(EXIT_FAILURE);
        } 

        expr_lhs->var = expr;
}
         
    return expr_lhs;
}


    std::optional<NodeStmnt*> parse_stmnt() {
        if (tryConsume(TokenType::let).has_value()) {
            auto ident = tryConsume(TokenType::ident, "Expected Identifier after let"); 
                tryConsume(TokenType::eq, "Expected '=' after ident");
                    if (auto node_expr = parser_expr(0)) {
                        tryConsume(TokenType::semi, "Expected ';' after declaration");
                            auto node_stmnt_let = m_allocator.alloc<NodeStmntLet>();
                            node_stmnt_let->ident = ident.value();
                            node_stmnt_let->expr = node_expr.value();

                            auto node_stmnt = m_allocator.alloc<NodeStmnt>();
                            node_stmnt->var = node_stmnt_let;
                            return node_stmnt;
                        
                    } else {
                        std::cerr << "Invalid expression after '='" << std::endl;
                        exit(EXIT_FAILURE);
                    }} 
        else if (tryConsume(TokenType::_return).has_value()) {
            auto node_stmnt_exit = m_allocator.alloc<NodeStmntExit>();

            if (tryConsume(TokenType::openParen).has_value()) {
                if (auto node_expr = parser_expr(0)) {
                    node_stmnt_exit->expr = node_expr.value();
                } else {
                    std::cerr << "Invalid expression after '(' for return statement" << std::endl;
                    exit(EXIT_FAILURE);
                }
                tryConsume(TokenType::closeParen, "Expected ')' after return statement");

            } else
            {
                if (auto node_expr = parser_expr(0)) {
                    node_stmnt_exit->expr = node_expr.value();
                } else {
                    std::cerr << "Invalid expression for return statement" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            tryConsume(TokenType::semi, "Expected ';' after return statement");
            auto node_stmnt = m_allocator.alloc<NodeStmnt>();
            node_stmnt->var = node_stmnt_exit;
            return node_stmnt;
        }

        return {};
    }

    std::optional<NodeProg> parse_prog() {
        NodeProg prog;
        while (peak().has_value()) {
            if (auto stmnt = parse_stmnt()) {
                prog.stmnts.push_back(stmnt.value());
            } else {
                std::cerr << "Invalid statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return prog;
    }

private:
    [[nodiscard]] std::optional<Token> tryConsume(TokenType type) {
        if (peak().has_value() && peak().value().type == type) {
            return consume();
        }
        return {};
    }    
    std::optional<Token> tryConsume(TokenType type, const std::string& errorMessage) {
        if (peak().has_value() && peak().value().type == type) {
            return consume();
        }
        std::cerr << errorMessage << std::endl;
        exit(EXIT_FAILURE);
    }

    [[nodiscard]] std::optional<Token> peak(int offset = 0) const {
        if (m_index + offset >= m_tokens.size()) {
            return {};
        }
        return m_tokens.at(m_index + offset);
    }

    Token consume() {
        return m_tokens.at(m_index++);
    }

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
    ArenaAllocator m_allocator;
};
