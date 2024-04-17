#pragma once

namespace Node {

    struct exit
    {
        
    };
    
    struct expr
    {
  
    };
    
}
class Parser
{
public:

    inline explicit Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens)) {}
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