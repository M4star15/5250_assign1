// quad_order.c
#include <stdio.h>
#include <math.h>
#include "quad.h"
#include "quad_order.h"

//orientation test
static int orientation(Point a, Point b, Point c) {
    double v = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (fabs(v) < 1e-9) return 0;     // collinear
    return (v > 0) ? 1 : 2;          // 1 = clockwise, 2 = counterclockwise
}

// check segment intersection 
static int segments_intersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    return (o1 != o2 && o3 != o4);
}

// detect self-crossing quadrilateral
int is_self_crossing(Quadrilateral q) {
    if (segments_intersect(q.A, q.B, q.C, q.D)) return 1;
    if (segments_intersect(q.B, q.C, q.D, q.A)) return 1;
    return 0;
}

// sort vertices counter-clockwise 
Quadrilateral sort_quad_ccw(Quadrilateral q) {
    Point pts[4] = { q.A, q.B, q.C, q.D };

    // centroid
    double cx = (q.A.x + q.B.x + q.C.x + q.D.x) / 4.0;
    double cy = (q.A.y + q.B.y + q.C.y + q.D.y) / 4.0;

    // compute angles
    double ang[4];
    for (int i = 0; i < 4; i++)
        ang[i] = atan2(pts[i].y - cy, pts[i].x - cx);

    // simple bubble sort by angle
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (ang[j] > ang[j+1]) {
                double tmp = ang[j];
                ang[j] = ang[j+1];
                ang[j+1] = tmp;

                Point tp = pts[j];
                pts[j] = pts[j+1];
                pts[j+1] = tp;
            }

    Quadrilateral out = { pts[0], pts[1], pts[2], pts[3] };
    return out;
}

// min ordering checker...
Quadrilateral ensure_valid_order(Quadrilateral q) {
    if (!is_self_crossing(q)) {
        return q;  // already fine
    }

    printf("\n!! WARNING: Quadrilateral is self-crossing (invalid order).\n");
    printf("Fix automatically? (y/n): ");

    char c;
    scanf(" %c", &c);

    if (c == 'y' || c == 'Y') {
        Quadrilateral fixed = sort_quad_ccw(q);
        printf("Order corrected automatically.\n");
        return fixed;
    }

    printf("Proceeding with original (invalid) points.\n");
    return q;
}