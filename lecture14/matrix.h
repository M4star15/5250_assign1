#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void build_blur_matrix(double **A, int n);

double dot(double *a, double *b, int n);
double norm(double *x, int n);

void matvec(double **A, double *x, double *y, int n);

void gaussian_elimination(double **A, double *b, int n);
void back_substitution(double **A, double *b, double *x, int n);

double power_iteration(double **A, int n);
double shifted_inverse_iteration(double **A, int n, double mu);
double rayleigh_iteration(double **A, int n);

#endif