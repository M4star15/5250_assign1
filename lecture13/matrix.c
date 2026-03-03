#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"

matrix new_matrix(int rows, int cols) {
    matrix A;
    A.rows = rows;
    A.cols = cols;
    A.val = malloc(rows * cols * sizeof(double));
    for (int i = 0; i < rows * cols; i++)
        A.val[i] = 0.0;
    return A;
}

vector new_vector(int size) {
    vector v;
    v.size = size;
    v.val = malloc(size * sizeof(double));
    for (int i = 0; i < size; i++)
        v.val[i] = 0.0;
    return v;
}

void free_matrix(matrix *A) { free(A->val); }
void free_vector(vector *v) { free(v->val); }

matrix matrix_copy(const matrix *A) {
    matrix C = new_matrix(A->rows, A->cols);
    for (int i = 0; i < A->rows * A->cols; i++)
        C.val[i] = A->val[i];
    return C;
}

vector vector_copy(const vector *v) {
    vector c = new_vector(v->size);
    for (int i = 0; i < v->size; i++)
        c.val[i] = v->val[i];
    return c;
}

matrix matrix_transpose(const matrix *A) {
    matrix T = new_matrix(A->cols, A->rows);
    for (int i = 1; i <= A->rows; i++)
        for (int j = 1; j <= A->cols; j++)
            mget(T,j,i) = mgetp(A,i,j);
    return T;
}

matrix matrix_mult(const matrix *A, const matrix *B) {
    assert(A->cols == B->rows);
    matrix C = new_matrix(A->rows, B->cols);
    for (int i = 1; i <= A->rows; i++)
        for (int j = 1; j <= B->cols; j++)
            for (int k = 1; k <= A->cols; k++)
                mget(C,i,j) += mgetp(A,i,k) * mgetp(B,k,j);
    return C;
}

vector matrix_vector_mult(const matrix *A, const vector *x) {
    assert(A->cols == x->size);
    vector y = new_vector(A->rows);
    for (int i = 1; i <= A->rows; i++)
        for (int j = 1; j <= x->size; j++)
            vget(y,i) += mgetp(A,i,j) * vgetp(x,j);
    return y;
}

double vector_norm(const vector *x) {
    double sum = 0.0;
    for (int i = 1; i <= x->size; i++)
        sum += vgetp(x,i) * vgetp(x,i);
    return sqrt(sum);
}

/* Gaussian elimination with partial pivoting */
vector solve(matrix A, vector b) {

    int n = A.rows;
    vector x = new_vector(n);

    for (int i = 1; i <= n-1; i++) {

        int p = i;
        double maxA = fabs(mget(A,i,i));

        for (int j = i+1; j <= n; j++) {
            if (fabs(mget(A,j,i)) > maxA) {
                maxA = fabs(mget(A,j,i));
                p = j;
            }
        }

        if (maxA < 1e-14) {
            printf("Singular matrix\n");
            exit(1);
        }

        if (p != i) {
            for (int j = 1; j <= n; j++) {
                double tmp = mget(A,i,j);
                mget(A,i,j) = mget(A,p,j);
                mget(A,p,j) = tmp;
            }
            double tmpb = vget(b,i);
            vget(b,i) = vget(b,p);
            vget(b,p) = tmpb;
        }

        for (int j = i+1; j <= n; j++) {
            double m = mget(A,j,i) / mget(A,i,i);
            for (int k = i; k <= n; k++)
                mget(A,j,k) -= m * mget(A,i,k);
            vget(b,j) -= m * vget(b,i);
        }
    }

    for (int i = n; i >= 1; i--) {
        double sum = 0.0;
        for (int j = i+1; j <= n; j++)
            sum += mget(A,i,j) * vget(x,j);
        vget(x,i) = (vget(b,i) - sum) / mget(A,i,i);
    }

    return x;
}