#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <vector>

enum class CharType {
    SPACE = 0,
    LBRACE,
    RBRACE,
    LBRACK,
    RBRACK,
    LPAREN,
    RPAREN,
    LANGLE,
    RANGLE,
    QUOTE,
    NEWLINE,
    TAB,
    COMMA,
    COLON,
    SEMICOLON,
    EQUALS,
    PERIOD,
    UNDERSCORE,
    SLASH,
    BSLASH,
    DASH,
    ASTERISK,
    ALPHA,
    DIGIT,

    UNKNOWN,
};

std::ostream & operator<<(std::ostream& out, const CharType type) {
    std::string str;
    switch(type) {
        case CharType::SPACE     : str = "SPACE"; break;
        case CharType::LBRACE    : str = "LBRACE"; break;
        case CharType::RBRACE    : str = "RBRACE"; break;
        case CharType::LBRACK    : str = "LBRACK"; break;
        case CharType::RBRACK    : str = "RBRACK"; break;
        case CharType::LPAREN    : str = "LPAREN"; break;
        case CharType::RPAREN    : str = "RPAREN"; break;
        case CharType::LANGLE    : str = "LANGLE"; break;
        case CharType::RANGLE    : str = "RANGLE"; break;
        case CharType::QUOTE     : str = "QUOTE"; break;
        case CharType::NEWLINE   : str = "NEWLINE"; break;
        case CharType::TAB       : str = "TAB"; break;
        case CharType::COMMA     : str = "COMMA"; break;
        case CharType::COLON     : str = "COLON"; break;
        case CharType::SEMICOLON : str = "SEMICOLON"; break;
        case CharType::EQUALS    : str = "EQUALS"; break;
        case CharType::PERIOD    : str = "PERIOD"; break;
        case CharType::UNDERSCORE: str = "UNDERSCORE"; break;
        case CharType::SLASH     : str = "SLASH"; break;
        case CharType::BSLASH    : str = "BSLASH"; break;
        case CharType::DASH      : str = "DASH"; break;
        case CharType::ASTERISK  : str = "ASTERISK"; break;
        case CharType::ALPHA     : str = "ALPHA"; break;
        case CharType::DIGIT     : str = "DIGIT"; break;
        case CharType::UNKNOWN   : str = "UNKNOWN"; break;
    }

    return out << str;
}

enum class TokenType  {
    STRICT,
    GRAPH,
    DIGRAPH,
    SUBGRAPH,
    EDGEOP,
    COMMENT,

    LBRACE,
    RBRACE,
    LBRACK,
    RBRACK,
    LPAREN,
    RPAREN,
    LANGLE,
    RANGLE,
    EQUALS,
    SEMICOLON,
    NEWLINE,

    ID,

    UNKNOWN,
};

struct Token {
    TokenType type;
    std::string str;
};

std::ostream & operator<<(std::ostream& out, const Token token) {
    std::string str;
    switch(token.type) {
        case TokenType::LBRACE   : str = "LBRACE"; break;
        case TokenType::RBRACE   : str = "RBRACE"; break;
        case TokenType::LBRACK   : str = "LBRACK"; break;
        case TokenType::RBRACK   : str = "RBRACK"; break;
        case TokenType::LPAREN   : str = "LPAREN"; break;
        case TokenType::RPAREN   : str = "RPAREN"; break;
        case TokenType::LANGLE   : str = "LANGLE"; break;
        case TokenType::RANGLE   : str = "RANGLE"; break;
        case TokenType::STRICT   : str = "STRICT"; break;
        case TokenType::GRAPH    : str = "GRAPH"; break;
        case TokenType::DIGRAPH  : str = "DIGRAPH"; break;
        case TokenType::SUBGRAPH : str = "SUBGRAPH"; break;
        case TokenType::EDGEOP   : str = "EDGEOP"; break;
        case TokenType::COMMENT  : str = "COMMENT"; break;
        case TokenType::ID       : str = "ID"; break;
        case TokenType::EQUALS   : str = "EQUALS"; break;
        case TokenType::SEMICOLON: str = "SEMICOLON"; break;
        case TokenType::NEWLINE  : str = "NEWLINE"; break;
        case TokenType::UNKNOWN  : str = "UNKNOWN"; break;
    }

    return out << str;
}

