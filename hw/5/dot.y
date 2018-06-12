%{
#include <cstdio>
#include <iostream>

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE* yyin;

void yyerror(const char* s);
%}

%token T_STRICT
%token T_GRAPH
%token T_DIGRAPH
%token T_ID
%token T_LBRACE
%token T_RBRACE
%token T_EQUALS
%token T_NODE
%token T_EDGE
%token T_LBRACK
%token T_RBRACK
%token T_SEMICOLON
%token T_COMMA
%token T_DEDGE
%token T_UEDGE
%token T_COLON
%token T_SUBGRAPH
%token T_N
%token T_NE
%token T_E
%token T_SE
%token T_S
%token T_SW
%token T_W
%token T_NW
%token T_C
%token T_UNDERSCORE

%%
graph: strict graph_t id T_LBRACE stmt_list T_RBRACE
     ;

strict:
      | T_STRICT
      ;

id:
  | T_ID
  ;

graph_t: T_GRAPH
       | T_DIGRAPH
       ;

stmt_list:
         | stmt
         | stmt stmt_list
         | stmt T_SEMICOLON stmt_list
         ;

stmt: node_stmt
    | edge_stmt
    | attr_stmt
    | T_ID T_EQUALS T_ID
    | subgraph
    ;

node_stmt: node_id attr_list
         | node_id
         ;

node_id: T_ID port
       | T_ID
       ;

port: T_COLON T_ID T_COLON compass_pt
    | T_COLON T_ID
    | T_COLON compass_pt
    ;

compass_pt: T_N
          | T_NE
          | T_E
          | T_SE
          | T_S
          | T_SW
          | T_W
          | T_NW
          | T_C
          | T_UNDERSCORE
          ;

attr_list: T_LBRACK a_list T_RBRACK attr_list
         | T_LBRACK a_list T_RBRACK
         ;

a_list: T_ID T_EQUALS T_ID T_SEMICOLON a_list
      | T_ID T_EQUALS T_ID T_SEMICOLON
      | T_ID T_EQUALS T_ID T_COMMA a_list
      | T_ID T_EQUALS T_ID T_COMMA
      | T_ID T_EQUALS T_ID a_list
      | T_ID T_EQUALS T_ID
      ;

edge_stmt: node_id edgeRHS
         | node_id edgeRHS attr_list
         | subgraph edgeRHS
         | subgraph edgeRHS attr_list
         ;

edgeRHS: edgeop node_id
       | edgeop node_id edgeRHS
       | edgeop subgraph
       | edgeop subgraph edgeRHS
       ;

edgeop: T_UEDGE
      | T_DEDGE
      ;

attr_stmt: T_GRAPH attr_list
         | T_NODE attr_list
         | T_EDGE attr_list
         ;

subgraph: T_LBRACE stmt_list T_RBRACE
        | T_SUBGRAPH T_LBRACE stmt_list T_RBRACE
        | T_SUBGRAPH T_ID T_LBRACE stmt_list T_RBRACE
        ;
%%

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
