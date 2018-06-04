#include <iostream>
#include <regex>

#include "chow/Lexer.hpp"

namespace chow {
    Lexer::~Lexer() {}

    std::vector<Token> Lexer::Lex() {
        std::vector<Token> tokens;
        ls.Consume();
        while(!ls.IsEOF()) {
            std::string lexeme(this->Lexeme());
            TokenType type = this->GetTokenType(lexeme);
            Token token = {lexeme, type};

            if(token.type == TokenType::WHITESPACE) {
                continue;
            }

            tokens.push_back(token);
        }
        return tokens;
    }

    TokenType Lexer::GetTokenType(std::string lexeme) {
        std::regex id_1("^([a-zA-Z\200-\377])(([a-zA-Z\200-\377])|[_]|[0-9])*");
        std::regex id_2("^-?(\\.[0-9]+|[0-9]+(\\.[0-9]*)?)$");
        std::regex id_3("^\".*?\"$");
        std::regex id_4("^<.*?>$");
        if(lexeme == "{") {
            return TokenType::LBRACE; 
        }
        if(lexeme == "}") {
            return TokenType::RBRACE; 
        }
        if(lexeme == "[") {
            return TokenType::LBRACK; 
        }
        if(lexeme == "]") {
            return TokenType::RBRACK; 
        }
        if(lexeme == "\n") {
            return TokenType::NEWLINE;
        }
        if(lexeme == "") {
            return TokenType::WHITESPACE;
        }
        if(lexeme == "=") {
            return TokenType::EQUALS;
        }
        if(lexeme == "->") {
            return TokenType::EDGEOP;
        }
        if(lexeme == "--") {
            return TokenType::EDGEOP;
        }
        if(lexeme == ";") {
            return TokenType::SEMICOLON;
        }
        if(lexeme == ",") {
            return TokenType::COMMA;
        }
        if(lexeme == "digraph") {
            return TokenType::DIGRAPH;
        }
        if(lexeme == "node") {
            return TokenType::NODE;
        }
        if(lexeme == "/*") {
            return TokenType::SCOMMENT;
        }
        if(lexeme == "*/") {
            return TokenType::ECOMMENT;
        }

        if(std::regex_match(lexeme, id_1)) {
            return TokenType::ID;
        }
        if(std::regex_match(lexeme, id_2)) {
            return TokenType::ID;
        }
        if(std::regex_match(lexeme, id_3)) {
            return TokenType::ID;
        }
        if(std::regex_match(lexeme, id_4)) {
            return TokenType::ID;
        }
        return TokenType::UNKNOWN;
    }

    std::string Lexer::Lexeme() {
        std::string lexeme("");
        CharELM elm = this->ls.GetELM();
        lexeme += elm.c;

        switch(elm.type) {
            case CharType::DASH:
                if(!this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
            case CharType::PERIOD:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
            case CharType::QUOTE:
                this->quote = !this->quote;
                if(!this->quote) {
                    this->ls.Consume();
                    return "\"";
                }
            case CharType::DIGIT:
            case CharType::UNDERSCORE:
            case CharType::ALPHA:
                this->lexing = true;
                this->ls.Consume();
                return lexeme + this->Lexeme();
                break;
            case CharType::RBRACE:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
            case CharType::LBRACE:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                break;
            case CharType::LBRACK:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                break;
            case CharType::RBRACK:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                if(this->lexing) {
                    this->lexing = false;
                    return "";
                }
                break;
            case CharType::SEMICOLON:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                if(this->lexing) {
                    this->lexing = false;
                    return "";
                }
                break;
            case CharType::EQUALS:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                if(this->lexing) {
                    this->lexing = false;
                    return "";
                }
                lexeme = "=";
                break;
            case CharType::BSLASH:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                break;
            case CharType::SLASH:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                if(this->lexing) {
                    this->lexing = false;
                    return "";
                }
                if(this->asterisk) {
                    this->ls.Consume();
                    this->asterisk = false;
                    return lexeme;
                }
                this->slash = !this->slash;
                if(this->slash) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                break;
            case CharType::ASTERISK:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                if(this->lexing) {
                    this->lexing = false;
                    return "";
                }
                if(this->slash) {
                    this->ls.Consume();
                    this->slash = false;
                    return lexeme;
                }
                this->asterisk = true;
                if(this->asterisk) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                break;
            case CharType::COMMA:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
                if(this->lexing) {
                    this->lexing = false;
                    return "";
                }
                break;
            case CharType::NEWLINE:
            case CharType::SPACE:
                if(this->quote) {
                    this->ls.Consume();
                    return lexeme + this->Lexeme();
                }
            case CharType::TAB:
                this->ls.Consume();
                return "";
                break;
            default:
                break;
        }
        this->ls.Consume();
        return lexeme;
    }
}
