#ifndef hash_h
#define hash_h

typedef struct node {
    Object ref;
    Object val;
    struct node *next;
} Node;

typedef struct Hash {
    char            mark;
    unsigned int    nhash;
    Node            **list;
} Hash;

#define markarray(t)        ((t)->mark)

#endif
