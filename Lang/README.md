# Frontend

## Description

The goal of this project is create grammar for graphic app, which we worked with in previous tasks, that implement Frontend using ANTLR, with translation into LLVM-IR coming after.

As a base, i took Language, Frontend and Backend of which i implemented during my first year of university, you can find initial description [here](https://github.com/aleksplast/My-language).

Some changes must have been made however, for example: declarations and access to two-dimensional arrays, linking with external functions, like `screen_put_pixel` or `sim_rand`.

Frontend is generated using `ANTLR`, that, using `Visitor` generated for us, AST is translated into LLVM-IR.

In `examples` directory you can find examples for some grammar subsets of my language, as well as version of `graphic-app`, written in latin (`app.stass`) and cyrillic(`app-rus.stass`).

## Usage

With me living on Ubuntu, using `ANTLR` is a literall hell. Big thanks to my [comrade](https://github.com/Vladislave0-0/LLVM-course/tree/main/05_LANG2IR) for helping me in this non-trivial task. Follow instructions on the given link to install `ANTLR` and proceed to the next step.

To use `lang2ir` you need:

```
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release
cmake --build ./build
./build/lang2ir ./examples/app-rus.stass
```

## Grammar

Here is parser rules of grammar of my language (full version with lexer rules in `grammar/SilverLang.g4`):
```
programm: statements;
statement: (arrdecl | vardecl | funcdecl | assignment | ifstatement | whilestatement | retstatement | call STRING_TERMINAL);
arrdecl: ARR NAME SBRA INT SKET SBRA INT SKET STRING_TERMINAL;
call: NAME BRA expr* KET;
retstatement: RET expr STRING_TERMINAL;
statements: statement*;
arraccess: NAME SBRA expr SKET SBRA expr SKET;
elseifstatement: ELSEIF BRA expr KET CBRA statements CKET elseifstatement?;
ifstatement: IF_ BRA expr KET CBRA statements CKET elseifstatement?;
whilestatement: WHILE BRA expr KET CBRA statements CKET;
funcdecl: FUNC NAME BRA NAME* KET CBRA statements CKET;
vardecl: VARIABLE NAME EQUAL expr str=STRING_TERMINAL;
assignment: (arraccess | NAME) EQUAL expr str=STRING_TERMINAL;
expr:
    '-' expr
    | expr (EQ | NE | LE | LT | GE | GT) expr
    | expr (MUL | DIV | REM) expr
    | expr (SUB | ADD) expr
    | BRA expr KET
    | arraccess
    | INT
    | NAME
    | call;
```

As you can see, it supports:
1. Function declarations
2. Variable declarations
3. Two-dimensional arrays declarations
4. Assignment
5. IF statements
6. ELSE IF statements
6. While loops
7. Return statements
8. Function calls
9. Two-dimensional array access
10. Arithmethic expressions

