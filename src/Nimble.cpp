#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"


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

    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<NodeExit> tree = parser.parse();

    if (!tree.has_value()){
        std::cerr << "No parsing occured" << std::endl;
        return EXIT_FAILURE;
    }
    
    Generator generator(tree.value());
    {
        std::fstream file("../out.asm", std::ios::out);
        file << generator.generate();
    } 

    system("nasm -felf64 ../out.asm");
    system("ld -o ../out ../out.o");

    return 0;
}
