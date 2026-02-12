#include "math.h"
//doing without math.h and doing with math.h
//cd 5250

// gcc lab.c -o lab -lm
//      ./lab

#include <stdio.h>

//if else?
//seems like better convention to use <> than "" for system files like stdin

/*Develop a script ("main program") with name lab.c: computing factorial of an
integer n, exponential of a real number x, and logarithm of a real number y*/

//approx using taylor series
// unstable for numbers above 20?
double expo (double x)
{
    double term = 1.0;
    double sum = 1.0;

//did using 500 terms...
    for(int n=1;n<500; n++){
            term *= x/n;
            sum += term; // sum = sum + term
        }
        return sum;

}

long long factorial(int n)
{
    long long f = 1;
    for (int i=1; i<=n; i++)
        {f *=i;} 
    return f;
}

double logi(double y)
{
    if(y<=0)
        {return -1e9;}
    double guess = 1.0;
    for(int i=0;i<500; i++)
    {
        double e = expo(guess);
        guess = guess - (e - y) / e;
    }
    return guess;

}

int main(){
    int n;
    double x, y;
    //variable names...
    printf("Enter integer number n for factorial:");
    scanf("%d", &n);
    
    printf("Enter real number x for exp:");
    scanf("%lf", &x);

    printf("Enter real number y for log:");
    scanf("%lf", &y);

    if (n < 0) {
        printf("\nFactorial undefined for negative n.\n");
    } else {
        long long f_custom = factorial(n);
        double f_math = tgamma(n + 1); // math.h factorial
        printf("\nFactorial(%d):\n", n);
        printf("  custom = %lld\n", f_custom);
        printf("  math.h = %.0f\n", f_math);
        printf("  diff   = %.0f\n", f_custom - f_math);
    }

    double e_custom = expo(x);
    double e_math = exp(x);

    printf("\nexp(%g):\n", x);
    printf("  custom = %.12f\n", e_custom);
    printf("  math.h = %.12f\n", e_math);
    printf("  diff   = %.12f\n", e_custom - e_math);

    if (y <= 0) {
        printf("\nlog(y) undefined for y <= 0.\n");
    } else {
        double l_custom = logi(y);
        double l_math = log(y);

        printf("\nlog(%g):\n", y);
        printf("  custom = %.12f\n", l_custom);
        printf("  math.h = %.12f\n", l_math);
        printf("  diff   = %.12f\n", l_custom - l_math);
    }

    return 0;

}