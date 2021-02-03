#include <stdio.h>

#include "lua.h"
#include "lualib.h"

static void callfunc(void) {
    lua_Object obj = lua_getparam(1);
    if (lua_isstring(obj)) lua_call(lua_getstring(obj), 0);
}

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        puts("usage: lua filename [functionnames]");
        return 1;
    }

    lua_register("callfunc", callfunc);

    return 0;
}
