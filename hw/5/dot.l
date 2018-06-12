%{
#include <cstdio>
#include <iostream>
#include "dot.tab.h"
#define YY_DECL extern "C" int yylex()

void count();
%}

%%
[ \t\n\r]       {count();}
[/][*].*?[*][/] {count();}
strict  {count(); return T_STRICT;}
graph   {count(); return T_GRAPH;}
digraph {count(); return T_DIGRAPH;}
"{"     {count(); return T_LBRACE;}
"}"     {count(); return T_RBRACE;}
";"     {count(); return T_SEMICOLON;}
"="     {count(); return T_EQUALS;}
node    {count(); return T_NODE;}
edge    {count(); return T_EDGE;}
"["     {count(); return T_LBRACK;}
"]"     {count(); return T_RBRACK;}
","     {count(); return T_COMMA;}
"--"    {count(); return T_UEDGE;}
"->"    {count(); return T_DEDGE;}
":"     {count(); return T_COLON;}
n       {count(); return T_N;}
ne      {count(); return T_NE;}
e       {count(); return T_E;}
se      {count(); return T_SE;}
s       {count(); return T_S;}
sw      {count(); return T_SW;}
w       {count(); return T_W;}
nw      {count(); return T_NW;}
c       {count(); return T_C;}
"_"     {count(); return T_UNDERSCORE;}
\".*?\" {count(); return T_ID;}
\<.*?\> {count(); return T_ID;}
-?(\\.[0-9]+|[0-9]+(\\.[0-9]*)?)                    {count(); return T_ID;}
([a-zA-Z\200-\377])(([a-zA-Z\200-\377])|[_]|[0-9])* {count(); return T_ID;}
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
