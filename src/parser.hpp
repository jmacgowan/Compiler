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
        std::variant<BinExprAdd*,BinExprMulti*> var;
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
    
class Parser
{
public:

    inline explicit Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens)),
    m_allocator(1024*1024*4) {}


    std::optional<NodeTerm*> parse_term(){
         if (peak().has_value() && peak().value().type == TokenType::_int_lit)
        {
            auto node_term_int_lit = m_allocator.alloc<NodeTermIntLit>();
            node_term_int_lit->_int_lit = consume();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = node_term_int_lit;
            return term;
        } else if(peak().has_value() && peak().value().type == TokenType::ident)
        {
            auto node_term_ident = m_allocator.alloc<NodeTermIdent>();
            node_term_ident->ident = consume();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = node_term_ident;
            return term;
        } 
        
        else {return {};}
    }
    std::optional<NodeExpr*> parser_expr(){

        if (auto term = parse_term()){
            if (peak().has_value() && peak().value().type == TokenType::plus)
            {
            
            auto bin_expr = m_allocator.alloc<BinExpr>();
            if(peak().has_value()&& peak().value().type == TokenType::plus){
                auto bin_expr_add = m_allocator.alloc<BinExprAdd>();
                auto lhs_expr = m_allocator.alloc<NodeExpr>();
                lhs_expr->var = term.value();
                bin_expr_add->lhs = lhs_expr;
                consume();
                if (auto rhs = parser_expr())
                {
                    bin_expr_add->rhs = rhs.value();
                    bin_expr->var = bin_expr_add;
                    auto expr =m_allocator.alloc<NodeExpr>();
                    expr->var = bin_expr;
                    return expr;
                }
                
            }else
            {
                std::cerr << "Invalid operator" << std::endl;
                exit(EXIT_FAILURE);
            }
            }else
        {
           auto expr = m_allocator.alloc<NodeExpr>();
           expr->var = term.value();
           return expr;
        }
        } 
        
        else {return {};}
    }

std::optional<NodeStmnt*> parse_stmnt() {
    if (peak().has_value() && peak().value().type == TokenType::let) {
        consume(); // Consume 'let'

        if (peak().has_value() && peak().value().type == TokenType::ident) {
            Token ident = consume(); // Consume identifier
            if (peak().has_value() && peak().value().type == TokenType::eq) {
                consume(); // Consume '='
                if (auto node_expr = parser_expr()) {
                    if (peak().has_value() && peak().value().type == TokenType::semi) {
                        consume(); // Consume ';'
                        auto node_stmnt_let = m_allocator.alloc<NodeStmntLet>();
                        node_stmnt_let->ident = ident;
                        node_stmnt_let->expr = node_expr.value();

                        auto node_stmnt = m_allocator.alloc<NodeStmnt>();
                        node_stmnt->var = node_stmnt_let;
                        return node_stmnt;
                    } else {
                        std::cerr << "Expected ';' after expression" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                } else {
                    std::cerr << "Invalid expression after '='" << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Expected '=' after identifier" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Expected identifier after 'let'" << std::endl;
            exit(EXIT_FAILURE);
        }
    } else if (peak().has_value() && peak().value().type == TokenType::_return) {
        consume(); // Consume 'return'
        
        auto node_stmnt_exit = m_allocator.alloc<NodeStmntExit>();

        if (peak().has_value() && peak().value().type == TokenType::openParen) {
            consume(); // Consume '('
            
            if (auto node_expr = parser_expr()) {
                node_stmnt_exit->expr = node_expr.value();
            } else {
                std::cerr << "Invalid expression after '(' for return statement" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (peak().has_value() && peak().value().type == TokenType::closeParen) {
                consume(); // Consume ')'
            } else {
                std::cerr << "Expected ')' after expression in return statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            if (auto node_expr = parser_expr()) {
                node_stmnt_exit->expr = node_expr.value();
            } else {
                std::cerr << "Invalid expression for return statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // Check for the semicolon
        if (peak().has_value() && peak().value().type == TokenType::semi) {
            consume(); // Consume ';'
        } else {
            std::cerr << "Expected ';' after return statement" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto node_stmnt = m_allocator.alloc<NodeStmnt>();
        node_stmnt->var = node_stmnt_exit;
        return node_stmnt;
    } else {
        return {};
    }
}

std::optional<NodeProg> parse_prog(){
        NodeProg prog;
        while (peak().has_value())
        {
            if (auto stmnt = parse_stmnt())
            {
                prog.stmnts.push_back(stmnt.value());
            }
            else
            {
                std::cerr << "Invalid Expr5" << std::endl;
                exit(EXIT_FAILURE);
            }
            
        }
        return prog;
    }
private:

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