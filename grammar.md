# Grammar Specification (BNF)

This file defines the grammar of our toy language using Backus–Naur Form (BNF).

---

## Program Structure
```
<program> ::= { <function-decl> }
```

---

## Function Declaration
```
<function-decl> ::= "fn" <type> <identifier> "(" <param-list> ")" "{" <stmt-list> "}"
<param-list> ::= <param> { "," <param> } | ε
<param> ::= <type> <identifier>
```

---

## Statements
```
<stmt-list> ::= { <stmt> }
<stmt> ::= <var-decl> | <return-stmt> | <expr-stmt>

<var-decl> ::= <type> <identifier> "=" <expr> ";"
<return-stmt> ::= "return" <expr> ";"
<expr-stmt> ::= <expr> ";"
```

---

## Expressions
```
<expr> ::= <term> { ("+" | "-") <term> }
<term> ::= <factor> { ("*" | "/") <factor> }
<factor> ::= <identifier> | <literal> | "(" <expr> ")"
```

---

## Literals and Types
```
<literal> ::= <intlit> | <floatlit> | <stringlit> | "true" | "false"
<type> ::= "int" | "float" | "string" | "bool"
```

---

## Lexical Elements
```
<identifier> ::= T_IDENTIFIER
<intlit> ::= T_INTLIT
<floatlit> ::= T_FLOATLIT
<stringlit> ::= T_STRINGLIT
```

---

### Example Program
```c
fn int add(int a, int b) {
    int result = a + b;
    return result;
}
```

Tokens produced:
```
T_FUNCTION, T_INT, T_IDENTIFIER("add"), T_PARENL, T_INT, T_IDENTIFIER("a"),
T_COMMA, T_INT, T_IDENTIFIER("b"), T_PARENR, T_BRACEL,
T_INT, T_IDENTIFIER("result"), T_ASSIGNOP, T_IDENTIFIER("a"),
T_PLUS, T_IDENTIFIER("b"), T_SEMICOLON,
T_RETURN, T_IDENTIFIER("result"), T_SEMICOLON,
T_BRACER, T_EOF
```

AST produced:
```
Program
  FunctionDecl: add
    Parameters:
       a
       b
    Body:
      VarDecl: result
        BinaryExpr: +
          Identifier: a
          Identifier: b
      ReturnStmt
        Identifier: result
```
