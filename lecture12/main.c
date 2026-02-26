#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"

// stack implementation
void push(node** top, double x)
{
    node* temp = malloc(sizeof(node));
    temp->value = x;
    temp->next = *top;
    *top = temp;
}

// pop
int pop(node** top, double* output)
{
    if (*top == NULL) return 0;

    node* temp = *top;
    *output = temp->value;
    *top = temp->next;
    free(temp);

    return 1;
}

// peek
double peek(node* top)
{
    if (top == NULL) return 0.0;
    return top->value;
}

//
void displayStack(node* top)
{
    if (top == NULL) {
        printf("Stack empty.\n");
        return;
    }
    // show what the proess is
    printf("\nStack (top → bottom):\n");
    node* ptr = top;
    while (ptr != NULL) {
        printf(" %.10f\n", ptr->value);
        ptr = ptr->next;
    }
}

void deleteStack(node** top)
{
    node* temp;
    while (*top != NULL)
    {
        temp = *top;
        *top = temp->next;  //move
        free(temp); //delete stack
    }
}


//simple function same as in the class
double f(double x)
{
    return x*x*x - x - 2.0;
}

// bisection function
double bisection(double a, double b, double tol, int maxIter, node** stack)
{
    double mid;

    for (int i = 0; i < maxIter; i++)
    {
        mid = 0.5 * (a + b);    //mid point calc
        push(stack, mid);   // this is to store midpoint in stack

        if (fabs(f(mid)) < tol)
            return mid;

        if (f(a) * f(mid) < 0)
            b = mid;
        else
            a = mid;
    }

    return mid;
}

// main
int main()
{
    double a, b, tol;
    int maxIter;

    printf("Enter interval a b: ");
    scanf("%lf %lf", &a, &b);

    printf("Enter tolerance: ");
    scanf("%lf", &tol);

    printf("Enter max iterations: ");
    scanf("%d", &maxIter);

    node* stack = NULL;

    double root = bisection(a, b, tol, maxIter, &stack);

    printf("\nApproximate root = %.10f\n", root);
    printf("Top of stack (via peek) = %.10f\n", peek(stack));

    printf("\nMidpoints saved on stack:\n");
    displayStack(stack);

    printf("\nNow popping (LIFO):\n");
    double value;
    while (pop(&stack, &value))
        printf(" popped %.10f\n", value);

    deleteStack(&stack);

    return 0;
}