/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zherk
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t K = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldc = 2;
    double alpha = 1.0f;
    double beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    enum CBLAS_TRANSPOSE trans = CblasNoTrans;
    nvpl_dcomplex_t * A = NULL;
    nvpl_dcomplex_t * C = NULL;

    printf("\nExample: cblas_zherk for the Hermitian rank-k update\n\n");
    printf("#### args: n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", ldc=%" PRId64 ", "
            "order=%c, uplo=%c, trans=%c\n", (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)ldc,
            order_to_char(order), uplo_to_char(uplo), transpose_to_char(trans));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    nvpl_int_t rowsA = (trans == CblasNoTrans) ? N : K;
    nvpl_int_t colsA = (trans == CblasNoTrans) ? K : N;
    nvpl_int_t rowsC = N;
    nvpl_int_t colsC = N;

    // allocate memory
    A = (nvpl_dcomplex_t *)malloc(lda * colsA  * sizeof(nvpl_dcomplex_t));
    C = (nvpl_dcomplex_t *)malloc(ldc * colsC  * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zmatrix(A, rowsA, colsA, lda, order, Full, CblasNonUnit);
    fill_zmatrix(C, rowsC, colsC, ldc, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);

    // print input data
    print_zmatrix(A, rowsA, colsA, lda, "A", order);
    print_zmatrix(C, rowsC, colsC, ldc, "C", order);

    // call cblas_zherk
    cblas_zherk(order, uplo, trans, N, K, alpha, A, lda, beta, C, ldc);

    // print result
    printf("\nAfter cblas_zherk operation:\n");
    print_zmatrix(C, rowsC, colsC, ldc, "C", order);

    // release memory
    free(A);
    free(C);
    return 0;
}
