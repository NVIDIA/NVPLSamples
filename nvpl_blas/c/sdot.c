/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_sdot
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    float result;
    nvpl_int_t N = 5;
    float * X;
    float * Y;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    printf("\nExample: cblas_sdot for computing the dot product of vectors X and Y\n\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 "\n", (int64_t)N,
            (int64_t)incX, (int64_t)incY);

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

    // call cblas_sdot
    result = cblas_sdot(N, X, incX, Y, incY);

    // print result
    printf("\nThe dot product of vectors X and Y: %f\n", result);

    // release memory
    free(X);
    free(Y);
    return 0;
}
