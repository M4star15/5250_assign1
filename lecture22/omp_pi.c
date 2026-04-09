#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void usage(const char* prog_name);
double RandDouble(unsigned int* seed);

int main(int argc, char* argv[])
{
    void usage(const char* prog_name);
    double RandDouble(unsigned int* seed);

    if (argc != 2) { usage(argv[0]); }

    const long long TotalTosses = strtoll(argv[1], NULL, 10);
    if (TotalTosses <= 0) { usage(argv[0]); }

    long long GlobalCount = 0;

    const double tstart = omp_get_wtime();

    #pragma omp parallel reduction(+:GlobalCount)
    {
        unsigned int seed = (unsigned int)(12345 + 1000 * omp_get_thread_num());

        #pragma omp for
        for (long long i = 0; i < TotalTosses; i++)
        {
            const double x = RandDouble(&seed);
            const double y = RandDouble(&seed);
            const double r2 = x * x + y * y;

            if (r2 <= 1.0) { GlobalCount++; }
        }
    }

    const double tend = omp_get_wtime();

    const double pi = 4.0 * ((double) GlobalCount) / ((double) TotalTosses);

    printf("OpenMP Monte Carlo method for pi\n");
    printf("Threads     = %d\n", omp_get_max_threads());
    printf("Total tosses= %lld\n", TotalTosses);
    printf("Inside count= %lld\n", GlobalCount);
    printf("pi estimate = %24.16e\n", pi);
    printf("Runtime     = %24.16e seconds\n", tend - tstart);

    return 0;
}

void usage(const char* prog_name)
{
    fprintf(stderr, "usage: %s <NumTosses>\n", prog_name);
    fprintf(stderr, "  NumTosses should be positive\n");
    exit(1);
}

double RandDouble(unsigned int* seed)
{
    return ((double) rand_r(seed)) / ((double) RAND_MAX);
}