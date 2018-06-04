#include "chow/Parser.hpp"

#include <iostream>
#include <string>

namespace chow {
    Parser::~Parser() {}

    bool Parser::Parse() {
        Token token = tokens.front();
        tokens.erase(tokens.begin());
        switch(token.type) {
            case TokenType::DIGRAPH:
                return this->Graph();
                break;
            default:
#ifdef DEBUG
                std::cerr << "Bad Start" << std::endl;
#endif
                return false;
        }
    }

    bool Parser::Graph() {
#ifdef DEBUG
        std::cout << "Graph()" << std::endl;
#endif
        Token id = tokens.front();
        tokens.erase(tokens.begin());
        Token lbrace = tokens.front();
        tokens.erase(tokens.begin());

        if(id.type != TokenType::ID) {
#ifdef DEBUG
            std::cerr << "BAD GRAPH ID" << std::endl;
#endif
            return false;
        }
        if(lbrace.type != TokenType::LBRACE) {
#ifdef DEBUG
            std::cerr << "BAD GRAPH LBRACE" << std::endl;
#endif
            return false;
        }

        return this->StmtList();
    }

    bool Parser::StmtList() {
#ifdef DEBUG
        std::cout << "StmtList()" << std::endl;
#endif
        Token first = tokens.front();
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
#endif
        this->Stmt();

        first = tokens.front();
#ifdef DEBUG
        std::cout << "StmtList()" << std::endl;
        std::cout << "\t" << first << std::endl;
#endif
        if(first.type != TokenType::SEMICOLON) {
            if(first.type == TokenType::RBRACE) {
                return true;
            }
            return false;
        }
        tokens.erase(tokens.begin());

        return this->StmtList();
    }

    bool Parser::Stmt() {
#ifdef DEBUG
        std::cout << "Stmt()" << std::endl;
#endif
        Token first = tokens[0];
        Token second = tokens[1];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
#endif

        switch(first.type) {
            case TokenType::NODE:
            case TokenType::GRAPH:
            case TokenType::EDGE:
                tokens.erase(tokens.begin());
                return this->AttrList();
                break;
            case TokenType::ID:
                if(second.type == TokenType::EQUALS) {
                    Token third = tokens.front();
                    return third.type == TokenType::ID;
                }
                if(second.type == TokenType::LBRACK) {
                    return this->NodeStmt();
                }
                if(second.type == TokenType::EDGEOP) {
                    return this->EdgeStmt();
                }
                return false;
                break;
            case TokenType::SCOMMENT:
                return this->Comment();
            default:
                return false;
        }
    }

    bool Parser::Comment() {
#ifdef DEBUG
        std::cout << "Comment()" << std::endl;
#endif
        Token first = tokens[0];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
#endif

        if(tokens.front().type == TokenType::ECOMMENT) {
            tokens.erase(tokens.begin());
            std::vector<Token> tmp;
            tmp.push_back({";", TokenType::SEMICOLON});
            for(const auto & token : tokens) {
                tmp.push_back(token);
            }
            tokens = tmp;
            return true;
        }
        tokens.erase(tokens.begin());

        return this->Comment();
    }

    bool Parser::NodeStmt() {
#ifdef DEBUG
        std::cout << "NodeStmt()" << std::endl;
#endif
        Token first = tokens[0];
        tokens.erase(tokens.begin());
        Token second = tokens[1];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
#endif

        bool rc_NodeID = this->NodeID();
        bool rc_AttrList = this->AttrList();

#ifdef DEBUG
        std::cout << "NodeStmt()" << std::endl;
#endif
        first = tokens[0];
        second = tokens[1];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
#endif

        return rc_NodeID | rc_AttrList;
    }

    bool Parser::EdgeStmt() {
#ifdef DEBUG
        std::cout << "EdgeStmt()" << std::endl;
#endif
        Token first = tokens[0];
        Token second = tokens[1];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
#endif

        bool rc_NodeID = this->NodeID();

#ifdef DEBUG
        std::cout << "EdgeStmt()" << std::endl;
#endif
        first = tokens[0];
        second = tokens[1];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
#endif

        bool rc_EdgeRhs(false);
        bool rc_AttrList(false);
        bool endStmt(false);
        if(second.type == TokenType::EDGEOP) {
            tokens.erase(tokens.begin());
            rc_EdgeRhs = this->EdgeRhs();
            if(tokens.front().type == TokenType::LBRACK) {
                rc_AttrList = this->AttrList();
            }
            if(tokens.front().type == TokenType::SEMICOLON) {
                endStmt = true;
            }
        }

#ifdef DEBUG
        std::cout << "EdgeStmt()" << std::endl;
#endif
        first = tokens[0];
        second = tokens[1];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
#endif

        return rc_NodeID | rc_EdgeRhs | rc_AttrList | endStmt;
    }
    bool Parser::AttrStmt() { return false; }

