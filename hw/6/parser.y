%{
#include "node.hpp"

extern int yylex();

void yyerror(const char* s);

chow::Graph* programBlock;
%}

%union {
    bool _bool;
    std::string* string;
    int token;
    chow::Node* node;
    chow::Graph* _graph;
    chow::Type* _type;
    chow::Identifier * _id;
    chow::StatementList* _statementList;
    chow::Statement* _statement;
    chow::AttrList* _attrList;
    chow::AList* _aList;
    chow::EdgeRhs* _edgeRhs;
    chow::EdgeType _edgeType;
}

%token <token> T_STRICT
%token <token> T_GRAPH
%token <token> T_DIGRAPH
%token <string> T_ID
%token <token> T_LBRACE
%token <token> T_RBRACE
%token <token> T_EQUALS
%token <token> T_NODE
%token <token> T_EDGE
%token <token> T_LBRACK
%token <token> T_RBRACK
%token <token> T_SEMICOLON
%token <token> T_COMMA
%token <token> T_DEDGE
%token <token> T_UEDGE
%token <token> T_COLON
%token <token> T_SUBGRAPH
%token <token> T_N
%token <token> T_NE
%token <token> T_E
%token <token> T_SE
%token <token> T_S
%token <token> T_SW
%token <token> T_W
%token <token> T_NW
%token <token> T_C
%token <token> T_UNDERSCORE

%type <_graph> program graph
%type <_bool> strict
%type <_type> graph_t
%type <_id> id node_id
%type <_statementList> stmt_list
%type <_statement> stmt node_stmt edge_stmt attr_stmt subgraph
%type <_attrList> attr_list
%type <_aList> a_list
%type <_edgeRhs> edgeRHS
%type <_edgeType> edgeop

%start program

%%
program: graph {$$ = $1; programBlock = $$;}
       ;

graph: strict graph_t id T_LBRACE stmt_list T_RBRACE {$$ = new chow::Graph($1, *$2, *$3, *$5);}
     ;

strict: {$$ = false;}
      | T_STRICT {$$ = true;}
      ;

id: {$$ = new chow::Identifier("");}
  | T_ID {$$ = new chow::Identifier(*$1);}
  ;

graph_t: T_GRAPH {$$ = new chow::Type(chow::GraphType::GRAPH);}
       | T_DIGRAPH {$$ = new chow::Type(chow::GraphType::DIGRAPH);}
       ;

stmt_list: {$$ = new chow::StatementList(new chow::Statement(), nullptr);}
         | stmt {$$ = new chow::StatementList($1, nullptr);}
         | stmt stmt_list {$$ = new chow::StatementList($1, $2);}
         | stmt T_SEMICOLON stmt_list {$$ = new chow::StatementList($1, $3);}
         ;

stmt: node_stmt {$$ = $1;}
    | edge_stmt {$$ =$1;}
    | attr_stmt {$$ =$1;}
    | T_ID T_EQUALS T_ID {$$ = new chow::IdAssignId(chow::Identifier(*$1), chow::Identifier(*$3));}
    | subgraph {$$ =$1;}
    ;

node_stmt: node_id attr_list {$$ = new chow::NodeStatement($1, $2);}
         | node_id {$$ = new chow::NodeStatement($1, nullptr);}
         ;

node_id: T_ID port {$$ = new chow::Identifier(*$1);}
       | T_ID {$$ = new chow::Identifier(*$1);}
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

attr_list: T_LBRACK a_list T_RBRACK attr_list {$$ = new chow::AttrList($2, $4);}
         | T_LBRACK a_list T_RBRACK {$$ = new chow::AttrList($2, nullptr);}
         ;

a_list: T_ID T_EQUALS T_ID T_SEMICOLON a_list {$$ = new chow::AList(chow::Identifier(*$1), chow::Identifier(*$3), $5);}
      | T_ID T_EQUALS T_ID T_SEMICOLON {$$ = new chow::AList(chow::Identifier(*$1), chow::Identifier(*$3), nullptr);}
      | T_ID T_EQUALS T_ID T_COMMA a_list {$$ = new chow::AList(chow::Identifier(*$1), chow::Identifier(*$3), $5);}
      | T_ID T_EQUALS T_ID T_COMMA {$$ = new chow::AList(chow::Identifier(*$1), chow::Identifier(*$3), nullptr);}
      | T_ID T_EQUALS T_ID a_list {$$ = new chow::AList(chow::Identifier(*$1), chow::Identifier(*$3), $4);}
      | T_ID T_EQUALS T_ID {$$ = new chow::AList(chow::Identifier(*$1), chow::Identifier(*$3), nullptr);}
      ;

edge_stmt: node_id edgeRHS {$$ = new chow::EdgeStatement($1, $2, nullptr);}
         | node_id edgeRHS attr_list {$$ = new chow::EdgeStatement($1, $2, nullptr);}
         | subgraph edgeRHS {$$ = new chow::EdgeStatement(nullptr, $2, nullptr);}
         | subgraph edgeRHS attr_list {$$ = new chow::EdgeStatement(nullptr, $2, $3);}
         ;

edgeRHS: edgeop node_id {$$ = new chow::EdgeRhs($1, $2, nullptr);}
       | edgeop node_id edgeRHS {$$ = new chow::EdgeRhs($1, $2, $3);}
       | edgeop subgraph {$$ = new chow::EdgeRhs($1, nullptr, nullptr);}
       | edgeop subgraph edgeRHS {$$ = new chow::EdgeRhs($1, nullptr, nullptr);}
       ;

edgeop: T_UEDGE {$$ = chow::EdgeType::UEDGE;}
      | T_DEDGE {$$ = chow::EdgeType::DEDGE;}
      ;

attr_stmt: T_GRAPH attr_list {$$ = new chow::AttrStatement();}
         | T_NODE attr_list {$$ = new chow::AttrStatement();}
         | T_EDGE attr_list {$$ = new chow::AttrStatement();}
         ;

subgraph: T_LBRACE stmt_list T_RBRACE {$$ = new chow::Subgraph();}
        | T_SUBGRAPH T_LBRACE stmt_list T_RBRACE {$$ = new chow::Subgraph();}
        | T_SUBGRAPH T_ID T_LBRACE stmt_list T_RBRACE {$$ = new chow::Subgraph();}
        ;
%%
