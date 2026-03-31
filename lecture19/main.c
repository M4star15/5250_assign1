#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <string.h>

static void initialize_signal(double *x, long long N) {
    // Deterministic signal, no rand() needed
    // Keeps values bounded and nontrivial
    #pragma omp parallel for
    for (long long i = 0; i < N; i++) {
        x[i] = sin(0.000001 * (double)i) + 0.5 * cos(0.000002 * (double)i) + 1.0;
    }
}

// Fine-grained versions 

static double normalize_l2_fine(double *x, long long N, int thread_count) {
    double sumsq = 0.0;

    double t1 = omp_get_wtime();

    // so that it doesnt start until prev one is done
    #pragma omp parallel for num_threads(thread_count) reduction(+:sumsq)
    for (long long i = 0; i < N; i++) {
        sumsq += x[i] * x[i];
    }

    // first parallel for ends before execution continues
    double norm = sqrt(sumsq);

    #pragma omp parallel for num_threads(thread_count)
    for (long long i = 0; i < N; i++) {
        x[i] /= norm;
    }

    double t2 = omp_get_wtime();
    printf("Computed L2 norm before normalization = %.12e\n", norm);
    return t2 - t1;     // for time
}

static double normalize_max_fine(double *x, long long N, int thread_count) {
    double maxval = 0.0;

    double t1 = omp_get_wtime();

    #pragma omp parallel for num_threads(thread_count) reduction(max:maxval)
    for (long long i = 0; i < N; i++) {
        double a = fabs(x[i]);
        if (a > maxval) maxval = a;
    }

    #pragma omp parallel for num_threads(thread_count)
    for (long long i = 0; i < N; i++) {
        x[i] /= maxval;
    }

    double t2 = omp_get_wtime();
    printf("Computed max norm before normalization = %.12e\n", maxval);
    return t2 - t1;
}

// Coarse-grained versions...

static double normalize_l2_coarse(double *x, long long N, int thread_count) {
    double sumsq = 0.0;

    double t1 = omp_get_wtime();

    #pragma omp parallel num_threads(thread_count)
    {
        int tid = omp_get_thread_num();     // thread number for parallelization

        long long chunk = N / thread_count;
        long long start = tid * chunk;
        long long end = (tid == thread_count - 1) ? N : start + chunk;

        double local_sumsq = 0.0;
        for (long long i = start; i < end; i++) {
            local_sumsq += x[i] * x[i];
        }

        #pragma omp critical
        {
            sumsq += local_sumsq;
        }

        #pragma omp barrier
        // so that the prev operation ends before the next
        double norm = sqrt(sumsq);

        for (long long i = start; i < end; i++) {
            x[i] /= norm;
        }
    }

    double t2 = omp_get_wtime();
    printf("Computed L2 norm before normalization = %.12e\n", sqrt(sumsq));
    return t2 - t1;
}

static double normalize_max_coarse(double *x, long long N, int thread_count) {
    double maxval = 0.0;

    double t1 = omp_get_wtime();
    //
    #pragma omp parallel num_threads(thread_count)
    {
        int tid = omp_get_thread_num();     // thread numbers

        long long chunk = N / thread_count;
        long long start = tid * chunk;
        long long end = (tid == thread_count - 1) ? N : start + chunk;

        double local_max = 0.0;
        for (long long i = start; i < end; i++) {
            double a = fabs(x[i]);
            if (a > local_max) local_max = a;
        }

        #pragma omp critical
        {
            if (local_max > maxval) maxval = local_max;
        }

        #pragma omp barrier

        for (long long i = start; i < end; i++) {
            x[i] /= maxval;
        }
    }

    double t2 = omp_get_wtime();
    printf("Computed max norm before normalization = %.12e\n", maxval);
    return t2 - t1;
}

// Check functions

static double check_l2_norm(double *x, long long N, int thread_count) {
    double sumsq = 0.0;
    #pragma omp parallel for num_threads(thread_count) reduction(+:sumsq)
    for (long long i = 0; i < N; i++) {
        sumsq += x[i] * x[i];
    }
    return sqrt(sumsq);
}

static double check_max_norm(double *x, long long N, int thread_count) {
    double maxval = 0.0;
    #pragma omp parallel for num_threads(thread_count) reduction(max:maxval)
    for (long long i = 0; i < N; i++) {
        double a = fabs(x[i]);
        if (a > maxval) maxval = a;
    }
    return maxval;
}

// Main

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <N> <threads> <norm_type> <grain>\n", argv[0]);
        printf("  <N>          : signal length (example: 100000000)\n");
        printf("  <threads>    : 1, 2, 4, 8, 16\n");
        printf("  <norm_type>  : l2 or max\n");
        printf("  <grain>      : fine or coarse\n");
        return 1;
    }

    long long N = atoll(argv[1]);
    int thread_count = atoi(argv[2]);       // thread count input
    char *norm_type = argv[3];
    char *grain = argv[4];

    if (N <= 0 || thread_count <= 0) {
        fprintf(stderr, "Error: N and thread_count must be positive.\n");
        return 1;
    }

    double *x = (double *) malloc((size_t)N * sizeof(double));
    if (!x) {
        fprintf(stderr, "Error: memory allocation failed for N = %lld\n", N);
        return 1;
    }

    initialize_signal(x, N);

    double elapsed = 0.0;

    if (strcmp(norm_type, "l2") == 0 && strcmp(grain, "fine") == 0) {
        elapsed = normalize_l2_fine(x, N, thread_count);
        double chk = check_l2_norm(x, N, thread_count);
        printf("Check after normalization: ||x||_2 = %.12e\n", chk);
    }
    else if (strcmp(norm_type, "l2") == 0 && strcmp(grain, "coarse") == 0) {
        elapsed = normalize_l2_coarse(x, N, thread_count);
        double chk = check_l2_norm(x, N, thread_count);
        printf("Check after normalization: ||x||_2 = %.12e\n", chk);
    }
    else if (strcmp(norm_type, "max") == 0 && strcmp(grain, "fine") == 0) {
        elapsed = normalize_max_fine(x, N, thread_count);
        double chk = check_max_norm(x, N, thread_count);
        printf("Check after normalization: ||x||_max = %.12e\n", chk);
    }
    else if (strcmp(norm_type, "max") == 0 && strcmp(grain, "coarse") == 0) {
        elapsed = normalize_max_coarse(x, N, thread_count);
        double chk = check_max_norm(x, N, thread_count);
        printf("Check after normalization: ||x||_max = %.12e\n", chk);
    }
    else {
        fprintf(stderr, "Error: invalid arguments.\n");
        free(x);
        return 1;
    }

    printf("N = %lld\n", N);
    printf("Threads = %d\n", thread_count);
    printf("Norm type = %s\n", norm_type);
    printf("Parallelism = %s\n", grain);
    printf("Elapsed time = %.6f seconds\n", elapsed);

    free(x);
    return 0;
}