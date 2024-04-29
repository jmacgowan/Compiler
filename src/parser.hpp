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

    struct NodeStmnt;

    struct NodeExpr;


    struct NodeTermParen
    {
        NodeExpr* expr;
    };
    
    struct BinExprAdd
    {
        NodeExpr* lhs;
        NodeExpr* rhs;
    };

    struct BinExprSub
    {
        NodeExpr* lhs;
        NodeExpr* rhs;
    };
    struct BinExprDiv
    {
        NodeExpr* lhs;
        NodeExpr* rhs;
    };

    struct BinExprMulti
    {
        NodeExpr* lhs;
        NodeExpr* rhs;
    };

    struct BinExpr
    {
        std::variant<BinExprAdd*, BinExprSub*, BinExprDiv* ,BinExprMulti*> var;
    };

    struct NodeTerm
    {
        std::variant<NodeTermIntLit*, NodeTermIdent*, NodeTermParen*> var;
    };
    struct NodeExpr
    {
        std::variant<NodeTerm*, BinExpr*> var;
    };

    struct NodeBool{
        NodeExpr* expr1;
        Token bool1;
        NodeExpr* expr2;
    };

    struct NodeCond{
        NodeBool* var;
    };
    
    struct NodeStmntFor
    {
        NodeStmnt* expr;
        NodeCond* cond;
        NodeStmnt* update;
        NodeStmntScope* stmnts;

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

    struct NodeStmntUpdate
    {
        Token ident;
        NodeExpr* expr;
    };

    struct NodeStmntScope
    {
        std::vector<NodeStmnt*> stmnts;
    };

    struct NodeIf
    {
        NodeCond* cond;
        NodeStmntScope* trueStmnts;
        NodeStmntScope* falseStmnts;
    };

    struct NodeStmnt
    {
        std::variant<NodeStmntExit*, NodeStmntLet*, NodeIf*, NodeStmntScope*, NodeStmntUpdate*, NodeStmntFor*> var;
    };

    struct NodeProg
    {
        std::vector<NodeStmnt*> stmnts; 
    };
    
class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens)),
        m_allocator(1024 * 1024 * 8) {}

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
        } else if (auto ident = tryConsume(TokenType::openParen)) {
            auto expr = parser_expr();
            if (!expr.has_value())
            {
            std::cerr <<("Can't parse bracketed expression") << std::endl;
            exit(EXIT_FAILURE);
            }
            tryConsume(TokenType::closeParen, "Expected ')'"); 
            auto node_term = m_allocator.alloc<NodeTermParen>();
            node_term->expr = expr.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->var = node_term;

            return term;
        } else {
            return {};
        }
    }
