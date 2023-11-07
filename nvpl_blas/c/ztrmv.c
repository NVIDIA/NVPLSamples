/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_ztrmv
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t lda = 2;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA = CblasNoTrans;
    enum CBLAS_UPLO uplo = CblasUpper;
    enum CBLAS_DIAG diag = CblasNonUnit;
    nvpl_dcomplex_t * A = NULL;
    nvpl_dcomplex_t * X;
    nvpl_int_t incX = 1;

    printf("\nExample: cblas_ztrmv for the triangular matrix-vector multiplication\n\n");
    printf("#### args: n=%" PRId64 ", lda=%" PRId64 ", incx=%" PRId64 ", transA=%c, order=%c, uplo=%c"
            ", diag=%c\n", (int64_t)N, (int64_t)lda, (int64_t)incX, transpose_to_char(transA), 
            order_to_char(order), uplo_to_char(uplo), diag_to_char(diag));

    nvpl_int_t len_x = 1 + (N - 1) * labs(incX);

    // allocate memory
    A = (nvpl_dcomplex_t *)malloc(lda * N * sizeof(nvpl_dcomplex_t));
    X = (nvpl_dcomplex_t *)malloc(len_x * sizeof(nvpl_dcomplex_t));

    // fill data
    fill_zmatrix(A, N, N, lda, order, UPLO_TO_FILL_MODE(uplo), diag);
    fill_zvector(X, N, incX);

    // print input data
    print_zmatrix(A, N, N, lda, "A", order);
    print_zvector(X, N, incX, "X");

    // call cblas_ztrmv
    cblas_ztrmv(order, uplo, transA, diag, N, A, lda, X, incX);

    // print result
    printf("\nAfter ztrmv operation:\n");
    print_zvector(X, N, incX, "X");

    // release memory
    free(A);
    free(X);
    return 0;
}
