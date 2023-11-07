/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zscal
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    nvpl_dcomplex_t * X;
    nvpl_dcomplex_t alpha = {1.0f, 2.0f};
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_zscal for multiplying the vector x by the scalar\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", alpha=(%g, %g)\n", (int64_t)N,
            (int64_t)incX, alpha.real, alpha.imag);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    // allocate memory
    X = (nvpl_dcomplex_t *)malloc(len_x * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zvector(X, N, incX);

    // print input data
    print_zvector(X, N, incX, "X");

    // call cblas_zscal
    cblas_zscal(N, &alpha, X, incX);

    // print result
    printf("\nThe result of cblas_zscal:\n");
    print_zvector(X, N, incX, "X");

    // release memory
    free(X);
    return 0;
}
