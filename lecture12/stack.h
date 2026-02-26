#ifndef STACK_H
#define STACK_H
// header
typedef struct node {
    double value;
    struct node* next;
} node;

void push(node** top, double x);
int pop(node** top, double* output);
double peek(node* top);
void displayStack(node* top);
void deleteStack(node** top);

#endif