/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_chpmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 2;
    nvpl_scomplex_t alpha = {1.0f, 2.0f};
    nvpl_scomplex_t beta = {1.0f, 2.0f};
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    nvpl_scomplex_t * AP = NULL;
    nvpl_scomplex_t * X = NULL;
    nvpl_scomplex_t * Y = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);
    nvpl_int_t len_ap = (N * (N + 1)) / 2;

    printf("\nExample: cblas_chpmv for a matrix-vector product using a Hermitian packed matrix\n\n");
    printf("#### args: n=%" PRId64 ", incX=%" PRId64 ", incY=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)incX, (int64_t)incY, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=(%g, %g), beta=(%g, %g)\n", alpha.real, alpha.imag, beta.real, beta.imag);

    // allocate memory
    AP = (nvpl_scomplex_t *)malloc(len_ap  * sizeof(nvpl_scomplex_t));
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));
    Y = (nvpl_scomplex_t *)malloc(len_y * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cvector(AP, len_ap, 1);
    fill_cvector(X, N, incX);
    fill_cvector(Y, N, incY);

    // print input data
    print_cvector(AP, len_ap, 1, "AP");
    print_cvector(X, N, incX, "X");
    print_cvector(Y, N, incY, "Y");

    // call cblas_chpmv
    cblas_chpmv(order, uplo, N, &alpha, AP, X, incX, &beta, Y, incY);

    // print result
    printf("\nAfter chpmv operation:\n");
    print_cvector(Y, N, incY, "Y");

    // release memory
    free(AP);
    free(X);
    free(Y);
    return 0;
}
