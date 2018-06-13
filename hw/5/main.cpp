#include <iostream>

#include "node.hpp"

extern int yyparse();
extern "C" FILE* yyin;


int main(const int argc, const char* argv[]) {
    if(argc != 2) {
        return EXIT_FAILURE;
    }

    //std::FILE* myfile = std::fopen(argv[1], "r");
    std::FILE* myfile = std::fopen(argv[1], "r");
    if(!myfile) {
        std::cout << "I can't open" << argv[1]  << std::endl;
        return EXIT_FAILURE;
    }

    yyin = myfile;

    do {
        yyparse();
    } while (!std::feof(yyin));

    return EXIT_SUCCESS;
}

void yyerror(const char *s) {
    std::cerr << "Parse error!  Message: " << s << std::endl;
    exit(EXIT_FAILURE);
}
