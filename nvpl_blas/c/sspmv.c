/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_sspmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 2;
    float alpha = 1.0f;
    float beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    float * AP = NULL;
    float * X = NULL;
    float * Y = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);
    nvpl_int_t len_ap = (N * (N + 1)) / 2;

    printf("\nExample: cblas_sspmv for a matrix-vector product with a symmetric packed matrix.\n\n");
    printf("#### args: n=%" PRId64 ", incX=%" PRId64 ", incY=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)incX, (int64_t)incY, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    // allocate memory
    AP = (float *)malloc(len_ap  * sizeof(float));
    X = (float *)malloc(len_x * sizeof(float));
    Y = (float *)malloc(len_y * sizeof(float));

    // fill data
    fill_svector(AP, len_ap, 1);
    fill_svector(X, N, incX);
    fill_svector(Y, N, incY);

    // print input data
    print_svector(AP, len_ap, 1, "AP");
    print_svector(X, N, incX, "X");
    print_svector(Y, N, incY, "Y");

    // call cblas_sspmv
    cblas_sspmv(order, uplo, N, alpha, AP, X, incX, beta, Y, incY);

    // print result
    printf("\nAfter sspmv operation:\n");
    print_svector(Y, N, incY, "Y");

    // release memory
    free(AP);
    free(X);
    free(Y);
    return 0;
}
