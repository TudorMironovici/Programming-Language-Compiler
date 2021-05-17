# What I Learned
- Deeper understanding of how code works behind the scenes
- How to handle various functions that are intertwined and call each other to analyze, parse and interpret a language
- How to Test code thoroughly  <br />





# Compiler
This compiler has three main parts to it: <br />
- Lexical Analyzer <br />
- Parser <br />
- Interpreter <br />

And it follows the following grammar rules: <br />
Prog := Sl <br />
Sl := SC { Sl } | Stmt SC { Sl } <br />
Stmt := PrintStmt | PrintlnStmt | RepeatStmt | Expr <br />
PrintStmt := PRINT Expr <br />
PrintlnStmt := PRINTLN Expr <br />
RepeatStmt:= Repeat Expr BEGIN Stmt END <br />
Expr := Sum { EQ Sum } <br />
Sum := Prod { (PLUS | MINUS) Prod } <br />
Prod := Primary { (STAR | SLASH) Primary } <br />
Primary := IDENT | ICONST | SCONST | LPAREN Expr RPAREN