    bool Parser::EdgeRhs() {
#ifdef DEBUG
        std::cout << "EdgeRhs()" << std::endl;
#endif
        Token first = tokens[0];
        Token second = tokens[1];
        Token third = tokens[2];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
        std::cout << "\t" << third << std::endl;
#endif

        if(first.type == TokenType::EDGEOP) {
            if(second.type == TokenType::ID) {
                tokens.erase(tokens.begin());
                tokens.erase(tokens.begin());
                return true;
            }
            if(second.type == TokenType::SEMICOLON) {
                tokens.erase(tokens.begin());
                return true;
            }
        }
        return false;
    }

    bool Parser::AttrList() {
#ifdef DEBUG
        std::cout << "AttrList()" << std::endl;
#endif
        Token first = tokens[0];
        Token second = tokens[1];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
#endif

        if(first.type == TokenType::LBRACK) {
            while(tokens.front().type != TokenType::RBRACK) {
                tokens.erase(tokens.begin());
                bool rc = this->AList();
                if(!rc) {
                    return false;
                }

#ifdef DEBUG
                std::cout << "AttrList()" << std::endl;
#endif
                first = tokens[0];
                second = tokens[1];
#ifdef DEBUG
                std::cout << "\t" << first << std::endl;
                std::cout << "\t" << second << std::endl;
#endif
            }
            tokens.erase(tokens.begin());
            return true;
        }

        return false;
    }
    bool Parser::AList() {
#ifdef DEBUG
        std::cout << "AList()" << std::endl;
#endif
        Token first = tokens[0];
        Token second = tokens[1];
        Token third = tokens[2];
        Token fourth = tokens[3];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
        std::cout << "\t" << third << std::endl;
        std::cout << "\t" << fourth << std::endl;
#endif

        if(first.type == TokenType::ID && second.type == TokenType::EQUALS
            && third.type == TokenType::ID) {
            if(fourth.type == TokenType::COMMA || fourth.type == TokenType::SEMICOLON) {
                tokens.erase(tokens.begin());
                tokens.erase(tokens.begin());
                tokens.erase(tokens.begin());
                tokens.erase(tokens.begin());
                return this->AList();
            }
            if(fourth.type == TokenType::RBRACK) {
                tokens.erase(tokens.begin());
                tokens.erase(tokens.begin());
                tokens.erase(tokens.begin());
                return true;
            }
            return false;
        }

        return false;
    }

    bool Parser::NodeID() {
#ifdef DEBUG
        std::cout << "NodeID()" << std::endl;
#endif
        Token first = tokens[0];
        Token second = tokens[1];
#ifdef DEBUG
        std::cout << "\t" << first << std::endl;
        std::cout << "\t" << second << std::endl;
#endif

        if(first.type != TokenType::COLON) {
            return false;
        }
        return true;
    }

    bool Parser::Port() {
        return false;
    }

    bool Parser::SubGraph() { return false; }
    bool Parser::CompassPT() { return false; }

    void Parser::Print() {
        std::string space("");
        std::string newline("");
        for(const auto & token : tokens) {
            if(token.type == TokenType::RBRACE) {
                space = "";
                newline = "\n";
            }
            std::cout << newline << space << token.lexeme;
            if(token.type == TokenType::LBRACE) {
                space = "\t";
                newline = "\n";
            }else if(token.type == TokenType::SEMICOLON) {
                space = "\t";
                newline = "\n";
            }else if(token.type == TokenType::ECOMMENT) {
                space = "\t";
                newline = "\n";
            }else{
                space = " ";
                newline = "";
            }
        }
        std::cout << std::endl;
        //space = "";
        //newline = "";
        //for(const auto & token : tokens) {
        //    if(token.type == TokenType::RBRACE) {
        //        space = "";
        //        newline = "\n";
        //    }
        //    std::cout << newline << space << token.type;
        //    if(token.type == TokenType::LBRACE) {
        //        space = "\t";
        //        newline = "\n";
        //    }else if(token.type == TokenType::SEMICOLON) {
        //        space = "\t";
        //        newline = "\n";
        //    }else if(token.type == TokenType::ECOMMENT) {
        //        space = "\t";
        //        newline = "\n";
        //    }else{
        //        space = " ";
        //        newline = "";
        //    }
        //}
        //std::cout << std::endl;
    }
}
