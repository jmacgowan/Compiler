#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

enum class TokenType {
    _return,
    _int_lit,
    semi
};

struct Token
{
    TokenType type;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string& str){
    std::vector<Token> tokens;
    std::string buf;
    for (int i = 0; i < str.length(); i++)
    {
        char c = str.at(i);
        if (std::isalpha(c))
        {
            buf.push_back(c);
            i++;
            while (isalnum(str.at(i))){
                buf.push_back(str.at(i));
                i++;
            }
            i--;

            if (buf == "return"){
                tokens.push_back({.type = TokenType::_return});
                buf.clear();
                continue;
            } else if(std::isdigit(c)){
                while (isalnum(str.at(i))){
                    buf.push_back(str.at(i));
                    i++;
                    }  
            i--;
            tokens.push_back({.type = TokenType::_int_lit, .value = buf});
            buf.clear();
            } else if (std::isspace(c)){
                continue;
            }
            
        }
        
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cerr << "Incorrect Usage. Correct usage follows." << std::endl;
        std::cerr << "nim <input.ni>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    tokenize(contents);
    return 0;
}
