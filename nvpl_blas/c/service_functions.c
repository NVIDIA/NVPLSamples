#include <stdio.h>
#include <stdlib.h>

#if defined(_OPENMP)
#include <omp.h>
#else
static int omp_get_max_threads() { return 1; }
#endif

#include "nvpl_blas.h"

#define M 128

static void call_gemm(const float *A, const float *B, float *C) {
    cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, M, M, M, 1.f,
            A, M, B, M, 0.f, C, M);
}

int main() {
    printf("Example is compiled with NVPL BLAS version %d.%d.%d\n"
           "            and run with NVPL BLAS version %d.%d.%d.\n\n",
            NVPL_BLAS_VERSION_MAJOR,
            NVPL_BLAS_VERSION_MINOR,
            NVPL_BLAS_VERSION_PATCH,
            nvpl_blas_get_version() / 10000,
            nvpl_blas_get_version() / 100 % 100,
            nvpl_blas_get_version() % 100);

    float *A = (float *)malloc(M * M * sizeof(*A));
    float *B = (float *)malloc(M * M * sizeof(*B));
    float *C = (float *)malloc(M * M * sizeof(*C));
    float *C2 = (float *)malloc(M * M * sizeof(*C2));

    printf("NVPL BLAS targets using %d threads.\n", nvpl_blas_get_max_threads());
    call_gemm(A, B, C);
    printf("\n");

    int use_nthreads = 2;
    printf("Set the numbers of threads for NVPL BLAS to %d.\n", use_nthreads);
    nvpl_blas_set_num_threads(use_nthreads);
    printf("Now NVPL BLAS targets using %d threads,"
            " while omp_get_max_threads() returns %d\n",
            nvpl_blas_get_max_threads(), omp_get_max_threads());
    call_gemm(A, B, C);
    printf("\n");

#if defined(_OPENMP)
    printf("Calling NVPL BLAS from parallel section...\n");
    omp_set_max_active_levels(2);
#   pragma omp parallel num_threads(2)
    {
        if (omp_get_thread_num() == 0) {
            int use_nt = 4;
            printf("For the thread 0, set NVPL BLAS local number of threads "
                    "to %d.\n", use_nt);
            int old_local_nt = nvpl_blas_set_num_threads_local(use_nt);
            call_gemm(A, B, C);
            printf("For the thread 0, reset the local number of threads "
                    "back to %d.\n", old_local_nt);
            nvpl_blas_set_num_threads_local(old_local_nt);
        }

#       pragma omp barrier // serialize output

        if (omp_get_thread_num() == 1) {
            printf("For the thread 1, simply call NVPL BLAS. "
                    "By default, since the call happens from a parallel "
                    "region, NVPL BLAS won't do further  parallelization.\n");
            call_gemm(A, B, C2);
        }
    }
#endif

    free(A);
    free(B);
    free(C);
    free(C2);

    return 0;
}