CharType char_type(char c) {
    switch(c) {
        case ' ':
            return CharType::SPACE;
            break;
        case '{':
            return CharType::LBRACE;
            break;
        case '}':
            return CharType::RBRACE;
            break;
        case '[':
            return CharType::LBRACK;
            break;
        case ']':
            return CharType::RBRACK;
            break;
        case '(':
            return CharType::LPAREN;
            break;
        case ')':
            return CharType::RPAREN;
            break;
        case '<':
            return CharType::LANGLE;
            break;
        case '>':
            return CharType::RANGLE;
            break;
        case '"':
            return CharType::QUOTE;
            break;
        case '\n':
            return CharType::NEWLINE;
            break;
        case '\t':
            return CharType::TAB;
            break;
        case ',':
            return CharType::COMMA;
            break;
        case ':':
            return CharType::COLON;
            break;
        case ';':
            return CharType::SEMICOLON;
            break;
        case '=':
            return CharType::EQUALS;
            break;
        case '.':
            return CharType::PERIOD;
            break;
        case '_':
            return CharType::UNDERSCORE;
            break;
        case '/':
            return CharType::SLASH;
            break;
        case '\\':
            return CharType::BSLASH;
            break;
        case '-':
            return CharType::DASH;
            break;
        case '*':
            return CharType::ASTERISK;
            break;
    }

    if(isdigit(c)) {
        return CharType::DIGIT;
    }

    if(isalpha(c)) {
        return CharType::ALPHA;
    }

    return CharType::UNKNOWN;
}

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

    std::vector<Token> tokens;

    char c;
    bool prev_slash = false;
    bool prev_asterisk = false;
    bool not_consume = false;
    while(not_consume || ifs.get(c)) {
        not_consume = false;
        CharType c_type = char_type(c);
        if(c_type == CharType::QUOTE) {
            prev_slash = false;
            std::string lexeme(1, c);
            while(ifs.get(c)) {
                c_type = char_type(c);
                if(c_type == CharType::QUOTE) {
                    lexeme += c;
                    break;
                }
                lexeme += c;
            }

            tokens.push_back({TokenType::ID, lexeme});
            //not_consume = true;
        }else if(c_type == CharType::ALPHA) {
            prev_slash = false;
            std::string lexeme(1, c);
            while(ifs.get(c)) {
                c_type = char_type(c);
                if(!(c_type == CharType::ALPHA || c_type == CharType::UNDERSCORE)) {
                    break;
                }
                lexeme += c;
            }

            if(lexeme == "digraph") {
                tokens.push_back({TokenType::DIGRAPH, lexeme});
            }else{
                tokens.push_back({TokenType::ID, lexeme});
            }
            not_consume = true;
        }else if(c_type == CharType::DIGIT) {
            std::string lexeme(1, c);
            while(ifs.get(c)) {
                c_type = char_type(c);
                if(c_type != CharType::DIGIT) {
                    break;
                }
                lexeme += c;
            }

            tokens.push_back({TokenType::ID, lexeme});
            not_consume = true;
        }else if(c_type == CharType::SLASH) {
            prev_slash = true;
        }else if(c_type == CharType::ASTERISK && prev_slash) {
            prev_slash = false;

            std::string lexeme("/");
            lexeme += c;
            while(ifs.get(c)) {
                c_type = char_type(c);
                if(c_type == CharType::ASTERISK) {
                    prev_asterisk = true;
                }else if(c_type == CharType::SLASH){
                    prev_asterisk = false;
                lexeme += c;
                    break;
                }else{
                    prev_asterisk = false;
                }
                lexeme += c;
            }

            tokens.push_back({TokenType::COMMENT, lexeme});
            not_consume = true;
        }else if(c_type == CharType::DASH) {
            std::string lexeme(1, c);
            ifs.get(c);
            c_type = char_type(c);
            if(c_type == CharType::RANGLE) {
                lexeme += c;
                tokens.push_back({TokenType::EDGEOP, lexeme});
            }else{
                tokens.push_back({TokenType::RANGLE, std::string(&c)});
                not_consume = true;
            }
        }else if(c_type == CharType::LBRACE) {
            tokens.push_back({TokenType::LBRACE, std::string(&c)});
        }else if(c_type == CharType::RBRACE) {
            tokens.push_back({TokenType::RBRACE, std::string(&c)});
        }else if(c_type == CharType::LBRACK) {
            tokens.push_back({TokenType::LBRACK, std::string(&c)});
        }else if(c_type == CharType::RBRACK) {
            tokens.push_back({TokenType::RBRACK, std::string(&c)});
        }else if(c_type == CharType::EQUALS) {
            tokens.push_back({TokenType::EQUALS, std::string(&c)});
        }else if(c_type == CharType::SEMICOLON) {
            tokens.push_back({TokenType::SEMICOLON, std::string(&c)});
        }else if(c_type == CharType::NEWLINE) {
            tokens.push_back({TokenType::NEWLINE, std::string(&c)});
        }else{
            prev_slash = false;
            //std::cout << c_type <<  std::endl;
        }
    }

    ifs.close();

    int line_num = 1;
    std::cout << line_num << "\t";
    line_num++;
    for(auto token: tokens) {
        if(token.type == TokenType::NEWLINE) {
            std::cout << std::endl << line_num << "\t";
            line_num++;
        }else{
            std::cout << token << " ";
        }
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
