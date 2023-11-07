/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_chpr
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    float alpha = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    nvpl_scomplex_t * A = NULL;
    nvpl_scomplex_t * X = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);

    printf("\nExample: cblas_chpr for the Hermitian rank-1 update\n\n");
    printf("#### args: n=%" PRId64 ", incX=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)incX, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=%g\n", alpha);

    // allocate memory
    A = (nvpl_scomplex_t *)malloc(N * N  * sizeof(nvpl_scomplex_t));
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cmatrix(A, N, N, N, order, UPLO_TO_FILL_MODE(uplo), CblasUnit);
    fill_cvector(X, N, incX);

    // print input data
    print_cmatrix(A, N, N, N, "A", order);
    print_cvector(X, N, incX, "X");

    // call cblas_chpr
    cblas_chpr(order, uplo, N, alpha, X, incX, A);

    // print result
    printf("\nAfter chpr operation:\n");
    print_cmatrix(A, N, N, N, "A", order);

    // release memory
    free(A);
    free(X);
    return 0;
}
