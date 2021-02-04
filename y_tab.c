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
# define NIL 257
# define IF 258
# define THEN 259
# define ELSE 260
# define ELSEIF 261
# define WHILE 262
# define DO 263
# define REPEAT 264
# define UNTIL 265
# define END 266
# define RETURN 267
# define LOCAL 268
# define NUMBER 269
# define FUNCTION 270
# define NAME 271
# define STRING 272
# define DEBUG 273
# define NOT 274
# define AND 275
# define OR 276
# define NE 277
# define LE 278
# define GE 279
# define CONC 280
# define UNARY 281

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;

static YYSTYPE *yyv;    /* value stack */
static int *yys;        /* state stack */

static YYSTYPE *yypv;    /* top of value stack */
static int *yyps;        /* top of state stack */

static int yystate;     /* current state */
static int yytmp;

int yynerrs;

int yyerrflag;          /* error recovery flag */
int yychar;             /* current input token number */

int lua_parse(void) {
    Byte *initcode = maincode;
    err = 0;
    if (yyparse() || (err == 1)) return 1;
    *maincode++ = HALT;
    if (lua_execute(initcode)) return 1;
    maincode = initcode;
    return 0;
}

int yypact[] = {
 -1000,  -258, -1000, -1000, -1000,  -234, -1000,    34,  -254, -1000,
 -1000, -1000, -1000,    43, -1000, -1000,    40, -1000,  -236, -1000,
 -1000, -1000,    93,    -9, -1000,    43,    43,    43,    92, -1000,
 -1000, -1000, -1000, -1000,    43,    43, -1000,    43,  -240,    62,
    31,   -13,    48,    83,  -242, -1000,    43,    43,    43,    43,
    43,    43,    43,    43,    43,    43,    43, -1000, -1000,    90,
    13, -1000, -1000,  -248,    43,    19,   -15,  -216, -1000,    60,
 -1000, -1000,  -249, -1000, -1000,    43,  -250,    43,    89,    61,
 -1000, -1000,    -3,    -3,    -3,    -3,    -3,    -3,    53,    53,
 -1000, -1000,    82, -1000, -1000, -1000,    -2, -1000,    85,    13,
 -1000,    43, -1000, -1000,    31,    43,   -36, -1000,    56,    60,
 -1000,  -255, -1000,    43,    43, -1000,  -269, -1000, -1000, -1000,
    13,    34, -1000,    43, -1000,    13, -1000, -1000, -1000, -1000,
  -193,    19,    19,   -53,    59, -1000, -1000,    -8,    58,    43,
 -1000, -1000, -1000, -1000,  -226, -1000,  -218,  -223, -1000,    43,
 -1000,  -269,    26, -1000, -1000, -1000,    13,  -253,    43, -1000,
 -1000, -1000,   -42, -1000,    43,    43, -1000,    34, -1000,    13,
 -1000, -1000, -1000, -1000,  -193, -1000 };

#define YYFLAG		(-1000)

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
    yypv = &yyv[-1];
    yyps = &yys[-1];
    yystate = 0;
    yytmp = 0;
    yynerrs = 0;
    yyerrflag = 0;
    yychar = -1;
    goto yystack;
    {
        register YYSTYPE *yy_pv;
        register int *yy_ps;
        register int yy_state;
        register int yy_n;
    yynewstate:
        yy_pv = yypv;
        yy_ps = yyps;
        yy_state = yystate;
        goto yy_newstate;
    yystack:
        yy_pv = yypv;
        yy_ps = yyps;
        yy_state = yystate;
    yy_stack:
        if (++yy_ps >= &yys[yymaxdepth]) {
            ...
        }
        *++yy_ps = yy_state;
        *++yy_pv = yyval;
    yy_newstate:
        if ((yy_n = yypact[yy_state]) <= YYFLAG)
            goto yydefault;
        if ((yychar < 0) && ((yychar = yylex()) < 0))
            yychar = 0;
    }
}