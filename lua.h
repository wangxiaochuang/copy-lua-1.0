#ifndef lua_h
#define lua_h

typedef void (*lua_CFunction) (void);
typedef struct Object *lua_Object;

#define lua_register(n, f) (lua_pushcfunction(f), lua_storeglobal(n))

int            lua_dofile 		(char *filename);
int            lua_dostring 		(char *string);

lua_Object     lua_getparam 		(int number);
char          *lua_getstring 		(lua_Object object);

int            lua_pushobject       	(lua_Object object);

int            lua_storeglobal		(char *name);

#endif
