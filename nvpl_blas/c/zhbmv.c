/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zhbmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 4;
    nvpl_int_t K = 2;
    nvpl_dcomplex_t alpha = {1.0f, 2.0f};
    nvpl_dcomplex_t beta = {1.0f, 2.0f};
    enum CBLAS_ORDER order = CblasColMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    nvpl_dcomplex_t * A = NULL;
    nvpl_dcomplex_t * X;
    nvpl_dcomplex_t * Y;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    nvpl_int_t KL = (uplo == CblasUpper) ? 0 : K;
    nvpl_int_t KU = (uplo == CblasUpper) ? K : 0;
    
    nvpl_int_t lda = K + 1;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);

    printf("\nExample: cblas_zhbmv for the Hermitian banded matrix-vector multiplication\n\n");
    printf("#### args: n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", incx=%" PRId64 ", incy=%" PRId64 
            ", uplo=%c, order=%c\n", (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)incX, (int64_t)incY,
            uplo_to_char(uplo), order_to_char(order));
    printf("           alpha=(%g, %g), beta=(%g, %g)\n", alpha.real, alpha.imag, beta.real, beta.imag);

    // allocate memory
    A = (nvpl_dcomplex_t *)malloc(lda * N * sizeof(nvpl_dcomplex_t));
    X = (nvpl_dcomplex_t *)malloc(len_x * sizeof(nvpl_dcomplex_t));
    Y = (nvpl_dcomplex_t *)malloc(len_y * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zbandmatrix(A, KL, KU, N, N, lda, order);
    fill_zvector(X, N, incX);
    fill_zvector(Y, N, incY);

    // print input data
    print_zbandmatrix(A, KL, KU, N, N, lda, "A", order);
    print_zvector(X, N, incX, "X");
    print_zvector(Y, N, incY, "Y");

    // call cblas_zhbmv
    cblas_zhbmv(order, uplo, N, K, (void *)&alpha, A, lda, X, incX, (void *)&beta, Y, incY);

    // print result
    printf("\nAfter zhbmv operation:\n");
    print_zvector(Y, N, incY, "Y");

    // release memory
    free(A);
    free(X);
    free(Y);
    return 0;
}
