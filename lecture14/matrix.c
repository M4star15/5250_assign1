#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"

void build_blur_matrix(double **A, int n)
{
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            A[i][j] = 0.0;

    for(int i=0;i<n;i++)
    {
        A[i][i] = 2.0/4.0;

        if(i>0)
            A[i][i-1] = 1.0/4.0;

        if(i<n-1)
            A[i][i+1] = 1.0/4.0;
    }
}

double dot(double *a, double *b, int n)
{
    double s = 0;
    for(int i=0;i<n;i++)
        s += a[i]*b[i];
    return s;
}

double norm(double *x, int n)
{
    return sqrt(dot(x,x,n));
}

void matvec(double **A, double *x, double *y, int n)
{
    for(int i=0;i<n;i++)
    {
        y[i] = 0;
        for(int j=0;j<n;j++)
            y[i] += A[i][j]*x[j];
    }
}

void gaussian_elimination(double **A, double *b, int n)
{
    for(int k=0;k<n-1;k++)
    {
        for(int i=k+1;i<n;i++)
        {
            double factor = A[i][k]/A[k][k];

            for(int j=k;j<n;j++)
                A[i][j] -= factor*A[k][j];

            b[i] -= factor*b[k];
        }
    }
}

void back_substitution(double **A, double *b, double *x, int n)
{
    for(int i=n-1;i>=0;i--)
    {
        x[i] = b[i];

        for(int j=i+1;j<n;j++)
            x[i] -= A[i][j]*x[j];

        x[i] /= A[i][i];
    }
}

double power_iteration(double **A, int n)
{
    double *x = malloc(n*sizeof(double));
    double *y = malloc(n*sizeof(double));

    for(int i=0;i<n;i++)
        x[i] = 1.0;

    double lambda = 0;

    for(int iter=0; iter<100; iter++)
    {
        matvec(A,x,y,n);

        lambda = dot(x,y,n)/dot(x,x,n);

        double nrm = norm(y,n);

        for(int i=0;i<n;i++)
            x[i] = y[i]/nrm;
    }

    free(x);
    free(y);

    return lambda;
}

double shifted_inverse_iteration(double **A, int n, double mu)
{
    double **B = malloc(n*sizeof(double*));
    double *x = malloc(n*sizeof(double));
    double *b = malloc(n*sizeof(double));
    double *y = malloc(n*sizeof(double));

    for(int i=0;i<n;i++)
    {
        B[i] = malloc(n*sizeof(double));

        for(int j=0;j<n;j++)
            B[i][j] = A[i][j];

        B[i][i] -= mu;
    }

    for(int i=0;i<n;i++)
        x[i] = 1;

    for(int iter=0; iter<20; iter++)
    {
        for(int i=0;i<n;i++)
            b[i] = x[i];

        double **C = malloc(n*sizeof(double*));
        for(int i=0;i<n;i++)
        {
            C[i] = malloc(n*sizeof(double));
            for(int j=0;j<n;j++)
                C[i][j] = B[i][j];
        }

        gaussian_elimination(C,b,n);
        back_substitution(C,b,y,n);

        double nrm = norm(y,n);
        for(int i=0;i<n;i++)
            x[i] = y[i]/nrm;

        for(int i=0;i<n;i++)
            free(C[i]);
        free(C);
    }

    matvec(A,x,y,n);

    double lambda = dot(x,y,n);

    for(int i=0;i<n;i++)
        free(B[i]);

    free(B);
    free(x);
    free(b);
    free(y);

    return lambda;
}

double rayleigh_iteration(double **A, int n)
{
    double *x = malloc(n*sizeof(double));
    double *y = malloc(n*sizeof(double));

    for(int i=0;i<n;i++)
        x[i] = 1;

    double mu = 0;

    for(int iter=0; iter<10; iter++)
    {
        matvec(A,x,y,n);

        mu = dot(x,y,n)/dot(x,x,n);

        double **B = malloc(n*sizeof(double*));
        double *b = malloc(n*sizeof(double));

        for(int i=0;i<n;i++)
        {
            B[i] = malloc(n*sizeof(double));
            for(int j=0;j<n;j++)
                B[i][j] = A[i][j];

            B[i][i] -= mu;
            b[i] = x[i];
        }

        gaussian_elimination(B,b,n);
        back_substitution(B,b,y,n);

        double nrm = norm(y,n);

        for(int i=0;i<n;i++)
            x[i] = y[i]/nrm;

        for(int i=0;i<n;i++)
            free(B[i]);
        free(B);
        free(b);
    }

    matvec(A,x,y,n);

    double lambda = dot(x,y,n);

    free(x);
    free(y);

    return lambda;
}