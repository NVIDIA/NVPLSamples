/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dtpmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t incX = 1;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    enum CBLAS_TRANSPOSE trans = CblasNoTrans;
    enum CBLAS_DIAG diag = CblasNonUnit;
    double * AP = NULL;
    double * X = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);
    nvpl_int_t len_ap = (N * (N + 1)) / 2;

    printf("\nExample: cblas_dtpmv for a matrix-vector product using a triangular packed matrix\n\n");
    printf("#### args: n=%" PRId64 ", incX=%" PRId64 ", order=%c, uplo=%c, trans=%c, diag=%c\n",
           (int64_t)N, (int64_t)incX, order_to_char(order), uplo_to_char(uplo), transpose_to_char(trans),
           diag_to_char(diag));

    // allocate memory
    AP = (double *)malloc(len_ap * sizeof(double));
    X = (double *)malloc(len_x * sizeof(double));

    // fill data
    fill_dvector(AP, len_ap, 1);
    fill_dvector(X, N, incX);

    // print input data
    print_dvector(AP, len_ap, 1, "AP");
    print_dvector(X, N, incX, "X");

    // call cblas_dtpmv
    cblas_dtpmv(order, uplo, trans, diag, N, AP, X, incX);

    // print result
    printf("\nAfter dtpmv operation:\n");
    print_dvector(X, N, incX, "X");

    // release memory
    free(AP);
    free(X);
    return 0;
}
