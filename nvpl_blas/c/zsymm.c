/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zsymm
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 2;
    nvpl_int_t N = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldb = 2;
    nvpl_int_t ldc = 2;
    nvpl_dcomplex_t alpha = {1.0f, 2.0f};
    nvpl_dcomplex_t beta = {1.0f, 2.0f};
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_SIDE side = CblasLeft;
    enum CBLAS_UPLO uplo = CblasUpper;
    nvpl_dcomplex_t * A = NULL;
    nvpl_dcomplex_t * B = NULL;
    nvpl_dcomplex_t * C = NULL;

    printf("\nExample: cblas_zsymm for symmetric matrix-matrix multiplication\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", ldc=%" PRId64 ", "
            "order=%c, side=%c, uplo=%c\n", (int64_t)M, (int64_t)N, (int64_t)lda, (int64_t)ldb,
            (int64_t)ldc, order_to_char(order), side_to_char(side), uplo_to_char(uplo));
    printf("           alpha=(%g, %g), beta=(%g, %g)\n", alpha.real, alpha.imag, beta.real, beta.imag);

    nvpl_int_t rowsA = (side == CblasLeft) ? M : N;
    nvpl_int_t colsA = rowsA; 
    nvpl_int_t rowsB = M;
    nvpl_int_t colsB = N;
    nvpl_int_t rowsC = M;
    nvpl_int_t colsC = N;

    // allocate memory
    A = (nvpl_dcomplex_t *)malloc(lda * colsA  * sizeof(nvpl_dcomplex_t));
    B = (nvpl_dcomplex_t *)malloc(ldb * colsB  * sizeof(nvpl_dcomplex_t));
    C = (nvpl_dcomplex_t *)malloc(ldc * colsC  * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zmatrix(A, rowsA, colsA, lda, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);
    fill_zmatrix(B, rowsB, colsB, ldb, order, Full, CblasNonUnit);
    fill_zmatrix(C, rowsC, colsC, ldc, order, Full, CblasNonUnit);

    // print input data
    print_zmatrix(A, rowsA, colsA, lda, "A", order);
    print_zmatrix(B, rowsB, colsB, ldb, "B", order);
    print_zmatrix(C, rowsC, colsC, ldc, "C", order);

    // call cblas_zsymm
    cblas_zsymm(order, side, uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);

    // print result
    printf("\nAfter cblas_zsymm operation:\n");
    print_zmatrix(C, rowsC, colsC, ldc, "C", order);

    // release memory
    free(A);
    free(B);
    free(C);
    return 0;
}
