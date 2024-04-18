#pragma once
#include <optional>
#include "tokenization.hpp"

    struct NodeExit
    {
        NodeExpr expr;
    };
    
    struct NodeExpr
    {
        Token _int_lit;
    };
    
class Parser
{
public:

    inline explicit Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens)) {}

    std::optional<NodeExpr> parser_expr(){
        if (peak().has_value() && peak().value().type == TokenType::_int_lit)
        {
            return NodeExpr {._int_lit = consume()};
        } else {
            return {};
        }
        
    }

    std::optional<NodeExit> parse(){
        std::optional<NodeExit> exit_node;
        while (peak().has_value())
        {
            if(peak().value().type == TokenType::_return){
                consume();
                if (auto node_expr = parser_expr()){
                    exit_node = NodeExit {.expr = node_expr.value()};
                 } 
                else {
                    std::cerr << "Invalid Expr" << std::endl;
                    exit(EXIT_FAILURE);
                }   
            }
            if(peak().value().type != TokenType::semi || !peak().has_value()){
                  std::cerr << "Invalid Expr" << std::endl;
                    exit(EXIT_FAILURE);
            }
        }
        
    }
private:

    [[nodiscard]] std::optional<Token> peak(int head = 1) const {
        if (m_index + head > m_tokens.size()) {
            return {};
        }
        return m_tokens.at(m_index);
    }

    Token consume() {
        return m_tokens.at(m_index++);
    }

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
    
};