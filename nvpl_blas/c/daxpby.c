/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_daxpby
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    double * X;
    double * Y;
    double alpha = 1.0f;
    double beta = 0.5f;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    printf("\nExample: cblas_daxpby for computing y := alpha * x + beta * y\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 ", alpha=%f, beta=%f\n", 
            (int64_t)N, (int64_t)incX, (int64_t)incY, alpha, beta);

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

    // call cblas_daxpby
    cblas_daxpby(N, alpha, X, incX, beta, Y, incY);

    // print result
    printf("\nThe result of cblas_daxpby:\n");
    print_dvector(Y, N, incY, "Y");

    // release memory
    free(X);
    free(Y);
    return 0;
}
