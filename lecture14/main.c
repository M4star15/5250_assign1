#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "matrix.h"

double **alloc_matrix(int n)
{
    double **A = malloc(n*sizeof(double*));

    for(int i=0;i<n;i++)
        A[i] = malloc(n*sizeof(double));

    return A;
}

void free_matrix(double **A, int n)
{
    for(int i=0;i<n;i++)
        free(A[i]);

    free(A);
}

void run_case(int n)
{
    double **A = alloc_matrix(n);

    build_blur_matrix(A,n);

    printf("\n====================\n");
    printf("n = %d\n",n);
    printf("====================\n");

    double p = power_iteration(A,n);
    printf("Power Iteration eigenvalue: %.6f\n",p);

    double s0 = shifted_inverse_iteration(A,n,0);
    printf("Shifted Inverse (mu=0): %.6f\n",s0);

    double s2 = shifted_inverse_iteration(A,n,2);
    printf("Shifted Inverse (mu=2): %.6f\n",s2);

    double r = rayleigh_iteration(A,n);
    printf("Rayleigh Iteration eigenvalue: %.6f\n",r);

    free_matrix(A,n);
}

int main()
{
    run_case(5);
    run_case(10);

    return 0;
}