#pragma once
#include <optional>
#include <variant>

    struct NodeExprIntLit
    {
        Token _int_lit;
    }; 
    
    struct NodeExprIdent
    {
        Token ident;
    };
    

    struct NodeExpr
    {
        std::variant<NodeExprIntLit, NodeExprIdent> var;
    };
    
    struct NodeStmntExit
    {
        NodeExpr expr;
    };
    struct NodeStmntLet
    {
        Token ident;
        NodeExpr expr;
    };

    struct NodeStmnt
    {
        std::variant<NodeStmntExit, NodeStmntLet> var;
    };

    struct NodeProg
    {
        std::vector<NodeStmnt> stmnts; 
    };
    
class Parser
{
public:

    inline explicit Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens)) {}

    std::optional<NodeExpr> parser_expr(){
        if (peak().has_value() && peak().value().type == TokenType::_int_lit)
        {
            return NodeExpr {.var = NodeExprIntLit {._int_lit = consume()}};
        } else if(peak().has_value() && peak().value().type == TokenType::ident)
        {
           return NodeExpr {.var = NodeExprIdent{.ident = consume()}};
        }
        else {return {};}
    }

    std::optional<NodeStmnt> parse_stmnt() {
    NodeStmntExit stmnt_exit;
           if (peak().has_value() && peak().value().type == TokenType::let) {
        consume(); // Consume 'let'

        if (peak().has_value() && peak().value().type == TokenType::ident) {
            Token ident = consume(); // Consume identifier
            if (peak().has_value() && peak().value().type == TokenType::eq) {
                consume(); // Consume '='
                if (auto node_expr = parser_expr()) {
                    if (peak().has_value() && peak().value().type == TokenType::semi) {
                        consume(); // Consume ';'
                        return NodeStmnt{.var = NodeStmntLet{.ident = ident, .expr = node_expr.value()}};
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
    }
    if (peak().value().type == TokenType::_return) {
        consume(); 
        
        if (peak().has_value() && peak().value().type == TokenType::openParen) {
            consume(); 
            
            if (auto node_expr = parser_expr()) {
                stmnt_exit = {.expr = node_expr.value()};
            } else {
                std::cerr << "Invalid Expr2" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (peak().has_value() && peak().value().type == TokenType::closeParen) {
                consume(); 
            } else {
                std::cerr << "Expected ')'" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            if (auto node_expr = parser_expr()) {
                stmnt_exit = {.expr = node_expr.value()};
            } else {
                std::cerr << "Invalid Expr" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // Check for the semicolon
        if (peak().has_value() && peak().value().type == TokenType::semi) {
            consume(); // Consume ';'
        } else {
            std::cerr << "Expected ';'" << std::endl;
            exit(EXIT_FAILURE);
        }

        return NodeStmnt{.var = stmnt_exit};
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
    
};