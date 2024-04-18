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
    var,
    eq
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    explicit Tokenizer(const std::string& src) : m_src(src) {}

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
                    
                default:
                    if (std::isalpha(peek().value())) {
                        buf.push_back(consume());
                        while (peek().has_value() && std::isalnum(peek().value())) {
                            buf.push_back(consume());
                        }
                        if (buf == "return") {
                            tokens.push_back({.type = TokenType::_return});
                            buf.clear();
                            break;
                        } else if(buf == "let"){
                            tokens.push_back({.type = TokenType::ident, .value = buf});
                            buf.clear();
                        }
                        
                    } else if (std::isdigit(peek().value())) {
                        buf.push_back(consume());
                        while (peek().has_value() && std::isdigit(peek().value())) {
                            buf.push_back(consume());
                        }
                        tokens.push_back({.type = TokenType::_int_lit, .value = buf});
                        buf.clear();
                        break;
                    } else if (std::isspace(peek().value())) {
                        consume();
                        break;
                    } else {
                        tokens.push_back({.type = TokenType::ident, .value = buf});
                        buf.clear();
                        continue;
                    }
            }
        }
        m_index = 0;
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
    int m_index = 0;
};
