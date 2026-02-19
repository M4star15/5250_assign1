#include <math.h>
#include "quad.h"

// area calculation using determinant method

double quad_area(Quadrilateral q) {
    double sum1 = q.A.x*q.B.y + q.B.x*q.C.y + q.C.x*q.D.y + q.D.x*q.A.y;    // forward diagonal sum
    double sum2 = q.A.y*q.B.x + q.B.y*q.C.x + q.C.y*q.D.x + q.D.y*q.A.x;    // backward diagonal sum..
    return 0.5 * fabs(sum1 - sum2);
}