#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    int rows;
    int cols;
    double *val;
} matrix;

typedef struct {
    int size;
    double *val;
} vector;

#define mget(mat,i,j) mat.val[(i-1)*mat.cols+(j-1)]
#define mgetp(mat,i,j) mat->val[(i-1)*mat->cols+(j-1)]
#define vget(vec,i) vec.val[(i-1)]
#define vgetp(vec,i) vec->val[(i-1)]

matrix new_matrix(int rows, int cols);
vector new_vector(int size);

void free_matrix(matrix *A);
void free_vector(vector *v);

matrix matrix_copy(const matrix *A);
vector vector_copy(const vector *v);

matrix matrix_transpose(const matrix *A);
matrix matrix_mult(const matrix *A, const matrix *B);
vector matrix_vector_mult(const matrix *A, const vector *x);

vector solve(matrix A, vector b);

double vector_norm(const vector *x);

#endif