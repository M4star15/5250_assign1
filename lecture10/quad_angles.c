#include <math.h>
#include "quad.h"

static double angle(Point p, Point v, Point n) {
    // angle at v formed by pv and nv... to b eused for calc
    double ax = p.x - v.x;
    double ay = p.y - v.y;
    double bx = n.x - v.x;
    double by = n.y - v.y;

    double dot = ax*bx + ay*by;
    double magA = sqrt(ax*ax + ay*ay);
    double magB = sqrt(bx*bx + by*by);

    return acos(dot / (magA * magB)) * 180.0 / M_PI;
}

void quad_angles(Quadrilateral q, double *A, double *B, double *C, double *D) {
    *A = angle(q.D, q.A, q.B);
    *B = angle(q.A, q.B, q.C);
    *C = angle(q.B, q.C, q.D);
    *D = angle(q.C, q.D, q.A);
}