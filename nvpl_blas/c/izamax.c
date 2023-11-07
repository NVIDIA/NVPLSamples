/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_izamax
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    CBLAS_INDEX result;
    nvpl_int_t N = 5;
    nvpl_dcomplex_t * X;
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_izamax for finding the (smallest) index of the element of the maximum\n\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 "\n", (int64_t)N, (int64_t)incX);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    // allocate memory
    X = (nvpl_dcomplex_t *)malloc(len_x * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zvector(X, N, incX);

    // print input data
    print_zvector(X, N, incX, "X");

    // call cblas_isamax
    result = cblas_izamax(N, X, incX);

    // print result
    printf("\nThe (smallest) index of the element of the maximum magnitude: %zu\n", result);

    // release memory
    free(X);
    return 0;
}
