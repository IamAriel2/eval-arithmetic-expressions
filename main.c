#include "structures.h"
#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *exp_ptr = NULL;   // The global pointer to the expression
int brackets_count = 0; // For validation of the brackets

int main()
{
    // Reading the input expression from the file:
    FILE *input = fopen("input.txt", "r");
    if (input == NULL)
    {
        input = fopen("input.txt", "w");
        if (input == NULL)
        {
            perror("Error: could not create input.txt");
            return 1;
        }
        fputs("1+1", input);
        fclose(input);
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
        perror("Error: opening file for writing");
        return 1;
    }
    fprintf(output, "%s\n", num);
    fclose(output);

    return 0;
}

Pointer *solve_tree(Pointer *p)
{
    // Simple post-order traversal to solve the expression tree.
    if (p->type == 'n')
        return p;
    p->f.o.left = solve_tree(p->f.o.left);
    p->f.o.right = solve_tree(p->f.o.right);
    return execute_operator(p);
}