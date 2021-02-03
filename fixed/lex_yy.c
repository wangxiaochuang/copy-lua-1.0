#include "../opcode.h"

static Input input;
static Unput unput;

void lua_setinput(Input fn) {
    input = fn;
}

void lua_setunput(Unput fn) {
    unput = fn;
}