/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_strmm
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 2;
    nvpl_int_t N = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldb = 2;
    float alpha = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transa = CblasNoTrans;
    enum CBLAS_UPLO uplo = CblasUpper;
    enum CBLAS_SIDE side = CblasLeft;
    enum CBLAS_DIAG diag = CblasNonUnit;
    float * A = NULL;
    float * B = NULL;

    printf("\nExample: cblas_strmm for the triangular matrix-matrix multiplication\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", order=%c"
            ", side=%c, uplo=%c, transa=%c, diag=%c\n", (int64_t)M, (int64_t)N, (int64_t)lda, (int64_t)ldb,
            order_to_char(order), side_to_char(side), uplo_to_char(uplo), transpose_to_char(transa), diag_to_char(diag));
    printf("           alpha=%g\n", alpha);

    nvpl_int_t rowsA = (side == CblasLeft) ? M : N;
    nvpl_int_t colsA = rowsA;
    nvpl_int_t rowsB = M;
    nvpl_int_t colsB = N;

    // allocate memory
    A = (float *)malloc(lda * colsA  * sizeof(float));
    B = (float *)malloc(ldb * colsB  * sizeof(float));

    // fill data
    fill_smatrix(A, rowsA, colsA, lda, order, UPLO_TO_FILL_MODE(uplo), diag);
    fill_smatrix(B, rowsB, colsB, ldb, order, Full, CblasNonUnit);

    // print input data
    print_smatrix(A, rowsA, colsA, lda, "A", order);
    print_smatrix(B, rowsB, colsB, ldb, "B", order);

    // call cblas_strmm
    cblas_strmm(order, side, uplo, transa, diag, M, N, alpha, A, lda, B, ldb);

    // print result
    printf("\nAfter strmm operation:\n");
    print_smatrix(B, rowsB, colsB, ldb, "B", order);

    // release memory
    free(A);
    free(B);
    return 0;
}
