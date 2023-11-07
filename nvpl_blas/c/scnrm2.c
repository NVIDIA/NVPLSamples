/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_scnrm2
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    float result;
    nvpl_int_t N = 5;
    nvpl_scomplex_t * X;
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_scnrm2 for computing the Euclidean norm of the vector\n\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 "\n", (int64_t)N, (int64_t)incX);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    // allocate memory
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cvector(X, N, incX);

    // print input data
    print_cvector(X, N, incX, "X");

    // call cblas_scnrm2
    result = cblas_scnrm2(N, X, incX);

    // print result
    printf("\nThe Euclidean norm of the vector: %f\n", result);

    // release memory
    free(X);
    return 0;
}
