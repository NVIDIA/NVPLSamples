/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_saxpby
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    float * X;
    float * Y;
    float alpha = 1.0f;
    float beta = 0.5f;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    printf("\nExample: cblas_saxpby for computing y := alpha * x + beta * y\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 ", alpha=%f, beta=%f\n", 
            (int64_t)N, (int64_t)incX, (int64_t)incY, alpha, beta);

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

    // call cblas_saxpby
    cblas_saxpby(N, alpha, X, incX, beta, Y, incY);

    // print result
    printf("\nThe result of cblas_saxpby:\n");
    print_svector(Y, N, incY, "Y");

    // release memory
    free(X);
    free(Y);
    return 0;
}
