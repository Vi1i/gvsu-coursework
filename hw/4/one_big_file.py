import sys

class TokenTypes(object):
    (STRICT, GRAPH, DIGRAPH, ID, LBRACE, RBRACE, SEMICOLON, EQUALS, NODE, EDGE,
     LBRACK, RBRACK, COMMA, COLON, EDGEOP, SUBGRAPH, N, NE, E, SE, S, SW, W, NW,
     C, UNDERLINE, EOF, COMMENT) = range(28)
    names = ["STRICT", "GRAPH", "DIGRAPH", "ID", "LBRACE", "RBRACE", "SEMICOLON",
     "EQUALS", "NODE", "EDGE", "LBRACK", "RBRACK", "COMMA", "COLON", "EDGEOP",
     "SUBGRAPH", "N", "NE", "E", "SE", "S", "SW", "W", "NW", "C", "UNDERLINE",
     "EOF", "COMMENT"]

class Token(object):
    def __init__(self, type, text):
        self.type = type
        self.text = text

    def __str__(self):
        return "<'%s', '%s'>" % (self.text, TokenTypes.names[self.type])

class Lexer(object):
    def __init__(self, str):
        self.input = str
        self.pos = 0
        self.c = self.input[self.pos]

    def __consume(self):
        self.pos += 1
        if self.pos >= len(self.input):
            self.c = TokenTypes.EOF
        else:
            self.c = self.input[self.pos]

    def next_token(self):
        while self.c != TokenTypes.EOF:
            if self.c in [' ', '\t']:
                self.__consume()
            elif self.c in ['\n', '\r']:
                self.__consume()
            elif self.c == ',':
                self.__consume()
                return Token(TokenTypes.COMMA, ',')
            elif self.c == '=':
                self.__consume()
                return Token(TokenTypes.EQUALS, '=')
            elif self.c == ';':
                self.__consume()
                return Token(TokenTypes.SEMICOLON, ';')
            elif self.c == '/':
                lexeme = ""
                lexeme += self.c
                self.__consume()
                if self.c != TokenTypes.EOF and self.c == '*':
                    lexeme += self.c
                    self.__consume()
                    while True:
                        if self.c == "*":
                            lexeme += self.c
                            self.__consume()
                            if self.c == "/":
                                lexeme += self.c
                                self.__consume()
                                break
                                #return Token(TokenTypes.COMMENT, lexeme)
                        if self.c in ['\n', '\r']:
                            print("Invalid character %c." % self.c, file=sys.stderr)
                            sys.exit(1)
                        lexeme += self.c
                        self.__consume()
            elif self.c == '-':
                self.__consume()
                if self.c != TokenTypes.EOF and self.c == '>':
                    self.__consume()
                    return Token(TokenTypes.EDGEOP, '->')
                print("Invalid character %c." % self.c, file=sys.stderr)
                sys.exit(1)
            elif self.c == '{':
                self.__consume()
                return Token(TokenTypes.LBRACE, '{')
            elif self.c == '}':
                self.__consume()
                return Token(TokenTypes.RBRACE, '}')
            elif self.c == '[':
                self.__consume()
                return Token(TokenTypes.LBRACK, '[')
            elif self.c == ']':
                self.__consume()
                return Token(TokenTypes.RBRACK, ']')
            elif self.c.isdigit():
                self.__consume()
                return Token(TokenTypes.ID, self.c)
            elif self.c == "\"":
                lexeme = "\""
                self.__consume()
                while self.c != TokenTypes.EOF and self.c != "\"":
                    lexeme += self.c
                    self.__consume()
                lexeme += self.c
                self.__consume()
                if lexeme == "digraph":
                    t = Token(TokenTypes.DIGRAPH, lexeme)
                elif lexeme == "node":
                    t = Token(TokenTypes.NODE, lexeme)
                else:
                    t = Token(TokenTypes.ID, lexeme)
                return t
            elif self.c.isalpha():
                # Consume all contiguous alphabetic characters.
                lexeme = ""
                while self.c != TokenTypes.EOF and (self.c.isalpha() or self.c == "_"):
                    lexeme += self.c
                    self.__consume()
                if lexeme == "digraph":
                    t = Token(TokenTypes.DIGRAPH, lexeme)
                elif lexeme == "node":
                    t = Token(TokenTypes.NODE, lexeme)
                else:
                    t = Token(TokenTypes.ID, lexeme)
                return t
            else:
                print("Invalid character %c." % self.c, file=sys.stderr)
                sys.exit(1)
        return Token(TokenTypes.EOF, "<EOF>")

class AST(object):
    def __init__(self, token):
        self.token = token
        self.children = []

    def add_child(self, t):
        self.children.append(t)

    def __str__(self):
        return str(self.token)

    def to_string_tree(self):
        if len(self.children) == 0:
                return str(self)
        buf = ("(%s," % str(self.token))
        for z in range(len(self.children)):
            t = self.children[z]
            if t is None:
                continue
            if z > 0:
                buf += ","
            buf += t.to_string_tree()
        buf += ")"
        return buf

