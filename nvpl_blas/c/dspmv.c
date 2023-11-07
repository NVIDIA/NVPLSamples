/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dspmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t incY = 2;
    double alpha = 1.0f;
    double beta = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    double * AP = NULL;
    double * X = NULL;
    double * Y = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_y = 1 + (N - 1) * labs(incY);
    nvpl_int_t len_ap = (N * (N + 1)) / 2;

    printf("\nExample: cblas_dspmv for a matrix-vector product with a symmetric packed matrix.\n\n");
    printf("#### args: n=%" PRId64 ", incX=%" PRId64 ", incY=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)incX, (int64_t)incY, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    // allocate memory
    AP = (double *)malloc(len_ap  * sizeof(double));
    X = (double *)malloc(len_x * sizeof(double));
    Y = (double *)malloc(len_y * sizeof(double));

    // fill data
    fill_dvector(AP, len_ap, 1);
    fill_dvector(X, N, incX);
    fill_dvector(Y, N, incY);

    // print input data
    print_dvector(AP, len_ap, 1, "AP");
    print_dvector(X, N, incX, "X");
    print_dvector(Y, N, incY, "Y");

    // call cblas_dspmv
    cblas_dspmv(order, uplo, N, alpha, AP, X, incX, beta, Y, incY);

    // print result
    printf("\nAfter dspmv operation:\n");
    print_dvector(Y, N, incY, "Y");

    // release memory
    free(AP);
    free(X);
    free(Y);
    return 0;
}
