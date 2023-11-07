/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_sger
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 2;
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 1;
    float alpha = 1.0f;
    enum CBLAS_ORDER order = CblasColMajor;
    float * A = NULL;
    float * X = NULL;
    float * Y = NULL;
    nvpl_int_t lda = (CblasColMajor == order) ? M : N;
    nvpl_int_t k = (CblasColMajor == order) ? N : M;
    nvpl_int_t len_x = 1 + (M -1) * labs(incX);
    nvpl_int_t len_y = 1 + (N -1) * labs(incY);

    printf("\nExample: cblas_sger for the rank-1 update\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", lda=%" PRId64 ", incX=%" PRId64 ", incY=%" PRId64 ", "
            "order=%c\n", (int64_t)M, (int64_t)N, (int64_t)lda, (int64_t)incX, (int64_t)incY,
            order_to_char(order));
    printf("           alpha=%g\n", alpha);

    // allocate memory
    A = (float *)malloc(lda * k  * sizeof(float));
    X = (float *)malloc(len_x * sizeof(float));
    Y = (float *)malloc(len_y * sizeof(float));

    // fill data
    fill_smatrix(A, M, N, lda, order, Full, CblasNonUnit);
    fill_svector(X, M, incX);
    fill_svector(Y, N, incY);

    // print input data
    print_smatrix(A, M, N, lda, "A", order);
    print_svector(X, M, incX, "X");
    print_svector(Y, N, incY, "Y");

    // call cblas_sger
    cblas_sger(order, M, N, alpha, X, incX, Y, incY, A, lda);

    // print result
    printf("\nAfter sger operation:\n");
    print_smatrix(A, M, N, lda, "A", order);

    // release memory
    free(A);
    free(X);
    free(Y);
    return 0;
}
