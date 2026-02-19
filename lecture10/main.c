#include <stdio.h>
#include "quad.h"
#include "quad_order.h"
//  cd ~/5250/lecture10
//  gcc main.c quad_area.c quad_perimeter.c quad_angles.c quad_order.c -o main -lm
//  ./main

int main() {
    Quadrilateral q;
    // 4 coords to be entered
    printf("Enter coordinates Ax Ay: ");
    scanf("%lf %lf", &q.A.x, &q.A.y);

    printf("Enter coordinates Bx By: ");
    scanf("%lf %lf", &q.B.x, &q.B.y);

    printf("Enter coordinates Cx Cy: ");
    scanf("%lf %lf", &q.C.x, &q.C.y);

    printf("Enter coordinates Dx Dy: ");
    scanf("%lf %lf", &q.D.x, &q.D.y);

    // fix order if needed
    q = ensure_valid_order(q);
    double P = quad_perimeter(q);
    double A = quad_area(q);

    double angA, angB, angC, angD;
    quad_angles(q, &angA, &angB, &angC, &angD);

    printf("\nPerimeter = %.4f\n", P);
    printf("Area      = %.4f\n", A);
    printf("Angles (degrees):\n");
    printf("  A = %.2f\n", angA);
    printf("  B = %.2f\n", angB);
    printf("  C = %.2f\n", angC);
    printf("  D = %.2f\n", angD);

    return 0;
}