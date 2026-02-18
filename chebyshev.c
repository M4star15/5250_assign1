// chebyshev.c
//  cd 5250/
// source venv5250/bin/activate

//  cd 5250/
//  gcc chebyshev.c -o cheb -lm
//  ./cheb


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double cheb_phi(int k, double x)
{
    switch(k)
    {
        case 0: return 1.0;
        case 1: return x;
        case 2: return 2*x*x - 1.0;
        case 3: return 4*x*x*x - 3*x;
        case 4: return 8*pow(x,4) - 8*x*x + 1.0;
        case 5: return 16*pow(x,5) - 20*pow(x,3) + 5*x;
        default:
            printf("Invalid k = %d\n", k);
            exit(1);
    }
}

int main()
{
    int N;
    printf("Input polynomial degree N (0–5): ");
    scanf("%d", &N);

    if (N < 0 || N > 5){
        printf("Error: N must be 0–5.\n");
        exit(1);
    }

    // read coefficients
    double b[6];
    for (int i = 0; i <= N; i++){
        printf("Set b[%d]: ", i);
        scanf("%lf", &b[i]);
    }

    const int NumPts = 101;
    double x[NumPts], y[NumPts];

    for (int i = 0; i < NumPts; i++){
        x[i] = -1.0 + 2.0 * i / (NumPts - 1);
    }

    // evaluate p_N(x)
    for (int i = 0; i < NumPts; i++){
        y[i] = 0.0;
        for (int k = 0; k <= N; k++){
            y[i] += b[k] * cheb_phi(k, x[i]);
        }
    }

    // write to data file
    FILE *fp = fopen("cheb_output.data", "w");
    for (int i = 0; i < NumPts; i++){
        fprintf(fp, "%.12f %.12f\n", x[i], y[i]);
    }
    fclose(fp);

    // call Python script
    system("python3 plot_cheb.py");

    return 0;
}
