/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_sgemm_batch
 *
 ******************************************************************************/
#include "example_helper.h"

#define GROUP_COUNT 2

int main() {
    nvpl_int_t M[GROUP_COUNT] = {2, 3};
    nvpl_int_t N[GROUP_COUNT] = {2, 3};
    nvpl_int_t K[GROUP_COUNT] = {2, 3};
    nvpl_int_t lda[GROUP_COUNT] = {2, 3};
    nvpl_int_t ldb[GROUP_COUNT] = {2, 3};
    nvpl_int_t ldc[GROUP_COUNT] = {2, 3};
    float alpha[GROUP_COUNT] = {2.0f, 2.0f};
    float beta[GROUP_COUNT] = {2.0f, 2.0f};
    enum CBLAS_ORDER order = CblasRowMajor;
    enum CBLAS_TRANSPOSE transA[GROUP_COUNT] = {CblasNoTrans, CblasNoTrans};
    enum CBLAS_TRANSPOSE transB[GROUP_COUNT] = {CblasNoTrans, CblasNoTrans};
    nvpl_int_t group_size[GROUP_COUNT] = {2, 3};
    float * A = NULL;
    float * B = NULL;
    float * C = NULL;
    float **A_array = NULL;
    float **B_array = NULL;
    float **C_array = NULL;
    nvpl_int_t total_batch_count = 0;


    printf("\nExample: cblas_sgemm_batch for the matrix-matrix multiplication of a batch of matrices\n\n");

    for (nvpl_int_t i = 0; i < GROUP_COUNT; ++i) {
        total_batch_count += group_size[i];
    }

    A_array = (float **)malloc(total_batch_count * sizeof(float *));
    B_array = (float **)malloc(total_batch_count * sizeof(float *));
    C_array = (float **)malloc(total_batch_count * sizeof(float *));

    nvpl_int_t idx = 0;
    for (nvpl_int_t i = 0; i < GROUP_COUNT; ++i) {
        printf("\nGroup=%" PRId64 ", Number of matrices in the Group=%" PRId64 "\n", (int64_t)i, (int64_t)group_size[i]);
        printf("#### args: m=%" PRId64 ", n=%" PRId64 ", k=%" PRId64 ", lda=%" PRId64 ", ldb=%" PRId64 ", ldc=%" PRId64 ", "
                "transA=%c, transB=%c, order=%c\n", (int64_t)M[i], (int64_t)N[i], (int64_t)K[i], (int64_t)lda[i], (int64_t)ldb[i],
                (int64_t)ldc[i], transpose_to_char(transA[i]), transpose_to_char(transB[i]), order_to_char(order));
        printf("           alpha=%g, beta=%g\n", alpha[i], beta[i]);

        nvpl_int_t rowsA = (transA[i] == CblasNoTrans) ? M[i] : K[i];
        nvpl_int_t colsA = (transA[i] == CblasNoTrans) ? K[i] : M[i];
        nvpl_int_t rowsB = (transB[i] == CblasNoTrans) ? K[i] : N[i];
        nvpl_int_t colsB = (transB[i] == CblasNoTrans) ? N[i] : K[i];
        nvpl_int_t rowsC = M[i];
        nvpl_int_t colsC = N[i];
        
        for (nvpl_int_t j = 0; j < group_size[i]; ++j) {
            // allocate memory
            A = (float *)malloc(lda[i] * colsA * sizeof(float));
            B = (float *)malloc(ldb[i] * colsB * sizeof(float));
            C = (float *)malloc(ldc[i] * colsC * sizeof(float));

            // fill data
            fill_smatrix(A, rowsA, colsA, lda[i], order, Full, CblasNonUnit);
            fill_smatrix(B, rowsB, colsB, ldb[i], order, Full, CblasNonUnit);
            fill_smatrix(C, rowsC, colsC, ldc[i], order, Full, CblasNonUnit);

            // print input data
            print_smatrix(A, rowsA, colsA, lda[i], "A", order);
            print_smatrix(B, rowsB, colsB, ldb[i], "B", order);
            print_smatrix(C, rowsC, colsC, ldc[i], "C", order);

            A_array[idx] = A;
            B_array[idx] = B;
            C_array[idx] = C;
            idx++;
        }
    }

    // call cblas_sgemm_batch
    cblas_sgemm_batch(order, transA, transB, M, N, K, alpha, (const float **)A_array, lda, (const float **)B_array,
            ldb, beta, C_array, ldc, GROUP_COUNT, group_size);

    // print result
    printf("\nAfter cblas_sgemm_batch operation:\n");
    idx = 0;
    for (nvpl_int_t i = 0; i < GROUP_COUNT; ++i) {
        printf("Group=%" PRId64 "\n", (int64_t)i);
        for (nvpl_int_t j = 0; j < group_size[i]; ++j) {
            printf("Matrix idx=%" PRId64 "\n", (int64_t)idx);
            print_smatrix(C_array[idx], M[i], N[i], ldc[i], "C", order);
            free(A_array[idx]);
            free(B_array[idx]);
            free(C_array[idx]);
            idx++;
        }
    }

    // release memory
    free(A_array);
    free(B_array);
    free(C_array);
    return 0;
}
