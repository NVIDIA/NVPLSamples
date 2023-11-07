/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_snrm2
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    float result;
    nvpl_int_t N = 5;
    float * X;
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_snrm2 for computing the Euclidean norm of the vector\n\n");
    printf("#### args: n=%" PRId64 ", incx=%" PRId64 "\n", (int64_t)N, (int64_t)incX);

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    // allocate memory
    X = (float *)malloc(len_x * sizeof(float));

    // fill data
    fill_svector(X, N, incX);

    // print input data
    print_svector(X, N, incX, "X");

    // call cblas_snrm2
    result = cblas_snrm2(N, X, incX);

    // print result
    printf("\nThe Euclidean norm of the vector: %f\n", result);

    // release memory
    free(X);
    return 0;
}
