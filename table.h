#ifndef table_h
#define table_h

#define lua_markstring(s)   (*((s) - 1))

int   lua_addfile              (char *fn);

#endif
