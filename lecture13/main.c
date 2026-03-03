#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "matrix.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double gaussian(double sigma) {
    double u1 = ((double)rand()+1)/((double)RAND_MAX+1);
    double u2 = ((double)rand()+1)/((double)RAND_MAX+1);
    return sigma * sqrt(-2*log(u1)) * cos(2*M_PI*u2);
}

void add_noise(vector *b, double sigma) {
    for (int i = 1; i <= b->size; i++)
        vgetp(b,i) += gaussian(sigma);
}

void build_blur(matrix *A) {
    int n = A->rows;
    for (int i = 1; i <= n; i++) {
        mgetp(A,i,i) = 2.0/4.0;
        if (i > 1) mgetp(A,i,i-1) = 1.0/4.0;
        if (i < n) mgetp(A,i,i+1) = 1.0/4.0;
    }
}

void generate_signal(vector *x) {
    int n = x->size;
    for (int i = 1; i <= n; i++)
        vgetp(x,i) = (i >= n/4 && i <= n/2) ? 1.0 : 0.0;
}

void run_case(int n) {

    double sigmas[3] = {1e-6,1e-4,1e-2};
    double lambdas[3] = {1e-6,1e-4,1e-2};

    printf("\n==============================\n");
    printf("n = %d\n", n);
    printf("==============================\n");

    matrix A = new_matrix(n,n);
    build_blur(&A);

    vector x_true = new_vector(n);
    generate_signal(&x_true);

    vector b_clean = matrix_vector_mult(&A,&x_true);

    for (int s=0;s<3;s++) {

        vector b = vector_copy(&b_clean);
        add_noise(&b,sigmas[s]);

        vector x_rec = solve(matrix_copy(&A),vector_copy(&b));

        vector diff = new_vector(n);
        for (int i=1;i<=n;i++)
            vget(diff,i)=vget(x_rec,i)-vget(x_true,i);

        double err = vector_norm(&diff)/vector_norm(&x_true);

        printf("sigma=%.0e | Unreg=%.3e\n",sigmas[s],err);

        matrix AT = matrix_transpose(&A);
        matrix ATA = matrix_mult(&AT,&A);

        for(int l=0;l<3;l++) {

            matrix ATA_reg = matrix_copy(&ATA);

            for(int i=1;i<=n;i++)
                mget(ATA_reg,i,i)+=lambdas[l];

            vector ATb = matrix_vector_mult(&AT,&b);
            vector x_reg = solve(ATA_reg,ATb);

            for(int i=1;i<=n;i++)
                vget(diff,i)=vget(x_reg,i)-vget(x_true,i);

            double err_reg = vector_norm(&diff)/vector_norm(&x_true);

            printf("sigma=%.0e lambda=%.0e | Reg=%.3e\n",
                    sigmas[s],lambdas[l],err_reg);

            free_vector(&x_reg);
            free_matrix(&ATA_reg);
        }

        free_matrix(&ATA);
        free_matrix(&AT);
        free_vector(&diff);
        free_vector(&x_rec);
        free_vector(&b);
    }

    free_vector(&b_clean);
    free_vector(&x_true);
    free_matrix(&A);
}

int main() {
    srand(time(NULL));
    run_case(64);
    run_case(128);
    return 0;
}