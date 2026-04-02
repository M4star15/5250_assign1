#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    int mode;       /* 0 = potential, 1 = bessel */
    double param;   /* x0 for potential, x for bessel */
} IntegrandParams;

static void usage(const char *prog) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s <mode> <threads> <num_points> <tol> <prefix>\n", prog);
    fprintf(stderr, "  mode = potential or bessel\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "  %s potential 4 401 1e-8 potential_t4\n", prog);
    fprintf(stderr, "  %s bessel    4 501 1e-8 bessel_t4\n", prog);
    exit(1);
}

// rho(x)

static double rho_fun(double x) {
    return exp(-x * x);
}

//integrands 

/* potential integrand:
   (rho(x) - rho(x0)) / |x-x0|   if x != x0
   0                             if x == x0
*/
static double integrand_potential(double x, void *ctx) {
    IntegrandParams *p = (IntegrandParams *)ctx;
    double x0 = p->param;

    if (fabs(x - x0) < 1.0e-14) {
        return 0.0;
    }

    return (rho_fun(x) - rho_fun(x0)) / fabs(x - x0);
}

//J0(x) = (1/pi) integral_0^pi cos(x sin(theta)) dtheta 
static double integrand_bessel(double theta, void *ctx) {
    IntegrandParams *p = (IntegrandParams *)ctx;
    double x = p->param;
    return cos(x * sin(theta));
}

static double eval_integrand(double t, void *ctx) {
    IntegrandParams *p = (IntegrandParams *)ctx;
    if (p->mode == 0) {
        return integrand_potential(t, ctx);
    } else {
        return integrand_bessel(t, ctx);
    }
}

//Simpson rule 

static double simpson_rule(double a, double b,
                           double (*f)(double, void *), void *ctx) {
    double c = 0.5 * (a + b);
    return (b - a) * (f(a, ctx) + 4.0 * f(c, ctx) + f(b, ctx)) / 6.0;
}

// Adaptive Simpson

static double adaptive_recursive(double a, double b, double tol,
                                 double (*f)(double, void *), void *ctx,
                                 int depth, FILE *trace_file) {
    const int MAX_DEPTH = 50;

    double c   = 0.5 * (a + b);
    double qab = simpson_rule(a, b, f, ctx);
    double qac = simpson_rule(a, c, f, ctx);
    double qcb = simpson_rule(c, b, f, ctx);

    double err_est = fabs(qac + qcb - qab) / 15.0;

    if (err_est < tol || depth >= MAX_DEPTH) {
        if (trace_file != NULL) {
            fprintf(trace_file, "%.15e %.15e\n", a, b);
        }
        return qac + qcb;
    }

    return adaptive_recursive(a, c, 0.5 * tol, f, ctx, depth + 1, trace_file)
         + adaptive_recursive(c, b, 0.5 * tol, f, ctx, depth + 1, trace_file);
}

static double adaptive_simpson(double a, double b, double tol,
                               double (*f)(double, void *), void *ctx) {
    return adaptive_recursive(a, b, tol, f, ctx, 0, NULL);
}

static double adaptive_simpson_trace(double a, double b, double tol,
                                     double (*f)(double, void *), void *ctx,
                                     FILE *trace_file) {
    return adaptive_recursive(a, b, tol, f, ctx, 0, trace_file);
}

//problem wrappers 

static double compute_potential(double x0, double tol) {
    IntegrandParams p;
    p.mode = 0;
    p.param = x0;

    double integral_part = adaptive_simpson(-1.0, 1.0, tol, eval_integrand, &p);

    /* V(x0) = integral_part + rho(x0) * log((1-x0)/(x0+1)) */
    double log_part = rho_fun(x0) * log((1.0 - x0) / (x0 + 1.0));

    return integral_part + log_part;
}

static double compute_bessel(double x, double tol) {
    IntegrandParams p;
    p.mode = 1;
    p.param = x;

    return adaptive_simpson(0.0, M_PI, tol, eval_integrand, &p) / M_PI;
}

// main
int main(int argc, char *argv[]) {
    if (argc != 6) {
        usage(argv[0]);
    }

    char mode_str[32];
    snprintf(mode_str, sizeof(mode_str), "%s", argv[1]);

    int threads = atoi(argv[2]);
    int num_points = atoi(argv[3]);
    double tol = atof(argv[4]);

    char prefix[128];
    snprintf(prefix, sizeof(prefix), "%s", argv[5]);

    if (threads <= 0 || num_points < 2 || tol <= 0.0) {
        usage(argv[0]);
    }

    int mode = -1;
    if (strcmp(mode_str, "potential") == 0) {
        mode = 0;
    } else if (strcmp(mode_str, "bessel") == 0) {
        mode = 1;
    } else {
        usage(argv[0]);
    }

    char values_file[256];
    char intervals_file[256];
    snprintf(values_file, sizeof(values_file), "%s_values.data", prefix);
    snprintf(intervals_file, sizeof(intervals_file), "%s_intervals.data", prefix);

    double *xvals = (double *)malloc((size_t)num_points * sizeof(double));
    double *yvals = (double *)malloc((size_t)num_points * sizeof(double));
    if (!xvals || !yvals) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(xvals);
        free(yvals);
        return 1;
    }

    double left, right;
    if (mode == 0) {
        double eps = 1.0e-6;
        left  = -1.0 + eps;
        right =  1.0 - eps;
    } else {
        left  = 0.0;
        right = 50.0;
    }

    double h = (right - left) / (double)(num_points - 1);
    for (int i = 0; i < num_points; i++) {
        xvals[i] = left + i * h;
    }

    omp_set_num_threads(threads);

    double t1 = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < num_points; i++) {
        if (mode == 0) {
            yvals[i] = compute_potential(xvals[i], tol);
        } else {
            yvals[i] = compute_bessel(xvals[i], tol);
        }
    }

    double t2 = omp_get_wtime();

    FILE *fv = fopen(values_file, "w");
    if (!fv) {
        fprintf(stderr, "Could not open %s\n", values_file);
        free(xvals);
        free(yvals);
        return 1;
    }

    for (int i = 0; i < num_points; i++) {
        fprintf(fv, "%.15e %.15e\n", xvals[i], yvals[i]);
    }
    fclose(fv);

    // representative interval trace for lecture-style interval plot
    int mid = num_points / 2;
    FILE *ft = fopen(intervals_file, "w");
    if (!ft) {
        fprintf(stderr, "Could not open %s\n", intervals_file);
        free(xvals);
        free(yvals);
        return 1;
    }

    IntegrandParams p;
    p.mode = mode;
    p.param = xvals[mid];

    if (mode == 0) {
        (void)adaptive_simpson_trace(-1.0, 1.0, tol, eval_integrand, &p, ft);
    } else {
        (void)adaptive_simpson_trace(0.0, M_PI, tol, eval_integrand, &p, ft);
    }

    fclose(ft);
    //print the info
    printf("mode                = %s\n", mode_str);
    printf("threads             = %d\n", threads);
    printf("num_points          = %d\n", num_points);
    printf("tol                 = %.15e\n", tol);
    printf("omp_get_num_procs   = %d\n", omp_get_num_procs());
    printf("omp_get_max_threads = %d\n", omp_get_max_threads());
    printf("elapsed_time        = %.8f\n", t2 - t1);
    printf("values_file         = %s\n", values_file);
    printf("intervals_file      = %s\n", intervals_file);

    free(xvals);
    free(yvals);
    return 0;
}