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
 * C.F.R. 12.212 (SEPT 1995) and is provided to the U.S. Government
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
    int       A_csrOffsets[] = { 0, 3, 4, 7, 9 };
    int       A_columns[]    = { 0, 2, 3, 1, 0, 2, 3, 1, 3 };
    float     A_values[]     = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
                                 6.0f, 7.0f, 8.0f, 9.0f };
    float     X[]            = { 1.0f, 8.0f, 23.0f, 52.0f };
    float     Y[]            = { 0.0f, 0.0f, 0.0f, 0.0f };
    float     Y_result[]     = { 1.0f, 2.0f, 3.0f, 4.0f };
    float     alpha           = 1.0f;
    void*     buffer          = NULL;
    //--------------------------------------------------------------------------
    // nvpl_sparse_ APIs
    nvpl_sparse_handle_t     handle = NULL;
    nvpl_sparse_sp_mat_descr_t matA;
    nvpl_sparse_dn_vec_descr_t vecX, vecY;
    size_t               bufferSize = 0;
    nvpl_sparse_spsv_descr_t  spsvDescr;
    CHECK_SPARSE( nvpl_sparse_create(&handle) );
    // Create sparse matrix A in CSR format
    CHECK_SPARSE( nvpl_sparse_create_csr(&matA, A_num_rows, A_num_cols, A_nnz,
                                      A_csrOffsets, A_columns, A_values,
                                      NVPL_SPARSE_INDEX_32I, NVPL_SPARSE_INDEX_32I,
                                      NVPL_SPARSE_INDEX_BASE_ZERO, NVPL_SPARSE_R_32F) );
    // Create dense vector X
    CHECK_SPARSE( nvpl_sparse_create_dn_vec(&vecX, A_num_cols, X, NVPL_SPARSE_R_32F) );
    // Create dense vector Y
    CHECK_SPARSE( nvpl_sparse_create_dn_vec(&vecY, A_num_rows, Y, NVPL_SPARSE_R_32F) );
    // Create opaque data structure, that holds analysis data between calls.
    CHECK_SPARSE( nvpl_sparse_spsv_create_descr(&spsvDescr) );
    // Specify Lower|Upper fill mode.
    nvpl_sparse_fill_mode_t fillmode = NVPL_SPARSE_FILL_MODE_LOWER;
    CHECK_SPARSE( nvpl_sparse_sp_mat_set_attribute(matA, NVPL_SPARSE_SPMAT_FILL_MODE,
                                              &fillmode, sizeof(fillmode)) );
    // Specify Unit|Non-Unit diagonal type.
    nvpl_sparse_diag_type_t diagtype = NVPL_SPARSE_DIAG_TYPE_NON_UNIT;
    CHECK_SPARSE( nvpl_sparse_sp_mat_set_attribute(matA, NVPL_SPARSE_SPMAT_DIAG_TYPE,
                                              &diagtype, sizeof(diagtype)) );
    // allocate an external buffer for analysis
    CHECK_SPARSE( nvpl_sparse_spsv_buffer_size(
                                handle, NVPL_SPARSE_OPERATION_NON_TRANSPOSE,
                                &alpha, matA, vecX, vecY, NVPL_SPARSE_R_32F,
                                NVPL_SPARSE_SPSV_ALG_DEFAULT, spsvDescr,
                                &bufferSize) );
    buffer = malloc(bufferSize);
    CHECK_SPARSE( nvpl_sparse_spsv_analysis(
                                handle, NVPL_SPARSE_OPERATION_NON_TRANSPOSE,
                                &alpha, matA, vecX, vecY, NVPL_SPARSE_R_32F,
                                NVPL_SPARSE_SPSV_ALG_DEFAULT, spsvDescr, buffer) );
    // execute SpSV
    CHECK_SPARSE( nvpl_sparse_spsv_solve(handle, NVPL_SPARSE_OPERATION_NON_TRANSPOSE,
                                       &alpha, matA, vecX, vecY, NVPL_SPARSE_R_32F,
                                       NVPL_SPARSE_SPSV_ALG_DEFAULT, spsvDescr) );

    // destroy matrix/vector descriptors
    CHECK_SPARSE( nvpl_sparse_destroy_sp_mat(matA) );
    CHECK_SPARSE( nvpl_sparse_destroy_dn_vec(vecX) );
    CHECK_SPARSE( nvpl_sparse_destroy_dn_vec(vecY) );
    CHECK_SPARSE( nvpl_sparse_spsv_destroy_descr(spsvDescr));
    CHECK_SPARSE( nvpl_sparse_destroy(handle) );
    free(buffer);
    int correct = 1;
    for (int i = 0; i < A_num_rows; i++) {
        if (Y[i] != Y_result[i]) { // direct floating point comparison is not
            correct = 0;           // reliable
            break;
        }
    }
    if (correct)
        printf("c_spsv_csr test PASSED\n");
    else
        printf("c_spsv_csr test FAILED: wrong result\n");

    return correct ? EXIT_SUCCESS : EXIT_FAILURE;
}
