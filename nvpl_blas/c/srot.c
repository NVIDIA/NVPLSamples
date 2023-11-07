/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_srot
 *
 ******************************************************************************/
#include "example_helper.h"
#include <math.h> // sqrt

int main() {
    nvpl_int_t N = 5;
    float * X;
    float * Y;
    float s = 0.5f;
    float c = sqrt(1 - s * s);
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    printf("\nExample: cblas_srot for applying Givens rotation matrix\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 ", c=%f, s=%f\n", (int64_t)N,
            (int64_t)incX, (int64_t)incY, c, s);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);
    // allocate memory
    X = (float *)malloc(len_x * sizeof(float));
    Y = (float *)malloc(len_y * sizeof(float));

    // fill data
    fill_svector(X, N, incX);
    fill_svector(Y, N, incY);

    // print input data
    print_svector(X, N, incX, "X");
    print_svector(Y, N, incY, "Y");

    // call cblas_srot
    cblas_srot(N, X, incX, Y, incY, c, s);

    // print result
    printf("\nThe result of cblas_srot:\n");
    print_svector(X, N, incX, "X");
    print_svector(Y, N, incY, "Y");

    // release memory
    free(X);
    free(Y);
    return 0;
}