std::optional<NodeCond*> parse_cond(){

    auto node_cond = m_allocator.alloc<NodeBool>();
    
    auto expr_lhs = parser_expr();

    if (!expr_lhs.has_value())
    {
        std::cerr << "Expected expr1" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    Token cond = consume();
    if (cond.type != TokenType::lt && cond.type != TokenType::gt && cond.type != TokenType::_equals) {
        std::cerr << "Expected comparison operator (<, >, or ==)" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto expr_rhs = parser_expr();
    if (!expr_rhs.has_value()) {
        std::cerr << "Can't parse right-hand side expression" << std::endl;
        exit(EXIT_FAILURE);
    }

    node_cond->expr1 = expr_lhs.value();
    node_cond->expr2 = expr_rhs.value();
    node_cond->bool1 = cond;
    
    auto node_cond1 = m_allocator.alloc<NodeCond>();
    node_cond1->var = node_cond;
    return node_cond1;
}


std::optional<NodeExpr*> parser_expr(int max_prec = 0) {

        std::optional<NodeTerm*> term = parse_term();
        if (!term.has_value()) {

            return {};
        }
        auto expr_lhs = m_allocator.alloc<NodeExpr>();
        expr_lhs->var = term.value();

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
           
        Token op = consume();
        int prec_new_val = prec.value() + 1;
        auto expr_rhs = parser_expr(prec_new_val);

        if (!expr_rhs.has_value())
        {
            std::cerr <<("Can't parse") << std::endl;
            exit(EXIT_FAILURE);
        }
        auto expr = m_allocator.alloc<BinExpr>();
        auto expr_lhs2 = m_allocator.alloc<NodeExpr>();
        if (op.type == TokenType::plus) {
            auto bin_expr_add = m_allocator.alloc<BinExprAdd>();
            expr_lhs2->var = expr_lhs->var; 
            bin_expr_add->lhs = expr_lhs2;
            bin_expr_add->rhs = expr_rhs.value();
            expr->var = bin_expr_add;
        } 
        else if (op.type == TokenType::minus) {
            auto bin_expr_minus = m_allocator.alloc<BinExprSub>();
            expr_lhs2->var = expr_lhs->var; 
            bin_expr_minus->lhs = expr_lhs2;
            bin_expr_minus->rhs = expr_rhs.value();
            expr->var = bin_expr_minus;
            }
        else if (op.type == TokenType::multi) {
            auto bin_expr_multi = m_allocator.alloc<BinExprMulti>();
            expr_lhs2->var = expr_lhs->var; 
            bin_expr_multi->lhs = expr_lhs2;
            bin_expr_multi->rhs = expr_rhs.value();
            expr->var = bin_expr_multi;
            } 
        else if (op.type == TokenType::divide) {
            auto bin_expr_divide = m_allocator.alloc<BinExprDiv>();
            expr_lhs2->var = expr_lhs->var; 
            bin_expr_divide->lhs = expr_lhs2;
            bin_expr_divide->rhs = expr_rhs.value();
            expr->var = bin_expr_divide;
            }

        expr_lhs->var = expr;
}
         
    return expr_lhs;
}


std::optional<NodeStmnt*> parse_let_statement() {
    auto node_stmnt = m_allocator.alloc<NodeStmnt>();
    auto let_token = tryConsume(TokenType::let);
    if (!let_token.has_value()) return {};

    auto ident = tryConsume(TokenType::ident, "Expected Identifier after let");
    tryConsume(TokenType::eq, "Expected '=' after ident");

    if (auto node_expr = parser_expr()) {
        tryConsume(TokenType::semi, "Expected ';' after declaration");

        auto node_stmnt_let = m_allocator.alloc<NodeStmntLet>();
        node_stmnt_let->ident = ident.value();
        node_stmnt_let->expr = node_expr.value();

        node_stmnt->var = node_stmnt_let;
        return node_stmnt;
    } else {
        std::cerr << "Invalid expression after '='." << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::optional<NodeStmnt*> parse_return_statement() {
    auto node_stmnt = m_allocator.alloc<NodeStmnt>();
    auto return_token = tryConsume(TokenType::_return);
    if (!return_token.has_value()) return {};

    auto node_stmnt_exit = m_allocator.alloc<NodeStmntExit>();

    if (tryConsume(TokenType::openParen).has_value()) {
        if (auto node_expr = parser_expr()) {
            node_stmnt_exit->expr = node_expr.value();
        } else {
            std::cerr << "Invalid expression after '(' for return statement." << std::endl;
            exit(EXIT_FAILURE);
        }
        tryConsume(TokenType::closeParen, "Expected ')' after return statement.");
    } else {
        if (auto node_expr = parser_expr()) {
            node_stmnt_exit->expr = node_expr.value();
        } else {
            std::cerr << "Invalid expression for return statement." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    tryConsume(TokenType::semi, "Expected ';' after return statement.");
    node_stmnt->var = node_stmnt_exit;
    return node_stmnt;
}

std::optional<NodeStmnt*> parse_if_statement() {
    auto node_stmnt = m_allocator.alloc<NodeStmnt>();
    auto if_token = tryConsume(TokenType::_if);
    if (!if_token.has_value()) return {};

    auto node_if = m_allocator.alloc<NodeIf>();
    tryConsume(TokenType::openParen, "Expected '('");

    if (auto node_cond = parse_cond()) {
        tryConsume(TokenType::closeParen, "Expected ')'");
        node_if->cond = node_cond.value();

        tryConsume(TokenType::openCurly, "Expected '{'");
        auto stmnts_true = parseStmnts().value();
        tryConsume(TokenType::closeCurly, "Expected '}'");

        auto stmnts_false = m_allocator.alloc<NodeStmntScope>();
        if (tryConsume(TokenType::_else).has_value()) {
            tryConsume(TokenType::openCurly, "Expected '{'");
            stmnts_false = parseStmnts().value();
            tryConsume(TokenType::closeCurly, "Expected '}'");
        }

        node_if->trueStmnts = stmnts_true;
        node_if->falseStmnts = stmnts_false;

        node_stmnt->var = node_if;
        return node_stmnt;
    } else {
        std::cerr << "Invalid expression after '(' for conditional statement." << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::optional<NodeStmntScope*> parseStmnts() {
    auto stmntBlock = m_allocator.alloc<NodeStmntScope>();

    while (auto stmnt = parse_stmnt()) {
        stmntBlock->stmnts.push_back(stmnt.value());
    }

    return stmntBlock;
}

std::optional<NodeStmnt*> parse_block_statement() {
    if (!tryConsume(TokenType::openCurly).has_value()) {
        return {};
    }
    auto stmnts = parseStmnts().value();
    auto node_stmnt = m_allocator.alloc<NodeStmnt>();
    node_stmnt->var = stmnts;
    tryConsume(TokenType::closeCurly, "Expected '}'");
    return node_stmnt;
}

std::optional<NodeStmnt*> parse_assignment_statement() {
    auto ident = tryConsume(TokenType::ident);
    if (!ident.has_value()) {
       return {};
        }
    tryConsume(TokenType::eq, "Expected '=' after ident");
    if (auto node_expr = parser_expr()) {
        tryConsume(TokenType::semi, "Expected ';' after expression");
        auto node_stmnt_update = m_allocator.alloc<NodeStmntUpdate>();
        node_stmnt_update->ident = ident.value();
        node_stmnt_update->expr = node_expr.value();
        auto node_stmnt = m_allocator.alloc<NodeStmnt>();
        node_stmnt->var = node_stmnt_update;
        return node_stmnt;
    } else {
        std::cerr << "Invalid expression after '='." << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::optional<NodeStmnt*> parse_for_statement() {
    auto _for = tryConsume(TokenType::_for);
    if (!_for.has_value()) {
        return {};
    }
    tryConsume(TokenType::openCurly, "Expected '(' after for");
    auto node_expr = parse_stmnt();
    if (!node_expr) {
        std::cerr << "Invalid expression after '('." << std::endl;
        exit(EXIT_FAILURE);
    } 
    tryConsume(TokenType::semi, "Expected ';' after expression");
    auto node_cond = parse_cond();
    if (!node_expr) {
        std::cerr << "Invalid condition" << std::endl;
        exit(EXIT_FAILURE);
    } 
    tryConsume(TokenType::semi, "Expected ';' after condition");
    auto node_update = parse_stmnt();
    if (!node_update) {
        std::cerr << "Invalid update expression in for" << std::endl;
        exit(EXIT_FAILURE);
    } 
    tryConsume(TokenType::semi, "Expected ';' after condition");
    tryConsume(TokenType::closeParen, "Expected ')' after final expression");
    tryConsume(TokenType::openCurly, "Expected statement block");
    auto node_block = parseStmnts();
    tryConsume(TokenType::closeCurly, "Expected '}' to close block");
    auto node_stmnt_for = m_allocator.alloc<NodeStmntFor>();
    node_stmnt_for->cond = node_cond.value();
    node_stmnt_for->expr = node_expr.value();
    node_stmnt_for->update = node_update.value();
    node_stmnt_for->stmnts = node_block.value();
    auto node_stmnt = m_allocator.alloc<NodeStmnt>();
    node_stmnt->var = node_stmnt_for;
    return node_stmnt;
  
}

std::optional<NodeStmnt*> parse_stmnt() {
    if (auto let_stmnt = parse_let_statement()) {
        return let_stmnt;
    } else if (auto return_stmnt = parse_return_statement()) {
        return return_stmnt;
    } else if (auto if_stmnt = parse_if_statement()) {
        return if_stmnt;
    } else if (auto block_stmnt = parse_block_statement()) {
        return block_stmnt;
    } else if (auto assignment_stmnt = parse_assignment_statement()) {
        return assignment_stmnt;
    } else if(auto for_stmnt = parse_for_statement()){
        return for_stmnt;
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
