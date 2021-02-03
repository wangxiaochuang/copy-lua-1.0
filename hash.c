#include <string.h>
#include <stdlib.h>

#include "opcode.h"
#include "hash.h"
#include "inout.h"
#include "table.h"
#include "lua.h"

#define nhash(t)        ((t)->nhash)
#define nodelist(t)     ((t)->list)
#define list(t, i)      ((t)->list[i])

static void freelist(Node *n) {
    while (n) {
        Node *next = n->next;
        free(n);
        n = next;
    }
}

void lua_hashdelete(Hash *h) {
    int i;
    for (i = 0; i < nhash(h); i++)
        freelist(list(h, i));
    free(nodelist(h));
    free(h);
}

void lua_hashmark(Hash *h) {
    int i;
    markarray(h) = 1;
    for (i = 0; i < nhash(h); i++) {
        Node *n;
        for (n = list(h, i); n != NULL; n = n->next) {
            lua_markobject(&n->ref);
            lua_markobject(&n->val);
        }
    }
}
