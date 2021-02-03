#include <stdio.h>
#include <string.h>

#include "opcode.h"
#include "hash.h"
#include "inout.h"
#include "table.h"

/* Exported variables */
int lua_linenumber;
int lua_debug;
int lua_debugline;

#ifndef MAXFUNCSTACK
#define MAXFUNCSTACK 32
#endif
static struct { int file; int function; } funcstack[MAXFUNCSTACK];
static int nfuncstack=0;

static FILE *fp;
static char *st;

/**
 * get the next character
 **/
static int stringinput(void) {
    st++;
    return (*(st - 1));
}

/**
 * unget the next character
 **/
static void stringunput(int c) {
    st--;
}

int lua_openstring(char *s) {
    lua_linenumber = 1;
    lua_setinput(stringinput);
    lua_setunput(stringunput);

    st = s;
    {
        char sn[64];
        sprintf(sn, "String: %10.10s...", s);
        if (lua_addfile(sn)) return 1;
    }
    return 0;
}

int lua_pushfunction(int file, int function) {
    if (nfuncstack >= MAXFUNCSTACK) {
        lua_error("function stack overflow");
        return 1;
    }
    funcstack[nfuncstack].file = file;
    funcstack[nfuncstack].function = function;
    nfuncstack++;
    return 0;
}

void lua_popfunction(void) {
    nfuncstack--;
}