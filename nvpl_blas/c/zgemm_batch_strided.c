/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_zgemm_batch_strided
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    nvpl_int_t M = 2;
    nvpl_int_t N = 2;
    nvpl_int_t K = 2;
    nvpl_int_t lda = 2;
    nvpl_int_t ldb = 2;
    nvpl_int_t ldc = 2;
    nvpl_dcomplex_t alpha = {1.0f, 2.0f};
    nvpl_dcomplex_t beta = {1.0f, 2.0f};
    nvpl_int_t batch_size = 2;
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA = CblasNoTrans;
    enum CBLAS_TRANSPOSE transB = CblasNoTrans;
    nvpl_dcomplex_t * A = NULL;
    nvpl_dcomplex_t * B = NULL;
    nvpl_dcomplex_t * C = NULL;
    int64_t matrixSizeA = 0;
    int64_t matrixSizeB = 0;
    int64_t matrixSizeC = 0;

    nvpl_int_t rowsA = (transA == CblasNoTrans) ? M : K;
    nvpl_int_t colsA = (transA == CblasNoTrans) ? K : M;
    nvpl_int_t rowsB = (transB == CblasNoTrans) ? K : N;
    nvpl_int_t colsB = (transB == CblasNoTrans) ? N : K;
    nvpl_int_t rowsC = M;
    nvpl_int_t colsC = N;

    matrixSizeA = (int64_t)lda * colsA;
    matrixSizeB = (int64_t)ldb * colsB;
    matrixSizeC = (int64_t)ldc * colsC;
    nvpl_int_t strideA = matrixSizeA;
    nvpl_int_t strideB = matrixSizeB;
    nvpl_int_t strideC = matrixSizeC;
    printf("\nExample: cblas_zgemm_batch_strided for the matrix-matrix multiplication of a batch of matrices\n\n");
    printf("#### args: m=%" PRId64 ", n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", ldc=%" PRId64 ", "
            "transA=%c, transB=%c, order=%c\n", (int64_t)M, (int64_t)N, (int64_t)K, (int64_t)lda, (int64_t)ldb,
            (int64_t)ldc, transpose_to_char(transA), transpose_to_char(transB), order_to_char(order));
    printf("           strideA=%" PRId64 ", strideB=%" PRId64 ", strideC=%" PRId64 ", batch_size=%" PRId64 "\n",
            (int64_t)strideA, (int64_t)strideB, (int64_t)strideC, (int64_t)batch_size);
    printf("           alpha=(%g, %g), beta=(%g, %g)\n", alpha.real, alpha.imag, beta.real, beta.imag);

    // allocate memory
    A = (nvpl_dcomplex_t *)malloc((strideA * (int64_t)batch_size) * sizeof(nvpl_dcomplex_t));
    B = (nvpl_dcomplex_t *)malloc((strideB * (int64_t)batch_size) * sizeof(nvpl_dcomplex_t));
    C = (nvpl_dcomplex_t *)malloc((strideC * (int64_t)batch_size) * sizeof(nvpl_dcomplex_t));

    // fill data
    for (nvpl_int_t i = 0; i < batch_size; ++i) {
        fill_zmatrix(A + strideA * i, rowsA, colsA, lda, order, Full, CblasNonUnit);
        fill_zmatrix(B + strideB * i, rowsB, colsB, ldb, order, Full, CblasNonUnit);
        fill_zmatrix(C + strideC * i, rowsC, colsC, ldc, order, Full, CblasNonUnit);
    }

    // print input data
    for (nvpl_int_t i = 0; i < batch_size; ++i) {
        print_zmatrix(A + strideA * i, rowsA, colsA, lda, "A", order);
        print_zmatrix(B + strideB * i, rowsB, colsB, ldb, "B", order);
        print_zmatrix(C + strideC * i, rowsC, colsC, ldc, "C", order);
    }

    // call cblas_zgemm_batch_strided
    cblas_zgemm_batch_strided(order, transA, transB, M, N, K, &alpha, A, lda, strideA,
            B, ldb, strideB, &beta, C, ldc, strideC, batch_size);

    // print result
    printf("\nAfter gemm operation:\n");
    for (nvpl_int_t i = 0; i < batch_size; ++i) {
        print_zmatrix(C + strideC * i, rowsC, colsC, ldc, "C", order);
    }


    // release memory
    free(A);
    free(B);
    free(C);
    return 0;
}
