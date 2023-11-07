/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_cgbmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 3;
    nvpl_int_t N = 4;
    nvpl_int_t KL = 1;
    nvpl_int_t KU = 2;
    nvpl_scomplex_t alpha = {1.0f, 2.0f};
    nvpl_scomplex_t beta = {1.0f, 2.0f};
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA = CblasNoTrans;
    nvpl_scomplex_t * A = NULL;
    nvpl_scomplex_t * X;
    nvpl_scomplex_t * Y;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    nvpl_int_t nx = (transA == CblasNoTrans) ? N : M;
    nvpl_int_t ny = (transA == CblasNoTrans) ? M : N;
    
    nvpl_int_t lda = KL + KU + 1;
    nvpl_int_t size_k = (order == CblasColMajor) ? N : M;
    nvpl_int_t len_x = 1 + (nx - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (ny - 1) * labs(incY);

    printf("\nExample: cblas_cgbmv for banded matrix-vector multiplication\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", kl=%" PRId64 ", ku=%" PRId64 ", lda=%" PRId64 ", "
            "incx=%" PRId64 ", incy=%" PRId64 ", transA=%c, order=%c\n", (int64_t)M, (int64_t)N, (int64_t)KL,
           (int64_t)KU, (int64_t)lda, (int64_t)incX, (int64_t)incY, transpose_to_char(transA),
           order_to_char(order));
    printf("           alpha=(%g, %g), beta=(%g, %g)\n", alpha.real, alpha.imag, beta.real, beta.imag);

    // allocate memory
    A = (nvpl_scomplex_t *)malloc(lda * size_k * sizeof(nvpl_scomplex_t));
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));
    Y = (nvpl_scomplex_t *)malloc(len_y * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cbandmatrix(A, KL, KU, M, N, lda, order);
    fill_cvector(X, nx, incX);
    fill_cvector(Y, ny, incY);

    // print input data
    print_cbandmatrix(A, KL, KU, M, N, lda, "A", order);
    print_cvector(X, nx, incX, "X");
    print_cvector(Y, ny, incY, "Y");

    // call cblas_cgbmv
    cblas_cgbmv(order, transA, M, N, KL, KU, (void *)&alpha, A, lda, X, incX, (void *)&beta, Y, incY);

    // print result
    printf("\nAfter cgbmv operation:\n");
    print_cvector(Y, ny, incY, "Y");

    // release memory
    free(A);
    free(X);
    free(Y);
    return 0;
}
