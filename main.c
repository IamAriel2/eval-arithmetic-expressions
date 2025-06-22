#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *exp_ptr = NULL;   // The global pointer to the expression
int brackets_count = 0; // For validation of the brackets

int main()
{
    // Reading the input expression from the file:
    FILE *input = fopen("input.txt", "r");
    if (input == NULL)
    {
        perror("Error: opening file");
        return 1;
    }
    char exp[MAX];
    if (fgets(exp, sizeof(exp), input) == NULL)
    {
        perror("Error: reading file");
        fclose(input);
        return 1;
    }
    fclose(input);
    // Calculating the result of the expression:
    exp_ptr = exp;
    Pointer *result = brackets_handler();
    FILE *output = fopen("output.txt", "w");
    if (result == NULL || brackets_count != 0)
    {
        fputs("Error: invalid expression\n", output);
        fclose(output);
        return 1;
    }
    char *num = print_number(result);
    free(result);
    // Writing the result to the output file:
    if (output == NULL)
    {
        perror("Error: opening file");
        return 1;
    }
    fprintf(output, "%s\n", num);
    fclose(output);

    return 0;
}

Pointer *brackets_handler()
{
    Pointer *number = create_number(0, 0);
    Pointer *root = create_operator('+', NULL, number);
    Pointer *oprs[4] = {root, NULL, NULL, NULL};
    create_tree(oprs);
    if (*exp_ptr == ')')
    {
        exp_ptr++;
        brackets_count--;
    }
    if (oprs[0] == NULL) // If there was an error in creating the tree, we return NULL.
        return NULL;
    solve_tree(root);
    return root;
}

// Creating a tree ignoring the brackets inside.
// If there are brackets, we let the function brackets_handler handle it.
void create_tree(Pointer *oprs[4])
{
    // Reading the number:
    Pointer *number;
    if (*exp_ptr == ' ')
        exp_ptr++;
    else if (*exp_ptr == '(')
    {
        exp_ptr++;
        brackets_count++;
        number = brackets_handler();
    }
    else
        number = read_number();

    if (number == NULL)
    {
        // Handling the case where the expressions starts with a negative number.
        // If the first character is a minus sign, we multiply the number by -1.
        if (*exp_ptr == '-')
        {
            number = create_number(-1, 0);
            *exp_ptr = '*';
        }
        else
        {
            oprs[0] = NULL;
            return;
        }
    }
    // Reading the operator:
    if (*exp_ptr == '\0' || *exp_ptr == ')')
    {
        oprs[0]->f.o.left = number;
        return;
    }
    Pointer *operator = read_operator();
    if (operator == NULL)
    {
        oprs[0] = NULL;
        return;
    }
    operator->f.o.left = number;

    create_tree(oprs);
    if (oprs[0] == 0) // Meaning that there is an error.
        return;
    // Linking the current operator to the tree.
    // Finding the right place for the operator in the tree.
    // The operator is placed in the tree according to its precedence.
    char pr = is_an_opr(operator->f.o.type), i;
    for (i = pr; i >= 0 && oprs[i] == NULL; i--)
        ;
    operator->f.o.right = oprs[i]->f.o.left;
    oprs[i]->f.o.left = operator;
    oprs[pr] = operator;
    for (i = pr + 1; i < 4; i++)
        oprs[i] = NULL;
}

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

Pointer *solve_tree(Pointer *p)
{
    if (p->type == 'n')
        return p;
    p->f.o.left = solve_tree(p->f.o.left);
    p->f.o.right = solve_tree(p->f.o.right);
    return execute_operator(p);
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
    static char number[50];
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