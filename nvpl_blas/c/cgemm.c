/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_cgemm
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
    nvpl_scomplex_t alpha = {1.0f, 2.0f};
    nvpl_scomplex_t beta = {1.0f, 2.0f};
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA = CblasNoTrans;
    enum CBLAS_TRANSPOSE transB = CblasNoTrans;
    nvpl_scomplex_t * A = NULL;
    nvpl_scomplex_t * B = NULL;
    nvpl_scomplex_t * C = NULL;

    printf("\nExample: cblas_cgemm for matrix-matrix multiplication\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", ldc=%" PRId64 ", "
            "transA=%c, transB=%c, order=%c\n", (int64_t)M, (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)ldb,
            (int64_t)ldc, transpose_to_char(transA), transpose_to_char(transB), order_to_char(order));
    printf("           alpha=(%g, %g), beta=(%g, %g)\n", alpha.real, alpha.imag, beta.real, beta.imag);

    nvpl_int_t rowsA = (transA == CblasNoTrans) ? M : K;
    nvpl_int_t colsA = (transA == CblasNoTrans) ? K : M;
    nvpl_int_t rowsB = (transB == CblasNoTrans) ? K : N;
    nvpl_int_t colsB = (transB == CblasNoTrans) ? N : K;
    nvpl_int_t rowsC = M;
    nvpl_int_t colsC = N;

    // allocate memory
    A = (nvpl_scomplex_t *)malloc(lda * colsA  * sizeof(nvpl_scomplex_t));
    B = (nvpl_scomplex_t *)malloc(ldb * colsB  * sizeof(nvpl_scomplex_t));
    C = (nvpl_scomplex_t *)malloc(ldc * colsC  * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cmatrix(A, rowsA, colsA, lda, order, Full, CblasNonUnit);
    fill_cmatrix(B, rowsB, colsB, ldb, order, Full, CblasNonUnit);
    fill_cmatrix(C, rowsC, colsC, ldc, order, Full, CblasNonUnit);

    // print input data
    print_cmatrix(A, rowsA, colsA, lda, "A", order);
    print_cmatrix(B, rowsB, colsB, ldb, "B", order);
    print_cmatrix(C, rowsC, colsC, ldc, "C", order);

    // call cblas_cgemm
    cblas_cgemm(order, transA, transB, M, N, K, (void *)&alpha, A, lda, B, ldb, (void *)&beta, C, ldc);

    // print result
    printf("\nAfter gemm operation:\n");
    print_cmatrix(C, rowsC, colsC, ldc, "C", order);

    // release memory
    free(A);
    free(B);
    free(C);
    return 0;
}
