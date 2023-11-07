/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_csscal
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    nvpl_scomplex_t * X;
    float alpha = 1.0f;
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_csscal for multiplying the vector x by the scalar\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", alpha=%f\n", (int64_t)N,
            (int64_t)incX, alpha);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    // allocate memory
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cvector(X, N, incX);

    // print input data
    print_cvector(X, N, incX, "X");

    // call cblas_csscal
    cblas_csscal(N, alpha, X, incX);

    // print result
    printf("\nThe result of cblas_csscal:\n");
    print_cvector(X, N, incX, "X");

    // release memory
    free(X);
    return 0;
}
