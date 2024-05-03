#include <cstdlib> // Needed for EXIT_FAILURE
#include <iostream> // Needed for std::cerr
#include <optional>
#include <string>
#include <vector>


enum class TokenType {
    _return,
    _int_lit,
    semi,
    openParen,
    closeParen,
    ident,
    let,
    eq,
    plus,
    minus,
    multi,
    divide,
    _if,
    _elif,
    _else,
    _bool,
    gt,
    lt,
    _equals,
    openCurly,
    closeCurly,
    _for,
    comment_start,
    comment_end,
    _newline
};


struct Token {
    TokenType type;
    int line;
    std::optional<std::string> value;

};

std::optional<int> bin_prec(TokenType type){
    switch (type)
    {
    case TokenType::minus:
    case TokenType::plus:
        return 1;
    case TokenType::multi:
    case TokenType::divide:
        return 2;
        break;
    default:
    return {};
        break;
    }
}
class Tokenizer {
public:
    Tokenizer(const std::string& src) : m_src(src) {}

std::vector<Token> tokenize() {
    std::vector<Token> tokens;
    std::string buf;
    int line_count = 0;
    while (peek().has_value()) {
        switch (peek().value()) {
            case ';':
                consume();
                tokens.push_back({TokenType::semi, line_count});
                break;
            case '(':
                consume();
                tokens.push_back({TokenType::openParen,line_count});
                break;
            case ')':
                consume();
                tokens.push_back({TokenType::closeParen, line_count});
                break;
            case '=':
                consume();
                tokens.push_back({TokenType::eq, line_count});
                break; 
            case '*':
                consume();
                if (peek() == '/') {
                    consume();
                    tokens.push_back({TokenType::comment_end, line_count});
                    break;
                } else {
                    consume();
                    tokens.push_back({TokenType::multi, line_count});
                    break;
                }
            case '/':
            consume();
                if (peek() == '*') {
                    consume();
                    tokens.push_back({TokenType::comment_start, line_count});
                    while (peek() != '*' || peek(1) != '/') {
                        if (!peek().has_value()) {
                            std::cerr << "Unterminated comment" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        consume();
                    }
                    consume();
                    consume();
                    break;
                } else {
                    consume();
                    tokens.push_back({TokenType::divide, line_count});
                    break;
                }
            case '+':
                consume();
                tokens.push_back({TokenType::plus,line_count});
                break;
            case '-':
                consume();
                tokens.push_back({TokenType::minus,line_count});
                break;
            case '<':
                consume();
                tokens.push_back({TokenType::lt, line_count});
                break;
            case '>':
                consume();
                tokens.push_back({TokenType::gt, line_count});
                break;
            case '{':
                consume();
                tokens.push_back({TokenType::openCurly,  line_count});
                break;
            case '}':
                consume();
                tokens.push_back({TokenType::closeCurly, line_count});
                break;
            case '#':
                consume();
                tokens.push_back({TokenType::comment_start,  line_count});
                break;
            case '~':
                consume();
                tokens.push_back({TokenType::comment_end,line_count});
                break;
            default:
                if (std::isdigit(peek().value())) {
                    buf.push_back(consume());
                    while (peek().has_value() && std::isdigit(peek().value())) {
                        buf.push_back(consume());
                    }
                    tokens.push_back({.type = TokenType::_int_lit, .line = line_count, .value = buf});
                    buf.clear();
                } else if (std::isalpha(peek().value())) {
                    buf.push_back(consume());
                    while (peek().has_value() && (std::isalnum(peek().value()) || peek().value() == '_')) {
                        buf.push_back(consume());
                    }

                    if (buf == "return") {
                        tokens.push_back({.type = TokenType::_return, .line = line_count});
                    } else if (buf == "let") {
                        tokens.push_back({.type = TokenType::let, .line = line_count});
                    } else if (buf == "for") {
                        tokens.push_back({.type = TokenType::_for, .line = line_count});
                    } else if (buf == "if") {
                        tokens.push_back({.type = TokenType::_if, .line = line_count});
                    } else if (buf == "else") {
                        tokens.push_back({.type = TokenType::_else, .line = line_count});
                    }else if (buf == "elif") {
                        tokens.push_back({.type = TokenType::_elif, .line = line_count});
                    } else if (buf == "==") {
                        tokens.push_back({.type = TokenType::_equals, .line = line_count});
                    } else if (buf == "true") {
                        tokens.push_back({.type = TokenType::_bool, .line = line_count, .value = "true"});
                    } else if (buf == "false") {
                        tokens.push_back({.type = TokenType::_bool, .line = line_count, .value = "false"});
                    } else {
                        tokens.push_back({.type = TokenType::ident, .line = line_count, .value = buf});
                    }
                    buf.clear();
                } 
                else if (peek().value() == '\n') {
                    line_count++;
                    consume();
                } else if (std::isspace(peek().value())) {
                    consume();}
                    else {
                    // Invalid character
                    std::cerr << "[Tokenisation Error] " << peek().value() << std::endl;
                    exit(EXIT_FAILURE);
                }
        }
    }
    return tokens;
}

private:
    std::optional<char> peek(int offset = 0) const {
        if (m_index + offset >= m_src.length()) {
            return {};
        }
        return m_src.at(m_index + offset);
    }

    char consume() {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    size_t m_index = 0;
};
