/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_cscal
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    nvpl_scomplex_t * X;
    nvpl_scomplex_t alpha = {1.0f, 2.0f};
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_cscal for multiplying the vector x by the scalar\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", alpha=(%g, %g)\n", (int64_t)N,
            (int64_t)incX, alpha.real, alpha.imag);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    // allocate memory
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cvector(X, N, incX);

    // print input data
    print_cvector(X, N, incX, "X");

    // call cblas_cscal
    cblas_cscal(N, &alpha, X, incX);

    // print result
    printf("\nThe result of cblas_cscal:\n");
    print_cvector(X, N, incX, "X");

    // release memory
    free(X);
    return 0;
}
