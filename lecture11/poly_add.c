#include <stdio.h>
#include <stdlib.h>
#include "poly.h"

/* Create a new polynomial that is the sum of p1 + p2 */
Term* add_poly(Term* p1, Term* p2) {
    Term* result = NULL;

    Term* a = p1;
    Term* b = p2;

    while (a && b) {
        if (a->exp == b->exp) {
            result = insert_term(result, a->coef + b->coef, a->exp);
            a = a->next;
            b = b->next;
        } else if (a->exp > b->exp) {
            result = insert_term(result, a->coef, a->exp);
            a = a->next;
        } else {
            result = insert_term(result, b->coef, b->exp);
            b = b->next;
        }
    }

    /* Add remaining terms */
    while (a) { result = insert_term(result, a->coef, a->exp); a = a->next; }
    while (b) { result = insert_term(result, b->coef, b->exp); b = b->next; }

    return result;
}