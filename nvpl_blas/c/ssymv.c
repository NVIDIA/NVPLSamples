/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_ssymv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 2;
    float alpha = 1.0;
    float beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    float * A = NULL;
    float * X = NULL;
    float * Y = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);

    printf("\nExample: cblas_ssymv for computing a matrix-vector product for a symmetric matrix.\n\n");
    printf("#### args: n=%" PRId64 ", lda=%" PRId64 ", incX=%" PRId64 ", incY=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)lda, (int64_t)incX, (int64_t)incY, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    // allocate memory
    A = (float *)malloc(lda * N  * sizeof(float));
    X = (float *)malloc(len_x * sizeof(float));
    Y = (float *)malloc(len_y * sizeof(float));

    // fill data
    fill_smatrix(A, N, N, lda, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);
    fill_svector(X, N, incX);
    fill_svector(Y, N, incY);

    // print input data
    print_smatrix(A, N, N, lda, "A", order);
    print_svector(X, N, incX, "X");
    print_svector(Y, N, incY, "Y");

    // call cblas_ssymv
    cblas_ssymv(order, uplo, N, alpha, A, lda, X, incX, beta, Y, incY);

    // print result
    printf("\nAfter ssymv operation:\n");
    print_svector(Y, N, incY, "Y");

    // release memory
    free(A);
    free(X);
    free(Y);
    return 0;
}
