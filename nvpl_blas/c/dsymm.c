/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dsymm
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 2;
    nvpl_int_t N = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldb = 2;
    nvpl_int_t ldc = 2;
    double alpha = 1.0f;
    double beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_SIDE side = CblasLeft;
    enum CBLAS_UPLO uplo = CblasUpper;
    double * A = NULL;
    double * B = NULL;
    double * C = NULL;

    printf("\nExample: cblas_dsymm for symmetric matrix-matrix multiplication\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", ldc=%" PRId64 ", "
            "order=%c, side=%c, uplo=%c\n", (int64_t)M, (int64_t)N, (int64_t)lda, (int64_t)ldb,
            (int64_t)ldc, order_to_char(order), side_to_char(side), uplo_to_char(uplo));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    nvpl_int_t rowsA = (side == CblasLeft) ? M : N;
    nvpl_int_t colsA = rowsA; 
    nvpl_int_t rowsB = M;
    nvpl_int_t colsB = N;
    nvpl_int_t rowsC = M;
    nvpl_int_t colsC = N;

    // allocate memory
    A = (double *)malloc(lda * colsA  * sizeof(double));
    B = (double *)malloc(ldb * colsB  * sizeof(double));
    C = (double *)malloc(ldc * colsC  * sizeof(double));

    // fill data
    fill_dmatrix(A, rowsA, colsA, lda, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);
    fill_dmatrix(B, rowsB, colsB, ldb, order, Full, CblasNonUnit);
    fill_dmatrix(C, rowsC, colsC, ldc, order, Full, CblasNonUnit);

    // print input data
    print_dmatrix(A, rowsA, colsA, lda, "A", order);
    print_dmatrix(B, rowsB, colsB, ldb, "B", order);
    print_dmatrix(C, rowsC, colsC, ldc, "C", order);

    // call cblas_dsymm
    cblas_dsymm(order, side, uplo, M, N, alpha, A, lda, B, ldb, beta, C, ldc);

    // print result
    printf("\nAfter cblas_dsymm operation:\n");
    print_dmatrix(C, rowsC, colsC, ldc, "C", order);

    // release memory
    free(A);
    free(B);
    free(C);
    return 0;
}