class Parser(object):
    def __init__(self, lexer):
        self.lexer = lexer
        self.lookahead = self.lexer.next_token()

    def __match(self, token=None, optional=False, or_tokens=None):
        if not token is None:
            if self.lookahead.type == token:
                self.lookahead = self.lexer.next_token()
            elif optional:
                return
            else:
                print("Expecting %s found %s" % (TokenTypes.names[token], self.lookahead), file=sys.stderr)
                sys.exit(1)
        elif not or_tokens is None:
            if self.lookahead.type in or_tokens:
                self.lookahead = self.lexer.next_token()
            else:
                for tok in or_tokens:
                    print("Expecting %s found %s" % (TokenTypes.names[tok], self.lookahead), file=sys.stderr)
                sys.exit(1)
        else:
            print("Expecting %s found %s" % (TokenTypes.names[token], self.lookahead), file=sys.stderr)
            sys.exit(1)

    def parse_port(self):
        print("parse_port()")

    def parse_node_id(self):
        node = AST('NODE_ID')
        if self.lookahead.type == TokenTypes.COLON:
            node.add_child(self.parse_port)
        return node

    def parse_node_stmt(self):
        node = AST('NODE_STMT')
        self.parse_node_id()
        if self.lookahead.type == TokenTypes.LBRACK:
            node.add_child(self.parse_attr_list())
        return node

    def parse_edge_rhs(self):
        node = AST('EDGE_RHS')
        self.__match(token=TokenTypes.EDGEOP)
        if self.lookahead.type == TokenTypes.SUBGRAPH:
            node.add_child(self.parse_subgraph())
        elif self.lookahead.type == TokenTypes.ID:
            self.__match(token=TokenTypes.ID)
            node.add_child(self.parse_node_id())

        if self.lookahead.type == TokenTypes.EDGEOP:
            node.add_child(self.parse_edge_rhs())
        return node

    def parse_edge_stmt(self):
        node = AST('EDGE_STMT')
        self.parse_edge_rhs()
        if self.lookahead.type == TokenTypes.LBRACK:
            node.add_child(self.parse_attr_list())
        return node

    def parse_a_list(self):
        node = AST('A_LIST')
        if self.lookahead.type == TokenTypes.RBRACK:
            return node
        self.__match(token=TokenTypes.ID)
        self.__match(token=TokenTypes.EQUALS)
        self.__match(token=TokenTypes.ID)
        return node

    def parse_attr_list(self):
        node = AST('ATTR_LIST')
        self.__match(token=TokenTypes.LBRACK)
        self.parse_a_list()
        while self.lookahead.type == TokenTypes.SEMICOLON or self.lookahead.type == TokenTypes.COMMA:
            self.__match(or_tokens=[TokenTypes.SEMICOLON, TokenTypes.COMMA])
            node.add_child(self.parse_a_list())
        self.__match(token=TokenTypes.RBRACK)
        return node

    def parse_attr_stmt(self):
        node = AST('ATTR_STMT')
        self.__match(or_tokens=[TokenTypes.GRAPH, TokenTypes.NODE, TokenTypes.EDGE])
        if self.lookahead.type == TokenTypes.LBRACK:
            node.add_child(self.parse_attr_list())
        return node

    def parse_subgraph(self):
        print("parse_subgraph()")

    def parse_stmt(self):
        node = AST('STMT')
        if self.lookahead.type == TokenTypes.RBRACE:
            return None
        if self.lookahead.type == TokenTypes.ID:
            self.__match(token=TokenTypes.ID)
            if self.lookahead.type == TokenTypes.EQUALS:
                self.__match(token=TokenTypes.EQUALS)
                self.__match(token=TokenTypes.ID)
                return node
            elif self.lookahead.type == TokenTypes.SUBGRAPH:
                print("\tSUBGRAPH")
                print("\t^---EDGE_STMT Possible")
                return node
            elif self.lookahead.type ==  TokenTypes.EDGEOP:
                node.add_child(self.parse_edge_stmt())
                return node
            else:
                node.add_child(self.parse_node_stmt())
                return node
        elif self.lookahead.type == TokenTypes.GRAPH or \
             self.lookahead.type == TokenTypes.NODE or \
             self.lookahead.type == TokenTypes.EDGE:
            node.add_child(self.parse_attr_stmt())
            return node
        elif self.lookahead.type == TokenTypes.SUBGRAPH:
            print("\tSUBGRAPH")
            print("\tEDGE_STMT")
            return node
        else:
            print("Expecting either %s, found %s" % (TokenTypes.names[TokenTypes.ID], TokenTypes.names[self.lookahead.type]), file=sys.stderr)
            sys.exit(1)
        return node

    def parse_stmt_list(self):
        node = AST('STMT_LIST')
        node.add_child(self.parse_stmt())
        while self.lookahead.type == TokenTypes.SEMICOLON:
            self.__match(TokenTypes.SEMICOLON)
            node.add_child(self.parse_stmt())
        return node

    def parse_graph(self):
        node = AST('GRAPH')
        self.__match(token=TokenTypes.STRICT, optional=True)
        self.__match(or_tokens=[TokenTypes.GRAPH, TokenTypes.DIGRAPH])
        self.__match(token=TokenTypes.ID)
        self.__match(token=TokenTypes.LBRACE)
        node.add_child(self.parse_stmt_list())
        self.__match(token=TokenTypes.RBRACE)

        return node

def main():
    filename = sys.argv[1]
    with open(filename, 'r') as myfile:
        data = myfile.read().replace('\n', '')

    lexer = Lexer(data)
    #t = lexer.next_token()
    #while (t.type != TokenTypes.EOF):
    #    print(t)
    #    t = lexer.next_token()
    parser = Parser(lexer)
    t = parser.parse_graph();
    print(t.to_string_tree())


if __name__ == "__main__":
    sys.exit(main())
