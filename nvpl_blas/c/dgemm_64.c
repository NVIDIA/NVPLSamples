/******************************************************************************
 * Content:
 *     This example demonstrates the _64 ILP64 CBLAS entry point:
 *     cblas_dgemm_64. It is identical to dgemm.c except the size-related
 *     arguments are declared as nvpl_int64_t and the call target is the
 *     _64-suffixed variant, which is exported from both LP64 and ILP64
 *     library builds.
 *
 ******************************************************************************/
#include "example_helper.h"
#include "nvpl_blas_cblas_64.h"  /* declares cblas_dgemm_64 */

int main() {
    nvpl_int64_t M = 2;
    nvpl_int64_t N = 2;
    nvpl_int64_t K = 2;
    nvpl_int64_t lda = 2;
    nvpl_int64_t ldb = 2;
    nvpl_int64_t ldc = 2;
    double alpha = 1.0f;
    double beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA = CblasNoTrans;
    enum CBLAS_TRANSPOSE transB = CblasNoTrans;
    double * A = NULL;
    double * B = NULL;
    double * C = NULL;

    printf("\nExample: cblas_dgemm_64 for matrix-matrix multiplication\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", ldc=%" PRId64 ", "
            "transA=%c, transB=%c, order=%c\n", (int64_t)M, (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)ldb,
            (int64_t)ldc, transpose_to_char(transA), transpose_to_char(transB), order_to_char(order));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    nvpl_int64_t rowsA = (transA == CblasNoTrans) ? M : K;
    nvpl_int64_t colsA = (transA == CblasNoTrans) ? K : M;
    nvpl_int64_t rowsB = (transB == CblasNoTrans) ? K : N;
    nvpl_int64_t colsB = (transB == CblasNoTrans) ? N : K;
    nvpl_int64_t rowsC = M;
    nvpl_int64_t colsC = N;

    // allocate memory
    A = (double *)malloc(lda * colsA  * sizeof(double));
    B = (double *)malloc(ldb * colsB  * sizeof(double));
    C = (double *)malloc(ldc * colsC  * sizeof(double));

    // fill data — the example_helper utilities take nvpl_int_t; the dimensions
    // here are small enough to narrow safely.
    fill_dmatrix(A, (nvpl_int_t)rowsA, (nvpl_int_t)colsA, (nvpl_int_t)lda, order, Full, CblasNonUnit);
    fill_dmatrix(B, (nvpl_int_t)rowsB, (nvpl_int_t)colsB, (nvpl_int_t)ldb, order, Full, CblasNonUnit);
    fill_dmatrix(C, (nvpl_int_t)rowsC, (nvpl_int_t)colsC, (nvpl_int_t)ldc, order, Full, CblasNonUnit);

    // print input data
    print_dmatrix(A, (nvpl_int_t)rowsA, (nvpl_int_t)colsA, (nvpl_int_t)lda, "A", order);
    print_dmatrix(B, (nvpl_int_t)rowsB, (nvpl_int_t)colsB, (nvpl_int_t)ldb, "B", order);
    print_dmatrix(C, (nvpl_int_t)rowsC, (nvpl_int_t)colsC, (nvpl_int_t)ldc, "C", order);

    // call cblas_dgemm_64 — the _64 entry point takes nvpl_int64_t arguments
    cblas_dgemm_64(order, transA, transB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);

    // print result
    printf("\nAfter gemm operation:\n");
    print_dmatrix(C, (nvpl_int_t)rowsC, (nvpl_int_t)colsC, (nvpl_int_t)ldc, "C", order);

    // release memory
    free(A);
    free(B);
    free(C);
    return 0;
}
