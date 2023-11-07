/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dscal
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    double * X;
    double alpha = 1.0f;
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_dscal for multiplying the vector x by the scalar\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", alpha=%f\n", (int64_t)N,
            (int64_t)incX, alpha);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    // allocate memory
    X = (double *)malloc(len_x * sizeof(double));

    // fill data
    fill_dvector(X, N, incX);

    // print input data
    print_dvector(X, N, incX, "X");

    // call cblas_dscal
    cblas_dscal(N, alpha, X, incX);

    // print result
    printf("\nThe result of cblas_dscal:\n");
    print_dvector(X, N, incX, "X");

    // release memory
    free(X);
    return 0;
}
