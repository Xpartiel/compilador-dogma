#ifndef TYPE_H
#define TYPE_H

typedef enum {
    T_integer,
    T_float,
    T_double,
    T_boolean,
    T_string,
    T_array,
    T_list,
    T_function,
    T_void
} BaseType;

typedef struct Type {
    BaseType base;
    struct Type *inner;
    struct Type *params;
} Type;

Type *new_type(BaseType base, Type *inner);
void print_type(Type *t);
void free_type( Type *t);

#endif