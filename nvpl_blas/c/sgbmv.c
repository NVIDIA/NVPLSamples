/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_sgbmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 3;
    nvpl_int_t N = 4;
    nvpl_int_t KL = 1;
    nvpl_int_t KU = 2;
    float alpha = 1.0f;
    float beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA = CblasNoTrans;
    float * A = NULL;
    float * X;
    float * Y;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    nvpl_int_t nx = (transA == CblasNoTrans) ? N : M;
    nvpl_int_t ny = (transA == CblasNoTrans) ? M : N;
    
    nvpl_int_t lda = KL + KU + 1;
    nvpl_int_t size_k = (order == CblasColMajor) ? N : M;
    nvpl_int_t len_x = 1 + (nx - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (ny - 1) * labs(incY);

    printf("\nExample: cblas_sgbmv for banded matrix-vector multiplication\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", kl=%" PRId64 ", ku=%" PRId64 ", lda=%" PRId64 ", "
            "incx=%" PRId64 ", incy=%" PRId64 ", transA=%c, order=%c\n", (int64_t)M, (int64_t)N, (int64_t)KL,
           (int64_t)KU, (int64_t)lda, (int64_t)incX, (int64_t)incY, transpose_to_char(transA),
           order_to_char(order));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    // allocate memory
    A = (float *)malloc(lda * size_k * sizeof(float));
    X = (float *)malloc(len_x * sizeof(float));
    Y = (float *)malloc(len_y * sizeof(float));

    // fill data
    fill_sbandmatrix(A, KL, KU, M, N, lda, order);
    fill_svector(X, nx, incX);
    fill_svector(Y, ny, incY);

    // print input data
    print_sbandmatrix(A, KL, KU, M, N, lda, "A", order);
    print_svector(X, nx, incX, "X");
    print_svector(Y, ny, incY, "Y");

    // call cblas_sgbmv
    cblas_sgbmv(order, transA, M, N, KL, KU, alpha, A, lda, X, incX, beta, Y, incY);

    // print result
    printf("\nAfter sgbmv operation:\n");
    print_svector(Y, ny, incY, "Y");

    // release memory
    free(A);
    free(X);
    free(Y);
    return 0;
}
