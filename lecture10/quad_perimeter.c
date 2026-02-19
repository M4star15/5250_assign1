#include <math.h>
#include "quad.h"

// perimeter calc
static double dist(Point p, Point q) {
    return sqrt((p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y));
}

double quad_perimeter(Quadrilateral q) {
    return dist(q.A, q.B) + dist(q.B, q.C) +
           dist(q.C, q.D) + dist(q.D, q.A);
}