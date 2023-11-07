/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_sspr
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    float alpha = 1.0f;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    float * AP = NULL;
    float * X = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_ap = (N * (N + 1)) / 2;

    printf("\nExample: cblas_sspr for a rank-1 update of a symmetric packed matrix\n\n");
    printf("#### args: n=%" PRId64 ", incX=%" PRId64 ", order=%c, uplo=%c\n",
           (int64_t)N, (int64_t)incX, order_to_char(order), uplo_to_char(uplo));
    printf("           alpha=%g\n", alpha);

    // allocate memory
    AP = (float *)malloc(len_ap * sizeof(float));
    X = (float *)malloc(len_x * sizeof(float));

    // fill data
    fill_svector(AP, len_ap, 1);
    fill_svector(X, N, incX);

    // print input data
    print_svector(AP, len_ap, 1, "AP");
    print_svector(X, N, incX, "X");

    // call cblas_sspr
    cblas_sspr(order, uplo, N, alpha, X, incX, AP);

    // print result
    printf("\nAfter sspr operation:\n");
    print_svector(AP, len_ap, 1, "AP");

    // release memory
    free(AP);
    free(X);
    return 0;
}
