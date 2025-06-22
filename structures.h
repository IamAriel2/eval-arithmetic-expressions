#pragma once

#include "structures.h"
#define MAX 100

typedef struct pointer
{
    char type;
    union
    {
        struct
        {
            char type;
            struct pointer *left, *right;
        } o;
        struct
        {
            double real, img;
        } n;
    } f;
} Pointer;

Pointer *create_number(double, double);
Pointer *create_operator(char, Pointer *, Pointer *);
void scan_tree(Pointer *);
char is_a_number(char);
char is_an_opr(char);
Pointer *read_number();
char *print_number(Pointer *);
Pointer *read_operator();
Pointer *execute_operator(Pointer *);
