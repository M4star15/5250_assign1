#ifndef POLY_H
#define POLY_H

typedef struct Term {
    double coef;
    int exp;
    struct Term* next;
} Term;

/* Core functions */
Term* insert_term(Term* head, double coef, int exp);
void print_poly(Term* head);
double eval_poly(Term* head, double x);

/* Addition */
Term* add_poly(Term* p1, Term* p2);

/* Cleanup */
void free_poly(Term* head);

#endif