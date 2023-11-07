/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_ssyr
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t lda = N;
    nvpl_int_t incX = 1;
    float alpha = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    float * A = NULL;
    float * X = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);

    printf("\nExample: cblas_ssyr for a rank-1 update of a symmetric matrix\n\n");
    printf("#### args: n=%" PRId64 ", lda=%" PRId64 ", incX=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)lda, (int64_t)incX, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=%g\n", alpha);

    // allocate memory
    A = (float *)malloc(lda * N  * sizeof(float));
    X = (float *)malloc(len_x * sizeof(float));

    // fill data
    fill_smatrix(A, N, N, lda, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);
    fill_svector(X, N, incX);

    // print input data
    print_smatrix(A, N, N, lda, "A", order);
    print_svector(X, N, incX, "X");

    // call cblas_ssyr
    cblas_ssyr(order, uplo, N, alpha, X, incX, A, lda);

    // print result
    printf("\nAfter ssyr operation:\n");
    print_smatrix(A, N, N, lda, "A", order);

    // release memory
    free(A);
    free(X);
    return 0;
}
