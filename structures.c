#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

extern char *exp_ptr;

Pointer *create_number(double real, double img)
{
    Pointer *p = malloc(sizeof(Pointer));
    p->type = 'n';
    p->f.n.real = real;
    p->f.n.img = img;
    return p;
}

Pointer *create_operator(char type, Pointer *left, Pointer *right)
{
    Pointer *p = malloc(sizeof(Pointer));
    p->type = 'o';
    p->f.o.type = type;
    p->f.o.left = left;
    p->f.o.right = right;
    return p;
}

void scan_tree(Pointer *p)
{
    if (p->type == 'n')
        print_number(p);
    else
    {
        printf("(");
        scan_tree(p->f.o.left);
        printf("%c", p->f.o.type);
        scan_tree(p->f.o.right);
        printf(")");
    }
}

char is_a_number(char c)
{
    return ((c >= '0' && c <= '9') || c == '.');
}

char is_an_opr(char c)
{
    if (c == '+' || c == '-')
    {
        return 0;
    }
    else if (c == '*' || c == '/')
    {
        return 1;
    }
    else if (c == '^')
    {
        return 2;
    }
    return -1;
}

Pointer *read_number()
{
    if (*exp_ptr == 'i')
    {
        exp_ptr++;
        return create_number(0, 1);
    }
    else if (is_a_number(*exp_ptr))
    {
        double number = strtod(exp_ptr, &exp_ptr);
        return create_number(number, 0);
    }
    return NULL;
}

char *print_number(Pointer *num)
{
    static char number[MAX];
    double real = num->f.n.real;
    double img = num->f.n.img;

    if (img == 0)
        sprintf(number, "%.3f", real);
    else if (real == 0)
        sprintf(number, "%.3fi", img);
    else
    {
        if (img > 0)
            sprintf(number, "%.3f + %.3fi", real, img);
        else
            sprintf(number, "%.3f - %.3fi", real, -img);
    }

    return number;
}

Pointer *read_operator()
{
    if (is_an_opr(*exp_ptr) != -1)
        return create_operator(*exp_ptr++, NULL, NULL);
    else
        return NULL;
}

Pointer *execute_operator(Pointer *opr)
{
    double left_real = opr->f.o.left->f.n.real;
    double left_img = opr->f.o.left->f.n.img;
    double right_real = opr->f.o.right->f.n.real;
    double right_img = opr->f.o.right->f.n.img;
    free(opr->f.o.left);
    free(opr->f.o.right);
    opr->type = 'n';

    switch (opr->f.o.type)
    {
    case '+':
        opr->f.n.real = left_real + right_real;
        opr->f.n.img = left_img + right_img;
        break;
    case '-':
        opr->f.n.real = left_real - right_real;
        opr->f.n.img = left_img - right_img;
        break;
    case '*':
        opr->f.n.real = left_real * right_real - left_img * right_img;
        opr->f.n.img = left_real * right_img + left_img * right_real;
        break;
    case '/':
        if (right_real == 0 && right_img == 0)
            return NULL;
        double denom = right_real * right_real + right_img * right_img;
        opr->f.n.real = (left_real * right_real + left_img * right_img) / denom;
        opr->f.n.img = (left_img * right_real - left_real * right_img) / denom;
        break;
    case '^':
        double r = sqrt(left_real * left_real + left_img * left_img);
        double theta = atan2(left_img, left_real);
        double rn = pow(r, right_real);
        double angle = right_real * theta;
        opr->f.n.real = rn * cos(angle);
        opr->f.n.img = rn * sin(angle);
        break;
    }
}