/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_cswap
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    nvpl_scomplex_t * X;
    nvpl_scomplex_t * Y;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    printf("\nExample: cblas_cswap for interchanging the elements of vector x and y\n\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 "\n", (int64_t)N,
            (int64_t)incX, (int64_t)incY);

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

    // call cblas_cswap
    cblas_cswap(N, X, incX, Y, incY);

    // print result
    printf("\nThe result of interchanging the elements of vector x and y:\n");
    print_cvector(X, N, incX, "X");
    print_cvector(Y, N, incY, "Y");

    // release memory
    free(X);
    free(Y);
    return 0;
}
