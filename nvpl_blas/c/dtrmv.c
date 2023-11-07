/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dtrmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t lda = 2;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA = CblasNoTrans;
    enum CBLAS_UPLO uplo = CblasUpper;
    enum CBLAS_DIAG diag = CblasNonUnit;
    double * A = NULL;
    double * X;
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_dtrmv for the triangular matrix-vector multiplication\n\n");
    printf("#### args: n=%" PRId64 ", lda=%" PRId64 ", incx=%" PRId64 ", transA=%c, order=%c, uplo=%c"
            ", diag=%c\n", (int64_t)N, (int64_t)lda, (int64_t)incX, transpose_to_char(transA), 
            order_to_char(order), uplo_to_char(uplo), diag_to_char(diag));

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);

    // allocate memory
    A = (double *)malloc(lda * N * sizeof(double));
    X = (double *)malloc(len_x * sizeof(double));

    // fill data
    fill_dmatrix(A, N, N, lda, order, UPLO_TO_FILL_MODE(uplo), diag);
    fill_dvector(X, N, incX);

    // print input data
    print_dmatrix(A, N, N, lda, "A", order);
    print_dvector(X, N, incX, "X");

    // call cblas_dtrmv
    cblas_dtrmv(order, uplo, transA, diag, N, A, lda, X, incX);

    // print result
    printf("\nAfter dtrmv operation:\n");
    print_dvector(X, N, incX, "X");

    // release memory
    free(A);
    free(X);
    return 0;
}
