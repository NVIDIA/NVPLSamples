/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_caxpby
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    nvpl_scomplex_t * X;
    nvpl_scomplex_t * Y;
    nvpl_scomplex_t alpha = {1.0f, 2.0f};
    nvpl_scomplex_t beta  = {2.0f, 1.0f};
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    printf("\nExample: cblas_caxpby for computing y := alpha * x + beta * y\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 ", alpha=(%g, %g), beta=(%g, %g)\n", 
            (int64_t)N, (int64_t)incX, (int64_t)incY, alpha.real, alpha.imag, beta.real, beta.imag);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);
    // allocate memory
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));
    Y = (nvpl_scomplex_t *)malloc(len_y * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cvector(X, N, incX);
    fill_cvector(Y, N, incY);

    // print input data
    print_cvector(X, N, incX, "X");
    print_cvector(Y, N, incY, "Y");

    // call cblas_caxpby
    cblas_caxpby(N, &alpha, X, incX, &beta, Y, incY);

    // print result
    printf("\nThe result of cblas_caxpby:\n");
    print_cvector(Y, N, incY, "Y");

    // release memory
    free(X);
    free(Y);
    return 0;
}
