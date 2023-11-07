/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_cgerc
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 2;
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;
    nvpl_scomplex_t alpha = {1.0f, 2.0f};
    enum CBLAS_ORDER order = CblasRowMajor;
    nvpl_scomplex_t * A = NULL;
    nvpl_scomplex_t * X = NULL;
    nvpl_scomplex_t * Y = NULL;
    nvpl_int_t lda = (CblasColMajor == order) ? M : N;
    nvpl_int_t k = (CblasColMajor == order) ? N : M;
    nvpl_int_t len_x = 1 + (M -1) * labs(incX);
    nvpl_int_t len_y = 1 + (N -1) * labs(incY);

    printf("\nExample: cblas_cgerc for the rank-1 update\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", lda=%" PRId64 ", incX=%" PRId64 ", incY=%" PRId64 ", "
            "order=%c\n", (int64_t)M, (int64_t)N, (int64_t)lda, (int64_t)incX, (int64_t)incY,
            order_to_char(order));
    printf("           alpha=(%g, %g)\n", alpha.real, alpha.imag);

    // allocate memory
    A = (nvpl_scomplex_t *)malloc(lda * k  * sizeof(nvpl_scomplex_t));
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));
    Y = (nvpl_scomplex_t *)malloc(len_y * sizeof(nvpl_scomplex_t));

    // fill data
    fill_cmatrix(A, M, N, lda, order, Full, CblasNonUnit);
    fill_cvector(X, M, incX);
    fill_cvector(Y, N, incY);

    // print input data
    print_cmatrix(A, M, N, lda, "A", order);
    print_cvector(X, M, incX, "X");
    print_cvector(Y, N, incY, "Y");

    // call cblas_cgerc
    cblas_cgerc(order, M, N, &alpha, X, incX, Y, incY, A, lda);

    // print result
    printf("\nAfter cgerc operation:\n");
    print_cmatrix(A, M, N, lda, "A", order);

    // release memory
    free(A);
    free(X);
    free(Y);
    return 0;
}
