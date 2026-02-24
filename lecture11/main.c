#include <stdio.h>
#include <stdlib.h>
#include "poly.h"

int main() {
    Term *p1 = NULL, *p2 = NULL;

    printf("Polynomial 1: enter number of terms: ");
    int n1; scanf("%d", &n1);
    for (int i = 0; i < n1; i++) {
        double c; int e;
        printf("coef exp: ");
        scanf("%lf %d", &c, &e);
        p1 = insert_term(p1, c, e);
    }

    printf("\nPolynomial 2: enter number of terms: ");
    int n2; scanf("%d", &n2);
    for (int i = 0; i < n2; i++) {
        double c; int e;
        printf("coef exp: ");
        scanf("%lf %d", &c, &e);
        p2 = insert_term(p2, c, e);
    }

    printf("\nP1(x) = "); print_poly(p1);
    printf("P2(x) = "); print_poly(p2);

    Term* sum = add_poly(p1, p2);
    printf("\nP1 + P2 = ");
    print_poly(sum);

    double x;
    printf("\nEnter x to evaluate sum: ");
    scanf("%lf", &x);

    printf("Sum evaluated at x=%.2f: %.2f\n", x, eval_poly(sum, x));

    free_poly(p1);
    free_poly(p2);
    free_poly(sum);

    return 0;
}