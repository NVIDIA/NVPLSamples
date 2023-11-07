/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zher2
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t lda = N;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;
    nvpl_dcomplex_t alpha = {1.0f, 2.0f};
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    nvpl_dcomplex_t * A = NULL;
    nvpl_dcomplex_t * X = NULL;
    nvpl_dcomplex_t * Y = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);

    printf("\nExample: cblas_zher2 for the Hermitian rank-2 update\n\n");
    printf("#### args: n=%" PRId64 ", lda=%" PRId64 ", incX=%" PRId64 ", incY=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)lda, (int64_t)incX, (int64_t)incY, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=(%g, %g)\n", alpha.real, alpha.imag);

    // allocate memory
    A = (nvpl_dcomplex_t *)malloc(lda * N  * sizeof(nvpl_dcomplex_t));
    X = (nvpl_dcomplex_t *)malloc(len_x * sizeof(nvpl_dcomplex_t));
    Y = (nvpl_dcomplex_t *)malloc(len_y * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zmatrix(A, N, N, lda, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);
    fill_zvector(X, N, incX);
    fill_zvector(Y, N, incY);

    // print input data
    print_zmatrix(A, N, N, lda, "A", order);
    print_zvector(X, N, incX, "X");
    print_zvector(Y, N, incY, "Y");

    // call cblas_zher2
    cblas_zher2(order, uplo, N, &alpha, X, incX, Y, incY, A, lda);

    // print result
    printf("\nAfter zher2 operation:\n");
    print_zmatrix(A, N, N, lda, "A", order);

    // release memory
    free(A);
    free(X);
    free(Y);
    return 0;
}
