#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

#include "chow/LexerStream.hpp"

namespace chow {
    LexerStream::~LexerStream() {
        this->ifs.close();
    }

    void LexerStream::Consume() {
        if(this->ifs.eof()) {
            this->eof = true;
        }else{
            char c;
            if(this->ifs.get(c)) {
                chow::CharType type = this->GetCharType(c);
                this->elm = {c, type};
            }else{
                this->elm = {'\0', CharType::_EOF};
            }
        }
    }

    CharELM LexerStream::GetELM() {
        return this->elm;
    }

    bool LexerStream::IsEOF() {
        return this->eof;
    }

    CharType LexerStream::GetCharType(char c) {
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
            return chow::CharType::DIGIT;
        }

        if(isalpha(c)) {
            return CharType::ALPHA;
        }

        return CharType::UNKNOWN;
    }
}
