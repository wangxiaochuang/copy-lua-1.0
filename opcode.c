#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __GNUC__
#include <floatingpoint.h>
#endif
#include "opcode.h"

#define tonumber(o) ((tag(o) != T_NUMBER) && (lua_tonumber(o) != 0))
#define tostring(o) ((tag(o) != T_STRING) && (lua_tostring(o) != 0))

#ifndef MAXSTACK
#define MAXSTACK 256
#endif
static Object stack[MAXSTACK] = { { T_MARK, { NULL } } };
static Object *top = stack + 1, *base = stack + 1;

char *lua_strdup(char *l) {
    char *s = calloc(strlen(l) + 2, sizeof(char));
    if (s == NULL) {
        lua_error("not enough memory");
        return NULL;
    }
    *s++ = 0;
    return strcpy(s, l);
}

static int lua_tonumber(Object *obj) {
    char *ptr;
    if (tag(obj) != T_STRING) {
        lua_reportbug("unexpected type at conversion to number");
        return 1;
    }
    nvalue(obj) = strtod(svalue(obj), &ptr);
    if (*ptr) {
        lua_reportbug("string to number convertion failed");
        return 2;
    }
    tag(obj) = T_NUMBER;
    return 0;
}

static int lua_tostring(Object *obj) {
    static char s[256];
    if (tag(obj) != T_NUMBER) {
        lua_reportbug("unexpected type at conversion to string");
        return 1;
    }
    if ((int) nvalue(obj) == nvalue(obj)) {
        sprintf(s, "%d", (int) nvalue(obj));
    } else {
        sprintf(s, "%g", nvalue(obj));
    }
    svalue(obj) = lua_createstring(lua_strdup(s));
    if (svalue(obj) == NULL)
        return 1;
    tag(obj) = T_STRING;
    return 0;
}

int lua_execute(Byte *pc) {
    while (1) {
        switch ((OpCode) *pc++) {
            case NOP: break;
            case PUSHNIL:
                tag(top++) = T_NIL; break;
            case PUSH0:
                tag(top) = T_NUMBER; nvalue(top++) = 0; break;
            case PUSH1:
                tag(top) = T_NUMBER; nvalue(top++) = 1; break;
            case PUSH2:
                tag(top) = T_NUMBER; nvalue(top++) = 2; break;
            case PUSHBYTE:
                tag(top) = T_NUMBER; nvalue(top++) = *pc++; break;
            case PUSHWORD:
                tag(top) = T_NUMBER; nvalue(top++) = *((Word *) (pc)); pc += sizeof(Word); break;
            case PUSHFLOAT:
                tag(top) = T_NUMBER; nvalue(top++) = *((float*) (pc)); pc += sizeof(float); break;
            case PUSHSTRING:
                {
                    int w = *((Word *) (pc));
                    pc += sizeof(Word);
                    tag(top) = T_STRING; svalue(top++) = lua_constant[w];
                }
                break;
        }
    }
}

void lua_markstack(void) {
    Object *o;
    for (o = top - 1; o >= stack; o--) {
        lua_markobject(o);
    }
}

int lua_call(char *functionname, int nparam) {
    static Byte startcode[] = { CALLFUNC, HALT }
    int i;
    Object func = s_object(lua_findsymbol(functionname));
    if (tag(&func) != T_FUNCTION) return 1;
    // PARAM2 => PARAM1 => func(top) => T_MARK => PARAM2 => PARAM1
    // nparam - 0: (top)func => T_MARK => top
    // nparam - 1: (PARAM1)func => (top)T_MARK => PARAM1 => top
    // nparam = 2: (PARAM2)func => (PARAM1)T_MARK => (top)PARAM2 => PARAM1 => top
    // nparam - 3: (PARAM3)func => (PARAM2)T_MARK => (PARAM1)PARAM3 => (top)PARAM2 => PARAM1 => top
    for (i = 1; i <= nparam; i++)
        *(top - i + 2) = *(top - i);
    top += 2;
    tag(top - nparam - 1) = T_MARK;
    *(top - nparam - 2) = func;
    return (lua_execute(startcode));
}

Object *lua_getparam(int number) {
    if (number <= 0 || number > top - base) return NULL;
    return (base + number - 1);
}

char *lua_getstring(Object *object) {
    if (tostring(object))
        return NULL;
    else
        return (svalue(object));
}

int lua_isstring(Object *object) {
    return (object != NULL && tag(object) == T_STRING);
}
