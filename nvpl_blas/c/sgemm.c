/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_sgemm
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 2;
    nvpl_int_t N = 2;
    nvpl_int_t K = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldb = 2;
    nvpl_int_t ldc = 2;
    float alpha = 1.0f;
    float beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA = CblasNoTrans;
    enum CBLAS_TRANSPOSE transB = CblasNoTrans;
    float * A = NULL;
    float * B = NULL;
    float * C = NULL;

    printf("\nExample: cblas_sgemm for matrix-matrix multiplication\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", ldc=%" PRId64 ", "
            "transA=%c, transB=%c, order=%c\n", (int64_t)M, (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)ldb,
            (int64_t)ldc, transpose_to_char(transA), transpose_to_char(transB), order_to_char(order));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    nvpl_int_t rowsA = (transA == CblasNoTrans) ? M : K;
    nvpl_int_t colsA = (transA == CblasNoTrans) ? K : M;
    nvpl_int_t rowsB = (transB == CblasNoTrans) ? K : N;
    nvpl_int_t colsB = (transB == CblasNoTrans) ? N : K;
    nvpl_int_t rowsC = M;
    nvpl_int_t colsC = N;

    // allocate memory
    A = (float *)malloc(lda * colsA  * sizeof(float));
    B = (float *)malloc(ldb * colsB  * sizeof(float));
    C = (float *)malloc(ldc * colsC  * sizeof(float));

    // fill data
    fill_smatrix(A, rowsA, colsA, lda, order, Full, CblasNonUnit);
    fill_smatrix(B, rowsB, colsB, ldb, order, Full, CblasNonUnit);
    fill_smatrix(C, rowsC, colsC, ldc, order, Full, CblasNonUnit);

    // print input data
    print_smatrix(A, rowsA, colsA, lda, "A", order);
    print_smatrix(B, rowsB, colsB, ldb, "B", order);
    print_smatrix(C, rowsC, colsC, ldc, "C", order);

    // call cblas_sgemm
    cblas_sgemm(order, transA, transB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);

    // print result
    printf("\nAfter gemm operation:\n");
    print_smatrix(C, rowsC, colsC, ldc, "C", order);

    // release memory
    free(A);
    free(B);
    free(C);
    return 0;
}
