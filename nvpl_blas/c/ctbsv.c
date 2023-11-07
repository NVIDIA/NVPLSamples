/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_ctbsv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 4;
    nvpl_int_t K = 2;
    nvpl_int_t incX = 1;
    nvpl_int_t lda = K + 1;
    enum CBLAS_ORDER order = CblasColMajor;
    enum CBLAS_UPLO uplo = CblasUpper;
    enum CBLAS_TRANSPOSE trans = CblasNoTrans;
    enum CBLAS_DIAG diag = CblasNonUnit;
    nvpl_scomplex_t * A = NULL;
    nvpl_scomplex_t * X = NULL;
    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);

    printf("\nExample: cblas_ctbsv for solving the triangular banded linear system with a single right-hand-side\n\n");
    printf("#### args: n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", incX=%" PRId64 ", order=%c, uplo=%c, trans=%c, diag=%c\n",
           (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)incX, order_to_char(order), uplo_to_char(uplo), transpose_to_char(trans),
           diag_to_char(diag));

    // allocate memory
    A = (nvpl_scomplex_t *)malloc(lda * N * sizeof(nvpl_scomplex_t));
    X = (nvpl_scomplex_t *)malloc(len_x * sizeof(nvpl_scomplex_t));

    nvpl_int_t kl = (CblasUpper == uplo) ? 0 : K;
    nvpl_int_t ku = (CblasUpper == uplo) ? K : 0;

    // fill data
    fill_cbandmatrix(A, kl, ku, N, N, lda, order);
    fill_cvector(X, N, incX);

    // print input data
    print_cbandmatrix(A, kl, ku, N, N, lda, "A", order);
    print_cvector(X, N, incX, "X");

    // call cblas_ctbsv
    cblas_ctbsv(order, uplo, trans, diag, N, K, A, lda, X, incX);

    // print result
    printf("\nAfter ctbsv operation:\n");
    print_cvector(X, N, incX, "X");

    // release memory
    free(A);
    free(X);
    return 0;
}
