/******************************************************************************
 * Content: 
 *     This example demondtrates use of API as below:
 *     cblas_dtrsv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t lda = N;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    enum CBLAS_TRANSPOSE trans = CblasNoTrans;
    enum CBLAS_DIAG diag = CblasNonUnit;
    double * A = NULL;
    double * X = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);

    printf("\nExample: cblas_dtrsv for solving a system of linear equations whose coefficients are in a triangular matrix\n\n");
    printf("#### args: n=%" PRId64 ", lda=%" PRId64 ", incX=%" PRId64 ", order=%c, uplo=%c, trans=%c, diag=%c\n",
           (int64_t)N, (int64_t)lda, (int64_t)incX, order_to_char(order), uplo_to_char(uplo), transpose_to_char(trans),
           diag_to_char(diag));

    // allocate memory
    A = (double *)malloc(lda * N * sizeof(double));
    X = (double *)malloc(len_x * sizeof(double));

    // fill data
    fill_dmatrix(A, N, N, lda, order, UPLO_TO_FILL_MODE(uplo), diag);
    fill_dvector(X, N, incX);

    // print input data
    print_dmatrix(A, N, N, lda, "A", order);
    print_dvector(X, N, incX, "X");

    // call cblas_dtrsv
    cblas_dtrsv(order, uplo, trans, diag, N, A, lda, X, incX);

    // print result
    printf("\nAfter dtrsv operation:\n");
    print_dvector(X, N, incX, "X");

    // release memory
    free(A);
    free(X);
    return 0;
}
