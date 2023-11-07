/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dtrsm
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 2;
    nvpl_int_t N = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldb = 2;
    double alpha = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transa = CblasNoTrans;
    enum CBLAS_UPLO uplo = CblasUpper;
    enum CBLAS_SIDE side = CblasLeft;
    enum CBLAS_DIAG diag = CblasNonUnit;
    double * A = NULL;
    double * B = NULL;

    printf("\nExample: cblas_dtrsm for solving the triangular linear system with multiple right-hand-sides\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", order=%c"
            ", side=%c, uplo=%c, transa=%c, diag=%c\n", (int64_t)M, (int64_t)N, (int64_t)lda, (int64_t)ldb,
            order_to_char(order), side_to_char(side), uplo_to_char(uplo), transpose_to_char(transa), diag_to_char(diag));
    printf("           alpha=%g\n", alpha);

    nvpl_int_t rowsA = (side == CblasLeft) ? M : N;
    nvpl_int_t colsA = rowsA;
    nvpl_int_t rowsB = M;
    nvpl_int_t colsB = N;

    // allocate memory
    A = (double *)malloc(lda * colsA  * sizeof(double));
    B = (double *)malloc(ldb * colsB  * sizeof(double));

    // fill data
    fill_dmatrix(A, rowsA, colsA, lda, order, UPLO_TO_FILL_MODE(uplo), diag);
    fill_dmatrix(B, rowsB, colsB, ldb, order, Full, CblasNonUnit);

    // print input data
    print_dmatrix(A, rowsA, colsA, lda, "A", order);
    print_dmatrix(B, rowsB, colsB, ldb, "B", order);

    // call cblas_dtrsm
    cblas_dtrsm(order, side, uplo, transa, diag, M, N, alpha, A, lda, B, ldb);

    // print result
    printf("\nAfter dtrsm operation:\n");
    print_dmatrix(B, rowsB, colsB, ldb, "B", order);

    // release memory
    free(A);
    free(B);
    return 0;
}
