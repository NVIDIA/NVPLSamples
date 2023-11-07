/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zdotc_sub
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    nvpl_dcomplex_t * X;
    nvpl_dcomplex_t * Y;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;
    nvpl_dcomplex_t dotc = {0.0f, 0.0f};
    printf("\nExample: cblas_zdotc_sub for computing the dot product of vectors X and Y\n\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 "\n", (int64_t)N,
            (int64_t)incX, (int64_t)incY);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);

    // allocate memory
    X = (nvpl_dcomplex_t *)malloc(len_x * sizeof(nvpl_dcomplex_t));
    Y = (nvpl_dcomplex_t *)malloc(len_y * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zvector(X, N, incX);
    fill_zvector(Y, N, incY);

    // print input data
    print_zvector(X, N, incX, "X");
    print_zvector(Y, N, incY, "Y");

    // call cblas_zdotc_sub
    cblas_zdotc_sub(N, X, incX, Y, incY, &dotc);

    // print result
    printf("\nThe dot product of vectors X and Y: (%g, %g)\n", dotc.real, dotc.imag);

    // release memory
    free(X);
    free(Y);
    return 0;
}
