/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dspr
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    double alpha = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    double * AP = NULL;
    double * X = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_ap = (N * (N + 1)) / 2;

    printf("\nExample: cblas_dspr for a rank-1 update of a symmetric packed matrix\n\n");
    printf("#### args: n=%" PRId64 ", incX=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)incX, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=%g\n", alpha);

    // allocate memory
    AP = (double *)malloc(len_ap * sizeof(double));
    X = (double *)malloc(len_x * sizeof(double));

    // fill data
    fill_dvector(AP, len_ap, 1);
    fill_dvector(X, N, incX);

    // print input data
    print_dvector(AP, len_ap, 1, "AP");
    print_dvector(X, N, incX, "X");

    // call cblas_dspr
    cblas_dspr(order, uplo, N, alpha, X, incX, AP);

    // print result
    printf("\nAfter dspr operation:\n");
    print_dvector(AP, len_ap, 1, "AP");

    // release memory
    free(AP);
    free(X);
    return 0;
}
