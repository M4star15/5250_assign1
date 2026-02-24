#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "poly.h"

/* Insert term sorted by exponent (descending) */
Term* insert_term(Term* head, double coef, int exp) {
    if (coef == 0) return head;

    // Case 1: list empty OR this exponent is largest
    if (!head || exp > head->exp) {
        Term* t = malloc(sizeof(Term));
        t->coef = coef;
        t->exp = exp;
        t->next = head;
        return t;
    }

    Term* cur = head;
    Term* prev = NULL;

    // Walk the list to find where this exponent fits
    while (cur && cur->exp > exp) {
        prev = cur;
        cur = cur->next;
    }

    // Case 2: exponent already exists → merge coefficients
    if (cur && cur->exp == exp) {
        cur->coef += coef;

        // If coefficient becomes 0, delete term
        if (cur->coef == 0) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            free(cur);
        }
        return head;
    }

    // Case 3: must insert between prev and cur
    Term* t = malloc(sizeof(Term));
    t->coef = coef;
    t->exp = exp;
    t->next = cur;

    if (prev) prev->next = t;
    else head = t;

    return head;
}

/* Print polynomial */
void print_poly(Term* head) {
    if (!head) { printf("0\n"); return; }

    Term* t = head;
    while (t) {
        printf("%.2fx^%d", t->coef, t->exp);
        if (t->next) printf(" + ");
        t = t->next;
    }
    printf("\n");
}

/* Evaluate polynomial at x */
double eval_poly(Term* head, double x) {
    double sum = 0.0;
    Term* t = head;
    while (t) {
        sum += t->coef * pow(x, t->exp);
        t = t->next;
    }
    return sum;
}

/* Free memory */
void free_poly(Term* head) {
    Term* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}