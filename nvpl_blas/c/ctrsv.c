/******************************************************************************
 * Content: 
 *     This example demonctrates use of API as below:
 *     cblas_ctrsv
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
    nvpl_scomplex_t * A = NULL;
    nvpl_scomplex_t * X = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);

    printf("\nExample: cblas_ctrsv for solving a system of linear equations whose coefficients are in a triangular matrix\n\n");
    printf("#### args: n=%" PRId64 ", lda=%" PRId64 ", incX=%" PRId64 ", order=%c, uplo=%c, trans=%c, diag=%c\n",
           (int64_t)N, (int64_t)lda, (int64_t)incX, order_to_char(order), uplo_to_char(uplo), transpose_to_char(trans),
           diag_to_char(diag));

    // allocate memory
    A = (nvpl_scomplex_t *)malloc(lda * N * sizeof(nvpl_scomplex_t));
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cmatrix(A, N, N, lda, order, UPLO_TO_FILL_MODE(uplo), diag);
    fill_cvector(X, N, incX);

    // print input data
    print_cmatrix(A, N, N, lda, "A", order);
    print_cvector(X, N, incX, "X");

    // call cblas_ctrsv
    cblas_ctrsv(order, uplo, trans, diag, N, A, lda, X, incX);

    // print result
    printf("\nAfter ctrsv operation:\n");
    print_cvector(X, N, incX, "X");

    // release memory
    free(A);
    free(X);
    return 0;
}
