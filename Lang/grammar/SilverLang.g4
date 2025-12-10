grammar SilverLang;

// Parser rules
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


ARR: 'vysadka' | 'высадка';
RET: 'vylet' | 'вылет';
EQ: 'top1' | 'топ1';
NE: 'top2' | 'топ2';
LE: 'top3' | 'топ3';
LT: 'top4' | 'топ4';
GE: 'top5' | 'топ5';
GT: 'top6' | 'топ6';
MUL: 'raskrutit' | 'раскрутить';
DIV: 'klif' | 'клиф';
SUB: 'damag' | 'дамаг';
ADD: 'baff' | 'бафф';
REM: 'braza' | 'браза';
WHILE: 'regat' | 'регать';
ELSEIF: 'vinluz' | 'винлуз';
IF_: 'vin' | 'вин';
FUNC: 'katka' | 'катка';
VARIABLE: 'karta' | 'карта';
EQUAL: 'stati' | 'статы';
SBRA: 'parni' | 'bonk' | 'парни'| 'бонк';
SKET: 'goyda' | 'ravanda' | 'гойда' | 'раванда';
CBRA: 'zdarova' | 'здарова';
CKET: 'poka' | 'пока';
BRA: 'podrubai' | 'подрубай';
KET: 'ofai' | 'офай';
STRING_TERMINAL: 'chat' | 'krasivo' | 'voprosi' | 'чат' | 'красиво' | 'вопросы';

NAME: [a-zA-Zа-яА-Я_]+;
INT: [0-9]+;
WS: [ \t\r\n]+ -> skip;
