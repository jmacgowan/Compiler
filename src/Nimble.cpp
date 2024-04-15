#include <iostream>
#include <fstream>

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cerr << "Incorrect Usage. Correct usage follows." << std::endl;
        std::cerr << "nim <input.ni>" << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
