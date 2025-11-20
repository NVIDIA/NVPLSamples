/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_dgemmt
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t N = 2;
    nvpl_int_t K = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldb = 2;
    nvpl_int_t ldc = 2;
    double alpha = 0.6;
    double beta = 0.4;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_UPLO uplo = CblasLower;
    enum CBLAS_TRANSPOSE transA = CblasTrans;
    enum CBLAS_TRANSPOSE transB = CblasNoTrans;
    double * A = NULL;
    double * B = NULL;
    double * C = NULL;

    printf("\nExample: cblas_dgemmt for symmetric matrix-matrix multiplication\n\n");
    printf("#### args: m=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", ldc=%" PRId64 ", "
            "order=%c, uplo=%c, transA=%c, transB=%c\n", (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)ldb,
            (int64_t)ldc, order_to_char(order), uplo_to_char(uplo), 
            transpose_to_char(transA), transpose_to_char(transB));
    printf("           alpha=%g, beta=%g\n", alpha, beta);

    nvpl_int_t rowsA = (transA == CblasNoTrans) ? N : K;
    nvpl_int_t colsA = (transA == CblasNoTrans) ? K : N;
    nvpl_int_t rowsB = (transB == CblasNoTrans) ? K : N;
    nvpl_int_t colsB = (transB == CblasNoTrans) ? N : K;
    nvpl_int_t rowsC = N;
    nvpl_int_t colsC = N;

    // allocate memory
    A = (double *)malloc(lda * colsA * sizeof(double));
    B = (double *)malloc(ldb * colsB * sizeof(double));
    C = (double *)malloc(ldc * colsC * sizeof(double));

    // fill data
    fill_dmatrix(A, rowsA, colsA, lda, order, Full, CblasNonUnit);
    fill_dmatrix(B, rowsB, colsB, ldb, order, Full, CblasNonUnit);
    fill_dmatrix(C, rowsC, colsC, ldc, order, UPLO_TO_FILL_MODE(uplo), CblasNonUnit);

    // print input data
    print_dmatrix(A, rowsA, colsA, lda, "A", order);
    print_dmatrix(B, rowsB, colsB, ldb, "B", order);
    print_dmatrix(C, rowsC, colsC, ldc, "C", order);

    // call cblas_dgemmt
    cblas_dgemmt(order, uplo, transA, transB, N, K, alpha, A, lda, B, ldb, beta, C, ldc);

    // print result
    printf("\nAfter dgemmt operation:\n");
    print_dmatrix(C, rowsC, colsC, ldc, "C", order);

    // release memory
    free(A);
    free(B);
    free(C);
    return 0;
}
