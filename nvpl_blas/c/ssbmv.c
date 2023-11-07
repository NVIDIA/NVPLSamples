/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_ssbmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 4;
    nvpl_int_t K = 2;
    float alpha = 1.0f;
    float beta = 1.0f;
    enum CBLAS_ORDER order = CblasColMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    float * A = NULL;
    float * X;
    float * Y;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    nvpl_int_t KL = (uplo == CblasUpper) ? 0 : K;
    nvpl_int_t KU = (uplo == CblasUpper) ? K : 0;
    
    nvpl_int_t lda = K + 1;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);

    printf("\nExample: cblas_ssbmv for the symmetric banded matrix-vector multiplication\n\n");
    printf("#### args: n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 
            ", uplo=%c, order=%c\n", (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)incX, (int64_t)incY,
            uplo_to_char(uplo), order_to_char(order));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    // allocate memory
    A = (float *)malloc(lda * N * sizeof(float));
    X = (float *)malloc(len_x * sizeof(float));
    Y = (float *)malloc(len_y * sizeof(float));

    // fill data
    fill_sbandmatrix(A, KL, KU, N, N, lda, order);
    fill_svector(X, N, incX);
    fill_svector(Y, N, incY);

    // print input data
    print_sbandmatrix(A, KL, KU, N, N, lda, "A", order);
    print_svector(X, N, incX, "X");
    print_svector(Y, N, incY, "Y");

    // call cblas_ssbmv
    cblas_ssbmv(order, uplo, N, K, alpha, A, lda, X, incX, beta, Y, incY);

    // print result
    printf("\nAfter ssbmv operation:\n");
    print_svector(Y, N, incY, "Y");

    // release memory
    free(A);
    free(X);
    free(Y);
    return 0;
}
