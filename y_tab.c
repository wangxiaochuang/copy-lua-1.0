#include "opcode.h"
#include "lua.h"

#ifndef MAXCODE
#define MAXCODE 1024
#endif
static long     buffer[MAXCODE];
static Byte     *code = (Byte *) buffer;
static long     mainbuffer[MAXCODE];
static Byte     *maincode = (Byte *) mainbuffer;

static int     err;

typedef union {
    int vInt;
    long vLong;
    float vFloat;
    Word vWord;
    Byte *pByte;
} YYSTYPE;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif

int lua_parse(void) {
    Byte *initcode = maincode;
    err = 0;
    if (yyparse() || (err == 1)) return 1;
    *maincode++ = HALT;
    if (lua_execute(initcode)) return 1;
    maincode = initcode;
    return 0;
}

static YYSTYPE *yyv;    /* value stack */
static int *yys;        /* state stack*/

int yyparse() {
    register YYSTYPE *yypvt;
    unsigned yymaxdepth = YYMAXDEPTH;

    yyv = (YYSTYPE *) malloc(yymaxdepth * sizeof(YYSTYPE));
    yys = (int *) malloc(yymaxdepth * sizeof(int));
    if (!yyv || !yys) {
        yyerror("out of memory");
        return 1;
    }
}