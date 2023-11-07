/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zher2k
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t K = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldb = 2;
    nvpl_int_t ldc = 2;
    nvpl_dcomplex_t alpha = {1.0f, 2.0f};
    double beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE trans = CblasNoTrans;
    enum CBLAS_UPLO uplo = CblasUpper;
    nvpl_dcomplex_t * A = NULL;
    nvpl_dcomplex_t * B = NULL;
    nvpl_dcomplex_t * C = NULL;

    printf("\nExample: cblas_zher2k for the Hermitian rank-2k update\n\n");
    printf("#### args: n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", ldc=%" PRId64 ", "
            "trans=%c, uplo=%c, order=%c\n", (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)ldb,
            (int64_t)ldc, transpose_to_char(trans), uplo_to_char(uplo), order_to_char(order));
    printf("           alpha=(%g, %g), beta=%g\n", alpha.real, alpha.imag, beta);

    nvpl_int_t rowsA = (trans == CblasNoTrans) ? N : K;
    nvpl_int_t colsA = (trans == CblasNoTrans) ? K : N;
    nvpl_int_t rowsB = (trans == CblasNoTrans) ? N : K;
    nvpl_int_t colsB = (trans == CblasNoTrans) ? K : N;
    nvpl_int_t rowsC = N;
    nvpl_int_t colsC = N;

    // allocate memory
    A = (nvpl_dcomplex_t *)malloc(lda * colsA  * sizeof(nvpl_dcomplex_t));
    B = (nvpl_dcomplex_t *)malloc(ldb * colsB  * sizeof(nvpl_dcomplex_t));
    C = (nvpl_dcomplex_t *)malloc(ldc * colsC  * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zmatrix(A, rowsA, colsA, lda, order, Full, CblasNonUnit);
    fill_zmatrix(B, rowsB, colsB, ldb, order, Full, CblasNonUnit);
    fill_zmatrix(C, rowsC, colsC, ldc, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);

    // print input data
    print_zmatrix(A, rowsA, colsA, lda, "A", order);
    print_zmatrix(B, rowsB, colsB, ldb, "B", order);
    print_zmatrix(C, rowsC, colsC, ldc, "C", order);

    // call cblas_zher2k
    cblas_zher2k(order, uplo, trans, N, K, &alpha, A, lda, B, ldb, beta, C, ldc);

    // print result
    printf("\nAfter zher2k operation:\n");
    print_zmatrix(C, rowsC, colsC, ldc, "C", order);

    // release memory
    free(A);
    free(B);
    free(C);
    return 0;
}
