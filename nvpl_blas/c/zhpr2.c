/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zhpr2
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 2;
    nvpl_dcomplex_t alpha = {1.0f, 2.0f};
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    nvpl_dcomplex_t * AP = NULL;
    nvpl_dcomplex_t * X = NULL;
    nvpl_dcomplex_t * Y = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);

    printf("\nExample: cblas_zhpr2 for the packed Hermitian rank-22 update\n\n");
    printf("#### args: n=%" PRId64 ", incX=%" PRId64 ", incY=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)incX, (int64_t)incY, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=(%g, %g)\n", alpha.real, alpha.imag);

    // allocate memory
    AP = (nvpl_dcomplex_t *)malloc(N * N  * sizeof(nvpl_dcomplex_t));
    X = (nvpl_dcomplex_t *)malloc(len_x * sizeof(nvpl_dcomplex_t));
    Y = (nvpl_dcomplex_t *)malloc(len_y * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zmatrix(AP, N, N, N, order, UPLO_TO_FILL_MODE(uplo), CblasUnit);
    fill_zvector(X, N, incX);
    fill_zvector(Y, N, incY);

    // print input data
    print_zmatrix(AP, N, N, N, "AP", order);
    print_zvector(X, N, incX, "X");
    print_zvector(Y, N, incY, "Y");

    // call cblas_zhpr2
    cblas_zhpr2(order, uplo, N, &alpha, X, incX, Y, incY, AP);

    // print result
    printf("\nAfter zhpr2 operation:\n");
    print_zmatrix(AP, N, N, N, "AP", order);

    // release memory
    free(AP);
    free(X);
    free(Y);
    return 0;
}
