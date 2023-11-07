/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_drotm
 *
 ******************************************************************************/
#include "example_helper.h"
#include <math.h> // sqrt

int main() {
    nvpl_int_t N = 5;
    double * X;
    double * Y;
    double P[5] = {1.0f, 0.25f, -1.0f, 1.0f, 0.5f};
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    printf("\nExample: cblas_drotm for applying Givens rotation matrix\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 "\n", (int64_t)N,
            (int64_t)incX, (int64_t)incY);
    print_dvector(P, 5, 1, "P");

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

    // call cblas_drotm
    cblas_drotm(N, X, incX, Y, incY, P);

    // print result
    printf("\nThe result of cblas_drotm:\n");
    print_dvector(X, N, incX, "X");
    print_dvector(Y, N, incY, "Y");

    // release memory
    free(X);
    free(Y);
    return 0;
}
