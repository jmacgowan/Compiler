#pragma once
#include <optional>
#include <vector>
#include <string>
#include <iostream> // Needed for std::cerr
#include <cctype>   // Needed for std::isalpha, std::isdigit, std::isspace

enum class TokenType {
    _return,
    _int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    inline explicit Tokenizer(const std::string& src)
        : m_src(src) {}

    inline std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buf;

        while (peak().has_value()) {
            if (std::isalpha(peak().value())) {
                buf.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value())) {
                    buf.push_back(consume());
                }
                if (buf == "return") {
                    tokens.push_back({ TokenType::_return });
                    buf.clear();
                    continue;
                }
            } else if (std::isdigit(peak().value())) {
                buf.push_back(consume());
                while (peak().has_value() && std::isdigit(peak().value())) {
                    buf.push_back(consume());
                }
                tokens.push_back({ TokenType::_int_lit, buf });
                buf.clear();
                continue;
            } else if (std::isspace(peak().value())) {
                consume();
                continue;
            } else if (peak().value() == ';') {
                consume();
                tokens.push_back({ TokenType::semi });
                continue;
            } else {
                std::cerr << "Parse Failure" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }

private:
    [[nodiscard]] std::optional<char> peak(int head = 1) const {
        if (m_index + head > m_src.length()) {
            return {};
        }
        return m_src.at(m_index);
    }

    char consume() {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    int m_index = 0;
};
