/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zdscal
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    nvpl_dcomplex_t * X;
    double alpha = 1.0f;
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_zdscal for multiplying the vector x by the scalar\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", alpha=%f\n", (int64_t)N,
            (int64_t)incX, alpha);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    // allocate memory
    X = (nvpl_dcomplex_t *)malloc(len_x * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zvector(X, N, incX);

    // print input data
    print_zvector(X, N, incX, "X");

    // call cblas_zdscal
    cblas_zdscal(N, alpha, X, incX);

    // print result
    printf("\nThe result of cblas_zdscal:\n");
    print_zvector(X, N, incX, "X");

    // release memory
    free(X);
    return 0;
}
