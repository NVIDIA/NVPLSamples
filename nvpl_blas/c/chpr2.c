/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_chpr2
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 2;
    nvpl_scomplex_t alpha = {1.0f, 2.0f};
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    nvpl_scomplex_t * AP = NULL;
    nvpl_scomplex_t * X = NULL;
    nvpl_scomplex_t * Y = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);

    printf("\nExample: cblas_chpr2 for the packed Hermitian rank-2 update\n\n");
    printf("#### args: n=%" PRId64 ", incX=%" PRId64 ", incY=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)incX, (int64_t)incY, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=(%g, %g)\n", alpha.real, alpha.imag);

    // allocate memory
    AP = (nvpl_scomplex_t *)malloc(N * N  * sizeof(nvpl_scomplex_t));
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));
    Y = (nvpl_scomplex_t *)malloc(len_y * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cmatrix(AP, N, N, N, order, UPLO_TO_FILL_MODE(uplo), CblasUnit);
    fill_cvector(X, N, incX);
    fill_cvector(Y, N, incY);

    // print input data
    print_cmatrix(AP, N, N, N, "AP", order);
    print_cvector(X, N, incX, "X");
    print_cvector(Y, N, incY, "Y");

    // call cblas_chpr2
    cblas_chpr2(order, uplo, N, &alpha, X, incX, Y, incY, AP);

    // print result
    printf("\nAfter chpr2 operation:\n");
    print_cmatrix(AP, N, N, N, "AP", order);

    // release memory
    free(AP);
    free(X);
    free(Y);
    return 0;
}
