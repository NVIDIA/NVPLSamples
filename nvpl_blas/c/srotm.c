/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_srotm
 *
 ******************************************************************************/
#include "example_helper.h"
#include <math.h> // sqrt

int main() {
    nvpl_int_t N = 5;
    float * X;
    float * Y;
    float P[5] = {1.0f, 0.25f, -1.0f, 1.0f, 0.5f};
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    printf("\nExample: cblas_srotm for applying Givens rotation matrix\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 "\n", (int64_t)N,
            (int64_t)incX, (int64_t)incY);
    print_svector(P, 5, 1, "P");

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

    // call cblas_srotm
    cblas_srotm(N, X, incX, Y, incY, P);

    // print result
    printf("\nThe result of cblas_srotm:\n");
    print_svector(X, N, incX, "X");
    print_svector(Y, N, incY, "Y");

    // release memory
    free(X);
    free(Y);
    return 0;
}
