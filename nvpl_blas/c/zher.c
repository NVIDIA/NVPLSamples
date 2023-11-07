/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zher
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t lda = N;
    nvpl_int_t incX = 1;
    double alpha = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    nvpl_dcomplex_t * A = NULL;
    nvpl_dcomplex_t * X = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);

    printf("\nExample: cblas_zher for the Hermitian rank-1 update\n\n");
    printf("#### args: n=%" PRId64 ", lda=%" PRId64 ", incX=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)lda, (int64_t)incX, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=%g\n", alpha);

    // allocate memory
    A = (nvpl_dcomplex_t *)malloc(lda * N  * sizeof(nvpl_dcomplex_t));
    X = (nvpl_dcomplex_t *)malloc(len_x * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zmatrix(A, N, N, lda, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);
    fill_zvector(X, N, incX);

    // print input data
    print_zmatrix(A, N, N, lda, "A", order);
    print_zvector(X, N, incX, "X");

    // call cblas_zher
    cblas_zher(order, uplo, N, alpha, X, incX, A, lda);

    // print result
    printf("\nAfter zher operation:\n");
    print_zmatrix(A, N, N, lda, "A", order);

    // release memory
    free(A);
    free(X);
    return 0;
}
