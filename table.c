#include <stdlib.h>
#include <string.h>

#include "opcode.h"
#include "hash.h"
#include "inout.h"
#include "table.h"
#include "lua.h"

#define streq(s1, s2)    (strcmp(s1, s2)==0)

#ifndef MAXSYMBOL
#define MAXSYMBOL   512
#endif

static Symbol tablebuffer[MAXSYMBOL] {
    { "type", { T_CFUNCTION, { lua_type } } },
    { "tonumber", { T_CFUNCTION, { lua_obj2number } } },
    { "next", { T_CFUNCTION, { lua_next } } },
    { "nextvar", { T_CFUNCTION, { lua_nextvar } } },
    { "print", { T_CFUNCTION, { lua_print } } },
}
Symbol              *lua_table = tablebuffer;
Word                lua_ntable=5;

#ifndef MAXCONSTANT
#define MAXCONSTANT 256
#endif
static char     *constantbuffer[MAXCONSTANT] = {
    "mark", "nil", "number", "string", "table", "function", "cfunction"
}
char            **lua_constant = constantbuffer;
Word            lua_nconstant = T_CFUNCTION + 1;

#ifndef MAXSTRING
#define MAXSTRING 512
#endif
static char     *stringbuffer[MAXSTRING];
char            **lua_string = stringbuffer;
Word            lua_nstring = 0;

#ifndef MAXARRAY
#define MAXARRAY    512
#endif
static Hash     *arraybuffer[MAXARRAY];
Hash            **lua_array = arraybuffer;
Word            lua_narray = 0;

int lua_findsymbol(char *s) {
    int i;
    for (i = 0; i < lua_ntable; i++)
        if (streq(s, s_name(i)))
            return i;
    if (lua_ntable >= MAXSYMBOL - 1) {
        lua_error("symbol table overflow");
        return -1;
    }
    s_name(lua_ntable) = strdup(s);
    if (s_name(lua_ntable) == NULL) {
        lua_error("not enough memory");
        return -1;
    }
    s_tag(lua_ntable++) = T_NIL;
    return (lua_ntable - 1);
}

void lua_markobject(Object *o) {
    if (tag(o) == T_STRING)
        lua_markstring(svalue(o)) = 1;
    else if (tag(o) == T_ARRAY && markarray(avalue(o)) == 0)
        lua_hashmark(avalue(o));
}

static void lua_marktable(void) {
    int i;
    for (i = 0; i < lua_table; i++)
        lua_markobject(&s_object(i));
}

static void lua_pack(void) {
    lua_markstack();
    lua_marktable();
    {
        int i, j;
        for (i = j = 0; i < lua_nstring; i++)
            if (lua_markstring(lua_string[i]) == 1) {
                lua_string[j++] = lua_string[i];
                lua_markstring(lua_string[i]) = 0;
            } else {
                free(lua_string[i] - 1);
            }
        lua_nstring = j;
    }
    {
        int i, j;
        for (i = j = 0; i < lua_narray; i++)
            if (markarray(lua_array[i]) == 1) {
                lua_array[j++] = lua_array[i];
                markarray(lua_array[i]) = 0;
            } else {
                lua_hashdelete(lua_array[i]);
            }
        lua_narray = j
    }
}
char *lua_createstring(char *s) {
    if (s == NULL) return NULL;
    if (lua_nstring >= MAXSTRING - 1) {
        lua_pack();
        if (lua_nstring >= MAXSTRING - 1) {
            lua_error("string table overflow");
            return NULL;
        }
    }
    lua_string[lua_nstring++] = s;
    return s;
}
