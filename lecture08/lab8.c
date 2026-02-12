#include <stdio.h>
#include <math.h>
// cd 5250/lecture08
//  gcc lab8.c -o lab8 -lm
//  ./lab8



// recursive factorial
// long long or int breaks for bigger numbers so double
// recursive factorial function
double factorial(int n)
{
    if (n <= 1)
    {
        // base case
        return 1.0;
    }
    else
    {
        // return n*(n-1)!
        return n * factorial(n - 1);
    }
}



// exp(x) using Taylor expansion around x0 = round(x)
// e = 2.718281828459... as given
// exp(x) = e^x0 * (summation of) ( (x - x0)^k / k! ) for formula

double myexp(double x) {
    const double e_const = 2.718281828459;

    int x0 = (int)round(x);       // this will be the closest integer
    double dx = x - x0;           // expansion point shift

    double sum = 0.0;
    int terms = 20;               // doing 20 for here

    for (int k = 0; k < terms; k++) {
        sum += pow(dx, k) / factorial(k);
    }

    // multiply by e^x0
    return pow(e_const, x0) * sum;
}


// main part
int main() {
    FILE *outfile = fopen("exp_output.data", "w");

    if (!outfile) {
        printf("Could not open output file.\n");
        return 1;
    }

    for (int i = 0; i <= 50; i++) {      // 0, 0.02, ..., 1.00 generating set of points here
        double x = i * 0.02;
        double y = myexp(x);

        fprintf(outfile, "%0.6f %23.16e\n", x, y);
    }

    fclose(outfile);

    printf("Data written to exp_output.data\n");
    return 0;
}
