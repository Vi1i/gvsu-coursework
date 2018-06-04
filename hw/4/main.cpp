#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "chow/LexerStream.hpp"
#include "chow/Lexer.hpp"
#include "chow/Parser.hpp"

int main(int argc, char * argv[]) {
    if(argc != 2) {
        std::cerr << "One argument expected" << std::endl;
    }

    std::string file(argv[1]);
    std::ifstream ifs;
    ifs.open(file, std::ios::in);

    if(!ifs) {
        std::cerr << "The file does not exist!" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<chow::Token> tokens;
    chow::LexerStream ls(ifs);
    chow::Lexer lexer(ls);

    tokens = lexer.Lex();

    chow::Parser parser(tokens);

    parser.Print();
    if(!parser.Parse()) {
        std::cerr << "Parsing failed!" << std::endl;
    }else{
        std::cout << "Parsing passed!" << std::endl;
    }

    return EXIT_SUCCESS;
}
