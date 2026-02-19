#ifndef QUAD_H
#define QUAD_H

typedef struct {
    double x, y;
} Point;

typedef struct {
    Point A, B, C, D;
} Quadrilateral;

double quad_perimeter(Quadrilateral q);
double quad_area(Quadrilateral q);
void quad_angles(Quadrilateral q, double *angA, double *angB, double *angC, double *angD);

#endif