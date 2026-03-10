#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "trimatrix.h"

// from sample code
void Hessenberg(const matrix* Ain, trimatrix* T);
void QRA(trimatrix* T);

static void build_blur_matrix(matrix* A)
{
    int N = A->rows;

    for (int i = 1; i <= N; i++)
    {
        mgetp(A, i, i) = 2.0 / 4.0;

        if (i > 1)
            mgetp(A, i, i - 1) = 1.0 / 4.0;

        if (i < N)
            mgetp(A, i, i + 1) = 1.0 / 4.0;
    }
}

static int cmp_desc(const void* a, const void* b)
{
    double x = *(const double*)a;
    double y = *(const double*)b;

    if (x < y) return 1;
    if (x > y) return -1;
    return 0;
}

static void run_case(int N)
{
    matrix A = new_matrix(N, N);
    trimatrix T = new_trimatrix(N);
    double* eig = (double*)malloc((size_t)N * sizeof(double));

    build_blur_matrix(&A);

   //full symmetric matrix -> tridiagonal -> QR algorithm
    Hessenberg(&A, &T);
    QRA(&T);

    for (int i = 1; i <= N; i++)
        eig[i - 1] = tget(&T, i, i);

    // sort eigenvalues for better output
    qsort(eig, (size_t)N, sizeof(double), cmp_desc);

    printf("\n========================\n");
    printf("N = %d\n", N);
    printf("========================\n");
    printf("Eigenvalues (descending):\n");

    for (int i = 0; i < N; i++)
        printf("lambda_%d = %.12f\n", i + 1, eig[i]);

    free(eig);
    delete_trimatrix(&T);
    delete_matrix(&A);
}

int main(void)
{
    //running QR alogirthm for N = 6, 12 for prev matrix
    run_case(6);
    run_case(12);
    return 0;
}