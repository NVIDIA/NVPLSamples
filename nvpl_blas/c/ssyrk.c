/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_ssyrk
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t K = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldc = 2;
    float alpha = 1.0f;
    float beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    enum CBLAS_TRANSPOSE trans = CblasNoTrans;
    float * A = NULL;
    float * C = NULL;

    printf("\nExample: cblas_ssyrk for the symmetric rank-k update\n\n");
    printf("#### args: n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", ldc=%" PRId64 ", "
            "order=%c, uplo=%c, trans=%c\n", (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)ldc,
            order_to_char(order), uplo_to_char(uplo), transpose_to_char(trans));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    nvpl_int_t rowsA = (trans == CblasNoTrans) ? N : K;
    nvpl_int_t colsA = (trans == CblasNoTrans) ? K : N;
    nvpl_int_t rowsC = N;
    nvpl_int_t colsC = N;

    // allocate memory
    A = (float *)malloc(lda * colsA  * sizeof(float));
    C = (float *)malloc(ldc * colsC  * sizeof(float));

    // fill data
    fill_smatrix(A, rowsA, colsA, lda, order, Full, CblasNonUnit);
    fill_smatrix(C, rowsC, colsC, ldc, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);

    // print input data
    print_smatrix(A, rowsA, colsA, lda, "A", order);
    print_smatrix(C, rowsC, colsC, ldc, "C", order);

    // call cblas_ssyrk
    cblas_ssyrk(order, uplo, trans, N, K, alpha, A, lda, beta, C, ldc);

    // print result
    printf("\nAfter cblas_ssyrk operation:\n");
    print_smatrix(C, rowsC, colsC, ldc, "C", order);

    // release memory
    free(A);
    free(C);
    return 0;
}
