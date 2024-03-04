/*
 * Copyright (c) 2019-2023, NVIDIA CORPORATION.  All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name(s) of the copyright holder(s) nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <nvpl_tensor.h>

#define HANDLE_ERROR(x)                                           \
    {                                                             \
        const nvpltensorStatus_t err = x;                         \
        if (err != NVPLTENSOR_STATUS_SUCCESS)                     \
        {                                                         \
            printf("Error: %s\n", nvpltensorGetErrorString(err)); \
            exit(-1);                                             \
        }                                                         \
    };

int main()
{
    typedef float floatTypeA;
    typedef float floatTypeC;
    typedef float floatTypeCompute;

    nvpltensorDataType_t typeA = NVPLTENSOR_R_32F;
    nvpltensorDataType_t typeC = NVPLTENSOR_R_32F;
    nvpltensorComputeDescriptor_t const descCompute = NVPLTENSOR_COMPUTE_DESC_32F;

    uint32_t const numThreads = 4;

    floatTypeCompute alpha = (floatTypeCompute) 1.1f;
    floatTypeCompute beta = (floatTypeCompute) 0.f;

    /**********************
     * Computing (partial) reduction : C_{0,1} = alpha * A_{0,2,3,1} + beta * C_{0,1}
     *********************/

    int32_t modeC[] = {0, 1};
    int32_t modeA[] = {0, 2, 3, 1};
    enum { nmodeA = 4 };
    enum { nmodeC = 2 };

    int64_t extent[] = {196, 64, 256, 64};

    int64_t extentC[nmodeC];
    for (int i = 0; i < nmodeC; ++i)
    {
        extentC[i] = extent[modeC[i]];
    }
    int64_t extentA[nmodeA];
    for (int i = 0; i < nmodeA; ++i)
    {
        extentA[i] = extent[modeA[i]];
    }

    /**********************
     * Allocating data
     *********************/

    int64_t elementsA = 1;
    for (int i = 0; i < nmodeA; ++i)
    {
        elementsA *= extent[i];
    }
    int64_t elementsC = 1;
    for (int i = 0; i < nmodeC; ++i)
    {
        elementsC *= extent[i];
    }

    int64_t sizeA = sizeof(floatTypeA) * elementsA;
    int64_t sizeC = sizeof(floatTypeC) * elementsC;

    uint32_t const kAlignment = 128;  // Alignment of the pointers (bytes)

    floatTypeA* A = aligned_alloc(kAlignment, sizeA);
    floatTypeC* C = aligned_alloc(kAlignment, sizeC);

    if (A == NULL || C == NULL)
    {
        printf("Error: allocation of tensor memory.\n");
        return -1;
    }

    /*******************
     * Initialize data
     *******************/

    for (int64_t i = 0; i < elementsA; i++)
        A[i] = (((floatTypeA) (rand() / RAND_MAX)) - 0.5) * 100;
    for (int64_t i = 0; i < elementsC; i++)
        C[i] = (((floatTypeC) (rand() / RAND_MAX)) - 0.5) * 100;

    /*************************
     * nvplTENSOR
     *************************/

    /*************************
     * Create nvplTENSOR handle
     *************************/

    nvpltensorHandle_t handle;
    HANDLE_ERROR(nvpltensorCreate(&handle));

    /**********************
     * Set numbers of threads that nvplTensor can use
     **********************/
    HANDLE_ERROR(nvpltensorSetNumThreads(handle, numThreads));

    /**********************
     * Create Tensor Descriptors
     **********************/

    nvpltensorTensorDescriptor_t descA;
    HANDLE_ERROR(
        nvpltensorCreateTensorDescriptor(handle, &descA, nmodeA, extentA, NULL /* stride */, typeA, kAlignment));

    nvpltensorTensorDescriptor_t descC;
    HANDLE_ERROR(
        nvpltensorCreateTensorDescriptor(handle, &descC, nmodeC, extentC, NULL /* stride */, typeC, kAlignment));

    nvpltensorOperator_t const opReduce = NVPLTENSOR_OP_ADD;

    /*******************************
     * Create Reduction Descriptor
     *******************************/

    nvpltensorOperationDescriptor_t desc;
    HANDLE_ERROR(nvpltensorCreateReduction(handle, &desc, descA, modeA, NVPLTENSOR_OP_IDENTITY, descC, modeC,
                                           NVPLTENSOR_OP_IDENTITY, descC, modeC, opReduce, descCompute));

    /**************************
     * Set the algorithm to use
     ***************************/

    nvpltensorAlgo_t const algo = NVPLTENSOR_ALGO_DEFAULT;

    nvpltensorPlanPreference_t planPref;
    HANDLE_ERROR(nvpltensorCreatePlanPreference(handle, &planPref, algo, NVPLTENSOR_JIT_MODE_NONE));

    /**********************
     * Query workspace estimate
     **********************/

    uint64_t workspaceSizeEstimate = 0;
    nvpltensorWorksizePreference_t const workspacePref = NVPLTENSOR_WORKSPACE_DEFAULT;
    HANDLE_ERROR(nvpltensorEstimateWorkspaceSize(handle, desc, planPref, workspacePref, &workspaceSizeEstimate));

    /**************************
     * Create Reduction Plan
     **************************/

    nvpltensorPlan_t plan;
    HANDLE_ERROR(nvpltensorCreatePlan(handle, &plan, desc, planPref, workspaceSizeEstimate));

    /**************************
     * Optional: Query information about the created plan
     **************************/

    // query actually used workspace
    uint64_t actualWorkspaceSize = 0;
    HANDLE_ERROR(nvpltensorPlanGetAttribute(handle, plan, NVPLTENSOR_PLAN_REQUIRED_WORKSPACE, &actualWorkspaceSize,
                                            sizeof(actualWorkspaceSize)));

    // At this point the user knows exactly how much memory is need by the operation and
    // only the smaller actual workspace needs to be allocated
    assert(actualWorkspaceSize <= workspaceSizeEstimate);
    actualWorkspaceSize += 256;

    void* work = NULL;
    if (actualWorkspaceSize > 0)
    {
        work = aligned_alloc(kAlignment, actualWorkspaceSize);
    }

    /**********************
     * Execute
     **********************/

    HANDLE_ERROR(
        nvpltensorReduce(handle, plan, (void const*) &alpha, A, (void const*) &beta, C, C, work, actualWorkspaceSize));

    /*************************/

    HANDLE_ERROR(nvpltensorDestroy(handle));
    HANDLE_ERROR(nvpltensorDestroyPlan(plan));
    HANDLE_ERROR(nvpltensorDestroyOperationDescriptor(desc));
    HANDLE_ERROR(nvpltensorDestroyTensorDescriptor(descA));
    HANDLE_ERROR(nvpltensorDestroyTensorDescriptor(descC));

    if (A)
        free(A);
    if (C)
        free(C);
    if (work)
        free(work);
    return 0;
}
