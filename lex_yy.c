#include "opcode.h"

static Input input;
static Unput unput;

void lua_setinput(Input fn) {
    input = fn;
}

void lua_setunput(Unput fn) {
    unput = fn;
}

int yylex() {
    int nstr;
    extern int yyprevious;
    while ((nstr = yylook()) >= 0) {
    yyfussy:
        switch (nstr) {
            case 0:
                if (yywrap()) return 0;
                break;
            case 1: break;
            case 2:
                { yylval.vInt = 1; return DEBUG; }
                break;
        }
    }
}