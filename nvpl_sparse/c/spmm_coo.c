/*
 * Copyright 1993-2023 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO LICENSEE:
 *
 * This source code and/or documentation ("Licensed Deliverables") are
 * subject to NVIDIA intellectual property rights under U.S. and
 * international Copyright laws.
 *
 * These Licensed Deliverables contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a form of NVIDIA software license agreement by and
 * between NVIDIA and Licensee ("License Agreement") or electronically
 * accepted by Licensee.  Notwithstanding any terms or conditions to
 * the contrary in the License Agreement, reproduction or disclosure
 * of the Licensed Deliverables to any third party without the express
 * written consent of NVIDIA is prohibited.
 *
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, NVIDIA MAKES NO REPRESENTATION ABOUT THE
 * SUITABILITY OF THESE LICENSED DELIVERABLES FOR ANY PURPOSE.  IT IS
 * PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.
 * NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD TO THESE LICENSED
 * DELIVERABLES, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY
 * SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THESE LICENSED DELIVERABLES.
 *
 * U.S. Government End Users.  These Licensed Deliverables are a
 * "commercial item" as that term is defined at 48 C.F.R. 2.101 (OCT
 * 1995), consisting of "commercial computer software" and "commercial
 * computer software documentation" as such terms are used in 48
 * C.F.R. 12.212 (SEPT 1995), and is provided to the U.S. Government
 * only as a commercial end item.  Consistent with 48 C.F.R.12.212 and
 * 48 C.F.R. 227.7202-1 through 227.7202-4 (JUNE 1995), all
 * U.S. Government End Users acquire the Licensed Deliverables with
 * only those rights set forth herein.
 *
 * Any use of the Licensed Deliverables in individual and commercial
 * software must include, in the user documentation and internal
 * comments to the code, the above Disclaimer and U.S. Government End
 * Users Notice.
 */

#include "example_helper.h"

int main(void) {
    // Host problem definition
    const int A_num_rows      = 4;
    const int A_num_cols      = 4;
    const int A_nnz           = 9;
    const int B_num_cols      = 3;
    int       A_row_ind[]    = { 0, 0, 0, 1, 2, 2, 2, 3, 3 };
    int       A_col_ind[]    = { 0, 2, 3, 1, 0, 2, 3, 1, 3 };
    float     A_values[]     = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
                                  6.0f, 7.0f, 8.0f, 9.0f };
    float     B[]            = { 1.0f, 2.0f, 3.0f, 4.0f,
                                  5.0f, 6.0f, 7.0f, 8.0f,
                                  9.0f, 10.0f, 11.0f, 12.0f };
    float     C[]            = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    float     C_result[]     = { 19.0f, 8.0f, 51.0f, 52.0f,
                                 43.0f, 24.0f, 123.0f, 120.0f,
                                 67.0f, 40.0f, 195.0f, 188.0f };
    float     alpha           = 1.0f;
    float     beta            = 0.0f;
    void*     buffer          = NULL;

    //--------------------------------------------------------------------------
    // nvpl_sparse_ APIs
    nvpl_sparse_handle_t     handle = NULL;
    nvpl_sparse_sp_mat_descr_t matA;
    nvpl_sparse_dn_mat_descr_t matB, matC, matD;
    nvpl_sparse_spmm_descr_t   mm_descr;
    size_t               bufferSize = 0;
    CHECK_SPARSE( nvpl_sparse_create(&handle) );
    CHECK_SPARSE( nvpl_sparse_spmm_create_descr(&mm_descr) );
    // Create sparse matrix A in COO format
    CHECK_SPARSE( nvpl_sparse_create_coo(&matA, A_num_rows, A_num_cols, A_nnz,
                                      A_row_ind, A_col_ind, A_values,
                                      NVPL_SPARSE_INDEX_32I,
                                      NVPL_SPARSE_INDEX_BASE_ZERO, NVPL_SPARSE_R_32F) );
    // Create dense matrix B
    CHECK_SPARSE( nvpl_sparse_create_dn_mat(&matB, A_num_cols, B_num_cols, A_num_cols, B, NVPL_SPARSE_R_32F, NVPL_SPARSE_ORDER_COL) );
    // Create dense matrix C (input)
    CHECK_SPARSE( nvpl_sparse_create_dn_mat(&matC, A_num_rows, B_num_cols, A_num_rows, C, NVPL_SPARSE_R_32F, NVPL_SPARSE_ORDER_COL) );
    // Create dense matrix D (output)
    CHECK_SPARSE( nvpl_sparse_create_dn_mat(&matD, A_num_rows, B_num_cols, A_num_rows, C, NVPL_SPARSE_R_32F, NVPL_SPARSE_ORDER_COL) );
    // allocate an external buffer if needed
    CHECK_SPARSE( nvpl_sparse_spmm_buffer_size(
                                 handle, NVPL_SPARSE_OPERATION_NON_TRANSPOSE,
                                 NVPL_SPARSE_OPERATION_NON_TRANSPOSE,
                                 &alpha, matA, matB, &beta, matC, matD, NVPL_SPARSE_R_32F,
                                 NVPL_SPARSE_SPMM_ALG_DEFAULT, mm_descr, &bufferSize) );

    buffer = malloc(bufferSize);

    // Analyze SpMM
    CHECK_SPARSE( nvpl_sparse_spmm_analysis(handle, NVPL_SPARSE_OPERATION_NON_TRANSPOSE,
                                 NVPL_SPARSE_OPERATION_NON_TRANSPOSE,
                                 &alpha, matA, matB, &beta, matC, matD, NVPL_SPARSE_R_32F,
                                 NVPL_SPARSE_SPMM_ALG_DEFAULT, mm_descr, buffer) );

    // execute SpMM
    CHECK_SPARSE( nvpl_sparse_spmm(handle, NVPL_SPARSE_OPERATION_NON_TRANSPOSE,
                                 NVPL_SPARSE_OPERATION_NON_TRANSPOSE,
                                 &alpha, matA, matB, &beta, matC, matD, NVPL_SPARSE_R_32F,
                                 NVPL_SPARSE_SPMM_ALG_DEFAULT, mm_descr) );

    // destroy matrix descriptors
    CHECK_SPARSE( nvpl_sparse_destroy_sp_mat(matA) );
    CHECK_SPARSE( nvpl_sparse_destroy_dn_mat(matB) );
    CHECK_SPARSE( nvpl_sparse_destroy_dn_mat(matC) );
    CHECK_SPARSE( nvpl_sparse_destroy_dn_mat(matD) );
    CHECK_SPARSE( nvpl_sparse_spmm_destroy_descr(mm_descr) );
    CHECK_SPARSE( nvpl_sparse_destroy(handle) );
    free(buffer);
    //--------------------------------------------------------------------------
    int correct = 1;
    for (int i = 0; i < A_num_rows * B_num_cols; i++) {
        if (C[i] != C_result[i]) { // direct floating point comparison is not
            correct = 0;           // reliable
            break;
        }
    }
    if (correct)
        printf("c_spmm_coo test PASSED\n");
    else
        printf("c_spmm_coo test FAILED: wrong result\n");
    return correct ? EXIT_SUCCESS : EXIT_FAILURE;
} 