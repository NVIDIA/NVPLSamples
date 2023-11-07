/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dswap
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    double * X;
    double * Y;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    printf("\nExample: cblas_dswap for interchanging the elements of vector x and y\n\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 "\n", (int64_t)N,
            (int64_t)incX, (int64_t)incY);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);
    // allocate memory
    X = (double *)malloc(len_x * sizeof(double));
    Y = (double *)malloc(len_y * sizeof(double));

    // fill data
    fill_dvector(X, N, incX);
    fill_dvector(Y, N, incY);

    // print input data
    print_dvector(X, N, incX, "X");
    print_dvector(Y, N, incY, "Y");

    // call cblas_dswap
    cblas_dswap(N, X, incX, Y, incY);

    // print result
    printf("\nThe result of interchanging the elements of vector x and y:\n");
    print_dvector(X, N, incX, "X");
    print_dvector(Y, N, incY, "Y");

    // release memory
    free(X);
    free(Y);
    return 0;
}
