#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

static void usage(const char *prog_name);
static double Phi(double x);
static double black_scholes_call(double S0, double K, double r, double T, double sigma);
static double lognormal_pdf(double S, double S0, double r, double T, double sigma);
static double integrand(double S, double S0, double K, double r, double T, double sigma);
static double simpson_call_price(double S0, double K, double r, double T, double sigma,
                                 double Smax, long long N, int thread_count);

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage(argv[0]);
    }

    const int thread_count = (int)strtol(argv[1], NULL, 10);
    const long long N = strtoll(argv[2], NULL, 10);

    if (thread_count < 1 || N < 2 || (N % 2 != 0)) {
        usage(argv[0]);
    }

    // Test case from the assignment...
    const double S0 = 100.0;
    const double K = 100.0;
    const double r = 0.05;
    const double T = 1.0;
    const double sigma = 0.2;
    const double Smax = 5.0 * S0;

    const double time1 = omp_get_wtime();

    const double C_num = simpson_call_price(S0, K, r, T, sigma, Smax, N, thread_count);

    const double time2 = omp_get_wtime();

    const double C_exact = black_scholes_call(S0, K, r, T, sigma);
    const double abs_err = fabs(C_num - C_exact);

    //print the values of threads, N, ...
    printf("\nEuropean Call Option via Simpson + OpenMP\n");
    printf("threads   = %d\n", thread_count);
    printf("N         = %lld\n", N);
    printf("S0        = %.6f\n", S0);
    printf("K         = %.6f\n", K);
    printf("r         = %.6f\n", r);
    printf("T         = %.6f\n", T);
    printf("sigma     = %.6f\n", sigma);
    printf("Smax      = %.6f\n\n", Smax);

    printf("Simpson Price      = %23.15e\n", C_num);
    printf("Black-Scholes      = %23.15e\n", C_exact);
    printf("Absolute Error     = %23.15e\n", abs_err);
    printf("Wall Time (sec)    = %23.15e\n\n", time2 - time1);

    return 0;
}

//positive and even
static void usage(const char *prog_name)
{
    fprintf(stderr, "usage: %s <thread_count> <N>\n", prog_name);
    fprintf(stderr, "  thread_count should be positive\n");
    fprintf(stderr, "  N should be even and >= 2\n");
    fprintf(stderr, "example: %s 4 10000000\n", prog_name);
    exit(1);
}

static double Phi(double x)
{
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

static double black_scholes_call(double S0, double K, double r, double T, double sigma)
{
    const double d1 = (log(S0 / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    const double d2 = d1 - sigma * sqrt(T);

    return S0 * Phi(d1) - K * exp(-r * T) * Phi(d2);
}

//log
static double lognormal_pdf(double S, double S0, double r, double T, double sigma)
{
    if (S <= 0.0) {
        return 0.0;
    }

    const double pi = acos(-1.0);
    const double mu = log(S0) + (r - 0.5 * sigma * sigma) * T;
    const double denom = S * sigma * sqrt(2.0 * pi * T);
    const double z = log(S) - mu;

    return exp(-(z * z) / (2.0 * sigma * sigma * T)) / denom;
}

static double integrand(double S, double S0, double K, double r, double T, double sigma)
{
    return exp(-r * T) * (S - K) * lognormal_pdf(S, S0, r, T, sigma);
}

// Simpson
static double simpson_call_price(double S0, double K, double r, double T, double sigma,
                                 double Smax, long long N, int thread_count)
{
    const double a = K;
    const double b = Smax;
    const double h = (b - a) / (double)N;

    double odd_sum = 0.0;
    double even_sum = 0.0;

    #pragma omp parallel for num_threads(thread_count) reduction(+:odd_sum)
    for (long long i = 1; i < N; i += 2) {
        const double S = a + i * h;
        odd_sum += integrand(S, S0, K, r, T, sigma);
    }

    #pragma omp parallel for num_threads(thread_count) reduction(+:even_sum)
    for (long long i = 2; i < N; i += 2) {
        const double S = a + i * h;
        even_sum += integrand(S, S0, K, r, T, sigma);
    }

    const double fa = integrand(a, S0, K, r, T, sigma);
    const double fb = integrand(b, S0, K, r, T, sigma);

    return (h / 3.0) * (fa + fb + 4.0 * odd_sum + 2.0 * even_sum);
}