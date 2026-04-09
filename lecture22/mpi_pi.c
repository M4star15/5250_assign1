#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void usage(const char* prog_name);
double RandDouble(unsigned int* seed);
long long CountInsideCircle(const long long NumTosses, unsigned int seed);

int main(int argc, char* argv[])
{
    long long CountInsideCircle(const long long NumTosses, unsigned int seed);
    double RandDouble(unsigned int* seed);
    void usage(const char* prog_name);

    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (argc != 2)
    {
        if (rank == 0) { usage(argv[0]); }
        MPI_Finalize();
        return 1;
    }

    const long long TotalTosses = strtoll(argv[1], NULL, 10);
    if (TotalTosses <= 0)
    {
        if (rank == 0) { usage(argv[0]); }
        MPI_Finalize();
        return 1;
    }

    long long LocalTosses = TotalTosses / nprocs;
    const long long Remainder = TotalTosses % nprocs;
    if (rank < Remainder) { LocalTosses++; }

    const unsigned int seed = (unsigned int)(12345 + 1000 * rank);

    const double tstart = MPI_Wtime();

    const long long LocalCount = CountInsideCircle(LocalTosses, seed);

    long long GlobalCount = 0;
    MPI_Reduce(&LocalCount, &GlobalCount, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    const double tend = MPI_Wtime();

    if (rank == 0)
    {
        const double pi = 4.0 * ((double) GlobalCount) / ((double) TotalTosses);
        printf("MPI Monte Carlo method for pi\n");
        printf("Processes   = %d\n", nprocs);
        printf("Total tosses= %lld\n", TotalTosses);
        printf("Inside count= %lld\n", GlobalCount);
        printf("pi estimate = %24.16e\n", pi);
        printf("Runtime     = %24.16e seconds\n", tend - tstart);
    }

    MPI_Finalize();
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

long long CountInsideCircle(const long long NumTosses, unsigned int seed)
{
    long long count = 0;

    for (long long i = 0; i < NumTosses; i++)
    {
        const double x = RandDouble(&seed);
        const double y = RandDouble(&seed);
        const double r2 = x * x + y * y;

        if (r2 <= 1.0) { count++; }
    }

    return count;
}