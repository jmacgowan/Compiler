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
    comment_end
};


struct Token {
    TokenType type;
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

    while (peek().has_value()) {
        switch (peek().value()) {
            case ';':
                consume();
                tokens.push_back({TokenType::semi});
                break;
            case '(':
                consume();
                tokens.push_back({TokenType::openParen});
                break;
            case ')':
                consume();
                tokens.push_back({TokenType::closeParen});
                break;
            case '=':
                consume();
                tokens.push_back({TokenType::eq});
                break;
            case '*':
                consume();
                if (peek() == '/') {
                    consume();
                    tokens.push_back({TokenType::comment_end});
                    break;
                } else {
                    consume();
                    tokens.push_back({TokenType::multi});
                    break;
                }
            case '/':
            consume();
                if (peek() == '*') {
                    consume();
                    tokens.push_back({TokenType::comment_start});
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
                    tokens.push_back({TokenType::divide});
                    break;
                }
            case '+':
                consume();
                tokens.push_back({TokenType::plus});
                break;
            case '-':
                consume();
                tokens.push_back({TokenType::minus});
                break;
            case '<':
                consume();
                tokens.push_back({TokenType::lt});
                break;
            case '>':
                consume();
                tokens.push_back({TokenType::gt});
                break;
            case '{':
                consume();
                tokens.push_back({TokenType::openCurly});
                break;
            case '}':
                consume();
                tokens.push_back({TokenType::closeCurly});
                break;
            case '#':
                consume();
                tokens.push_back({TokenType::comment_start});
                break;
            case '~':
                consume();
                tokens.push_back({TokenType::comment_end});
                break;
            default:
                if (std::isdigit(peek().value())) {
                    buf.push_back(consume());
                    while (peek().has_value() && std::isdigit(peek().value())) {
                        buf.push_back(consume());
                    }
                    tokens.push_back({.type = TokenType::_int_lit, .value = buf});
                    buf.clear();
                } else if (std::isalpha(peek().value())) {
                    buf.push_back(consume());
                    while (peek().has_value() && (std::isalnum(peek().value()) || peek().value() == '_')) {
                        buf.push_back(consume());
                    }

                    if (buf == "return") {
                        tokens.push_back({.type = TokenType::_return});
                    } else if (buf == "let") {
                        tokens.push_back({.type = TokenType::let});
                    } else if (buf == "for") {
                        tokens.push_back({.type = TokenType::_for});
                    } else if (buf == "if") {
                        tokens.push_back({.type = TokenType::_if});
                    } else if (buf == "else") {
                        tokens.push_back({.type = TokenType::_else});
                    }else if (buf == "elif") {
                        tokens.push_back({.type = TokenType::_elif});
                    } else if (buf == "==") {
                        tokens.push_back({.type = TokenType::_equals});
                    } else if (buf == "true") {
                        tokens.push_back({.type = TokenType::_bool, .value = "true"});
                    } else if (buf == "false") {
                        tokens.push_back({.type = TokenType::_bool, .value = "false"});
                    } else {
                        tokens.push_back({.type = TokenType::ident, .value = buf});
                    }
                    buf.clear();
                } else if (std::isspace(peek().value())) {
                    consume();
                } else {
                    // Invalid character
                    std::cerr << "Invalid character: " << peek().value() << std::endl;
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
