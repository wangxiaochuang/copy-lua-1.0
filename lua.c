#include <stdio.h>

#include "lua.h"
#include "lualib.h"

void test(void) {
    lua_pushobject(lua_getparam(1));
    lua_call("c", 1);
}

static void callfunc(void) {
    lua_Object obj = lua_getparam(1);
    if (lua_isstring(obj)) lua_call(lua_getstring(obj), 0);
}

static void execstr(void) {
    lua_Object obj = lua_getparam(1);
    if (lua_isstring(obj)) lua_dostring(lua_getstring(obj));
}

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        puts("usage: lua filename [functionnames]");
        return 1;
    }

    lua_register("callfunc", callfunc);
    lua_register("execstr", execstr);
    lua_register("test", test);

    return 0;
}
