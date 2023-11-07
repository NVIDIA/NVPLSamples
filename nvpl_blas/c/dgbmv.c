/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dgbmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 3;
    nvpl_int_t N = 4;
    nvpl_int_t KL = 1;
    nvpl_int_t KU = 2;
    double alpha = 1.0f;
    double beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA = CblasNoTrans;
    double * A = NULL;
    double * X;
    double * Y;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;

    nvpl_int_t nx = (transA == CblasNoTrans) ? N : M;
    nvpl_int_t ny = (transA == CblasNoTrans) ? M : N;
    
    nvpl_int_t lda = KL + KU + 1;
    nvpl_int_t size_k = (order == CblasColMajor) ? N : M;
    nvpl_int_t len_x = 1 + (nx - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (ny - 1) * labs(incY);

    printf("\nExample: cblas_dgbmv for banded matrix-vector multiplication\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", kl=%" PRId64 ", ku=%" PRId64 ", lda=%" PRId64 ", "
            "incx=%" PRId64 ", incy=%" PRId64 ", transA=%c, order=%c\n", (int64_t)M, (int64_t)N, (int64_t)KL,
           (int64_t)KU, (int64_t)lda, (int64_t)incX, (int64_t)incY, transpose_to_char(transA),
           order_to_char(order));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    // allocate memory
    A = (double *)malloc(lda * size_k * sizeof(double));
    X = (double *)malloc(len_x * sizeof(double));
    Y = (double *)malloc(len_y * sizeof(double));

    // fill data
    fill_dbandmatrix(A, KL, KU, M, N, lda, order);
    fill_dvector(X, nx, incX);
    fill_dvector(Y, ny, incY);

    // print input data
    print_dbandmatrix(A, KL, KU, M, N, lda, "A", order);
    print_dvector(X, nx, incX, "X");
    print_dvector(Y, ny, incY, "Y");

    // call cblas_dgbmv
    cblas_dgbmv(order, transA, M, N, KL, KU, alpha, A, lda, X, incX, beta, Y, incY);

    // print result
    printf("\nAfter dgbmv operation:\n");
    print_dvector(Y, ny, incY, "Y");

    // release memory
    free(A);
    free(X);
    free(Y);
    return 0;
}
