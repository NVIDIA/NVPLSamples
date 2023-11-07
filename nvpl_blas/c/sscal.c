/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_sscal
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 5;
    float * X;
    float alpha = 1.0f;
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_sscal for multiplying the vector x by the scalar\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 ", alpha=%f\n", (int64_t)N,
            (int64_t)incX, alpha);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    // allocate memory
    X = (float *)malloc(len_x * sizeof(float));

    // fill data
    fill_svector(X, N, incX);

    // print input data
    print_svector(X, N, incX, "X");

    // call cblas_sscal
    cblas_sscal(N, alpha, X, incX);

    // print result
    printf("\nThe result of cblas_sscal:\n");
    print_svector(X, N, incX, "X");

    // release memory
    free(X);
    return 0;
}
