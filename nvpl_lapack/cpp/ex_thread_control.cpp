#include <nvpl_lapack.h>
#include <omp.h>
#include <stdio.h>

/* Auxiliary routine: printing a vector of floats */
void print_svector(const char *desc, nvpl_int_t n, const float *vec) {
    nvpl_int_t j;
    printf("\n %s\n", desc);
    for (j = 0; j < n; j++)
        printf(" %6.2f", vec[j]);
    printf("\n");
}

/* Workaround for NVHPC compiler bug */
void __attribute__((noinline))
slarfg(nvpl_int_t *n, float *alpha, float *X, nvpl_int_t *incx, float *tau) {
    slarfg_(n, alpha, X, incx, tau);
}

/* Main program */
int main() {
    nvpl_int_t n0, n1, incx0, incx1;
    float alpha0, alpha1, tau0, tau1;

    /* Initialization */
    float x0[4] = {1, 2, 3, 4};
    float x1[4] = {6, 7, 8, 9};

    n0 = 5;
    n1 = 5;
    incx0 = 1;
    incx1 = 1;
    alpha0 = 1.0;
    alpha1 = 2.0;
    tau0 = 3.0;
    tau1 = 4.0;

    printf("NVPL LAPACK version: %d \n", nvpl_lapack_get_version());
    printf("sizeof(nvpl_int_t) = %d \n", (int)sizeof(nvpl_int_t));

    print_svector("Initial vector #0", n0 - 1, x0);
    print_svector("Initial vector #1", n1 - 1, x1);
    printf("\n");

    printf("OpenMP max number of threads = %d \n", omp_get_max_threads());

    /* Expected: same value as omp_get_max_threads() as no other settings applied */
    printf("(before any threading control APIs are called) LAPACK setting for "
           "nthreads = %d \n",
            nvpl_lapack_get_max_threads());

    /* Setting the global upper limit on the number of threads
       which can be used by NVPL LAPACK. */
    nvpl_lapack_set_num_threads(7);

    /* Expected: 7 as the minimum of OpenMP and global NVPL LAPACK settings */
    printf("(after setting the global limit) LAPACK setting for nthreads = %d "
           "\n",
            nvpl_lapack_get_max_threads());

    omp_set_max_active_levels(2); /* could be (deprecated) omp_set_nested(1) */

    /* Expected: 7 as the local setting takes the highest precedence */
    printf("(before the parallel region) LAPACK setting for nthreads = %d \n",
            nvpl_lapack_get_max_threads());

/* This parallel section demonstrates that without any local settings,
       NVPL LAPACK would use a single thread in case of nested parallelism */
#pragma omp parallel num_threads(2)
    {
        int tid = omp_get_thread_num();
        /* Expected: 1 as the call happens inside a parallel region and no local
           settings for LAPACK have been set */
        printf("tid %d: (inside a dummy parallel region) LAPACK setting for "
               "nthreads = %d \n",
                tid, nvpl_lapack_get_max_threads());
    }

/* This parallel section demonstrates the effects of having local settings */
#pragma omp parallel num_threads(3)
    {
        int tid = omp_get_thread_num();

        /* To demonstrate different behaviors, Threads 0 and 1 get a local
           setting for the number of threads */
        if (tid == 0 || tid == 1) {
            nvpl_lapack_set_num_threads_local(3);
        }

        /* Thread 0 is showing the recommended practice which is:
           When new local settings are set, one should
           save the previous value and restore it after calling LAPACK so that
           previously set local settings are not overwritten and lost */
        if (tid == 0) {
            /* Saving the local number of threads set previously */
            int nthreads_prev = nvpl_lapack_set_num_threads_local(4);
            /* Expected: 4 as the new local settings are applied */
            printf("tid %d: (locally) LAPACK setting for nthreads = %d \n", tid,
                    nvpl_lapack_get_max_threads());
            slarfg(&n0, &alpha0, x0, &incx0, &tau0);
            /* Restoring the number of threads set for LAPACK locally*/
            nvpl_lapack_set_num_threads_local(nthreads_prev);
        }
        /* Thread 1 does not follow the best practice and only sets the new
           local number of threads to use */
        if (tid == 1) {
            nvpl_lapack_set_num_threads_local(2);
            printf("tid %d: (locally) LAPACK setting for nthreads = %d \n", tid,
                    nvpl_lapack_get_max_threads());
            slarfg(&n1, &alpha1, x1, &incx1, &tau1);
        }

#pragma omp barrier
        /* Demonstrating the difference between following and not-following
           the recommended practice */
        if (tid == 0 || tid == 1) {
            /* Expected: 3 for Thread 0 (with save/restore),
        but 2 for Thread 1 (just overwrote the previous local setting) */
            printf("tid %d: (aftermath) LAPACK setting for nthreads = %d \n",
                    tid, nvpl_lapack_get_max_threads());
        }

#pragma omp barrier
        /* Thread 2 does not use apply any local settings and thus uses the
           global limit */
        if (tid == 2) {
            /* Expected: 7 */
            printf("tid %d: (locally) LAPACK setting for nthreads = %d \n", tid,
                    nvpl_lapack_get_max_threads());
        }

#pragma omp barrier
        /* Demonstrating the effect of calling with argument equal to 0:
           this disables any local settings applied previously */
        if (tid == 0) {
            nvpl_lapack_set_num_threads_local(0);
            /* Expected: 7 as the  */
            printf("tid %d: (after disabling previous local settings) "
                   "LAPACK setting for nthreads = %d \n",
                    tid, nvpl_lapack_get_max_threads());
        }
    }

    /* Expected: 7 as the main thread only had a global setting after the local
       setting got disabled in the end of the parallel region. Here it is
       assumed that Thread 0 inside the parallel region is the same as the main
       thread outside the parallel region. */
    printf("(after the parallel region) LAPACK setting for nthreads = %d \n",
            nvpl_lapack_get_max_threads());

    /* Print generated reflectors (one for seq, two for omp) */
    print_svector("Householder reflector #0", n0 - 1, x0);
    print_svector("Householder reflector #1", n1 - 1, x1);
    printf("\n");

    return 0;
}
