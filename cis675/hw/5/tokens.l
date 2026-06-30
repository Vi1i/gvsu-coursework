%option noyywrap
%{
#include <string>
#include <cstdio>
#include <iostream>

#include "node.hpp"
#include "parser.hpp"

#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)

void count();
%}

%%
[ \t\n\r]       {count();}
[/][*].*?[*][/] ;
strict  {count(); return TOKEN(T_STRICT);}
graph   {count(); return TOKEN(T_GRAPH);}
digraph {count(); return TOKEN(T_DIGRAPH);}
"{"     {count(); return TOKEN(T_LBRACE);}
"}"     {count(); return TOKEN(T_RBRACE);}
";"     {count(); return TOKEN(T_SEMICOLON);}
"="     {count(); return TOKEN(T_EQUALS);}
node    {count(); return TOKEN(T_NODE);}
edge    {count(); return TOKEN(T_EDGE);}
"["     {count(); return TOKEN(T_LBRACK);}
"]"     {count(); return TOKEN(T_RBRACK);}
","     {count(); return TOKEN(T_COMMA);}
"--"    {count(); return TOKEN(T_UEDGE);}
"->"    {count(); return TOKEN(T_DEDGE);}
":"     {count(); return TOKEN(T_COLON);}
n       {count(); return TOKEN(T_N);}
ne      {count(); return TOKEN(T_NE);}
e       {count(); return TOKEN(T_E);}
se      {count(); return TOKEN(T_SE);}
s       {count(); return TOKEN(T_S);}
sw      {count(); return TOKEN(T_SW);}
w       {count(); return TOKEN(T_W);}
nw      {count(); return TOKEN(T_NW);}
c       {count(); return TOKEN(T_C);}
"_"     {count(); return TOKEN(T_UNDERSCORE);}
\".*?\" {count(); SAVE_TOKEN; return T_ID;}
\<.*?\> {count(); SAVE_TOKEN; return T_ID;}
-?(\\.[0-9]+|[0-9]+(\\.[0-9]*)?)                    {count(); SAVE_TOKEN; return T_ID;}
([a-zA-Z\200-\377])(([a-zA-Z\200-\377])|[_]|[0-9])* {count(); SAVE_TOKEN; return T_ID;}
.       ;
%%

int column = 0;
void count() {
    int i;
    for(i = 0; yytext[i] != '\0'; ++i) {
        if(yytext[i] == '\n') {
            column = 0;
        }else if(yytext[i] == '\t'){
            column += 8 - (column % 8);
        }else{
            column++;
        }
    }
    ECHO;
}
