#include "type.h"
#include <stdio.h>
#include <stdlib.h>

Type *new_type(BaseType base, Type *inner){
    Type *t = malloc(sizeof(Type));
    t->base = base;
    t->inner = inner;
    return t;
}

void print_type(Type *t){
    if (!t) {
        printf("NULL");
        return;
    }

    switch(t->base) {
        case T_integer:
            printf("integer");
            break;
        case T_float:
            printf("float");
            break;
        case T_double:
            printf("double");
            break;
        case T_boolean:
            printf("boolean");
            break;
        case T_string:
            printf("string");
            break;
        case T_array:
            printf("array<");
            print_type(t->inner);
            printf(">");
            break;
        case T_list:
            printf("list<");
            print_type(t->inner);
            printf(">");
            break;
        default:
            printf("TIPO DESCONOCIDO\n");
    }
}

void free_type(Type *t){
    if(!t) return;
    if(t->inner) free_type(t->inner);
    free(t);
}