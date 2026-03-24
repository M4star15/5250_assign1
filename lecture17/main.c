#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif

static void usage(const char *prog_name);
static int **alloc_int_matrix(int rows, int cols);
static double **alloc_double_matrix(int rows, int cols);
static void free_int_matrix(int **mat);
static void free_double_matrix(double **mat);
static void fill_random_image(int **img, int N);
static void blur_image(int **img, double **out, int N, int thread_count);
static void print_small_input(int **img, int N);
static void print_small_output(double **out, int outN);

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage(argv[0]);
    }

    const int thread_count = (int)strtol(argv[1], NULL, 10);
    const int N = (int)strtol(argv[2], NULL, 10);

    if (thread_count <= 0 || N < 3) {
        usage(argv[0]);
    }

    const int outN = N - 2;

    int **img = alloc_int_matrix(N, N);
    double **out = alloc_double_matrix(outN, outN);

    srand((unsigned int)time(NULL));
    fill_random_image(img, N);

    const double time1 = omp_get_wtime();
    blur_image(img, out, N, thread_count);
    const double time2 = omp_get_wtime();

    printf("Input image size   : %d x %d\n", N, N);
    printf("Blurred image size : %d x %d\n", outN, outN);
    printf("With %d threads, clock_time = %11.5e (sec)\n", thread_count, time2 - time1);
    
    // we only print out for these eol 18 so it doesnt cover the whole screen
    if (N <= 18) {
        printf("\nInput image:\n");
        print_small_input(img, N);

        printf("\nBlurred image:\n");
        print_small_output(out, outN);
    }

    free_int_matrix(img);
    free_double_matrix(out);

    return 0;
}

static void usage(const char *prog_name)
{
    fprintf(stderr, "usage: %s <thread_count> <N>\n", prog_name);
    fprintf(stderr, "  thread_count should be positive\n");
    fprintf(stderr, "  N should be at least 3\n"); //when N is less than 3 we cannot really do N-2
    exit(1);
}

static int **alloc_int_matrix(int rows, int cols)
{
    int **mat = (int **)malloc(rows * sizeof(int *));
    if (mat == NULL) {
        fprintf(stderr, "Error: failed to allocate row pointers.\n");
        exit(1);
    }

    int *data = (int *)malloc(rows * cols * sizeof(int));
    if (data == NULL) {
        fprintf(stderr, "Error: failed to allocate int matrix data.\n");
        free(mat);
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        mat[i] = data + i * cols;
    }

    return mat;
}

static double **alloc_double_matrix(int rows, int cols)
{
    double **mat = (double **)malloc(rows * sizeof(double *));
    if (mat == NULL) {
        fprintf(stderr, "Error: failed to allocate row pointers.\n");
        exit(1);
    }

    double *data = (double *)malloc(rows * cols * sizeof(double));
    if (data == NULL) {
        fprintf(stderr, "Error: failed to allocate double matrix data.\n");
        free(mat);
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        mat[i] = data + i * cols;
    }

    return mat;
}

static void free_int_matrix(int **mat)
{
    if (mat != NULL) {
        free(mat[0]);
        free(mat);
    }
}

static void free_double_matrix(double **mat)
{
    if (mat != NULL) {
        free(mat[0]);
        free(mat);
    }
}

static void fill_random_image(int **img, int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            img[i][j] = rand() % 256; //formula given in question
        }
    }
}

static void blur_image(int **img, double **out, int N, int thread_count)
{
    const int outN = N - 2;

    #pragma omp parallel for num_threads(thread_count) schedule(static)
    for (int i = 0; i < outN; i++) {
        for (int j = 0; j < outN; j++) {
            int sum = 0;

            for (int di = 0; di < 3; di++) {
                for (int dj = 0; dj < 3; dj++) {
                    sum += img[i + di][j + dj];
                }
            }

            out[i][j] = sum / 9.0;  // simple summation divided by 9.0
        }
    }
}

static void print_small_input(int **img, int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%4d ", img[i][j]);
        }
        printf("\n");
    }
}

static void print_small_output(double **out, int outN)
{
    for (int i = 0; i < outN; i++) {
        for (int j = 0; j < outN; j++) {
            printf("%7.2f ", out[i][j]);
        }
        printf("\n");
    }
}