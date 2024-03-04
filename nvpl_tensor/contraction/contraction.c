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
    typedef float floatTypeB;
    typedef float floatTypeC;
    typedef float floatTypeCompute;

    nvpltensorDataType_t typeA = NVPLTENSOR_R_32F;
    nvpltensorDataType_t typeB = NVPLTENSOR_R_32F;
    nvpltensorDataType_t typeC = NVPLTENSOR_R_32F;
    nvpltensorComputeDescriptor_t const descCompute = NVPLTENSOR_COMPUTE_DESC_32F;

    uint32_t const numThreads = 4;

    /**********************
     * Computing: C_{0,2,1,3} = alpha * A_{0,4,5,1} B_{2,5,3,4} + beta * C_{0,2,1,3}
     **********************/

    floatTypeCompute alpha = (floatTypeCompute) 1.1f;
    floatTypeCompute beta = (floatTypeCompute) 0.f;

    int32_t modeC[] = {0, 2, 1, 3};
    int32_t modeA[] = {0, 4, 5, 1};
    int32_t modeB[] = {2, 5, 3, 4};
    enum { nmodeA = 4 };
    enum { nmodeB = 4 };
    enum { nmodeC = 4 };

    int64_t extent[] = {6, 6, 6, 4, 4, 4};

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
    int64_t extentB[nmodeB];
    for (int i = 0; i < nmodeB; ++i)
    {
        extentB[i] = extent[modeB[i]];
    }

    /**********************
     * Allocating data
     **********************/

    int64_t elementsA = 1;
    for (int i = 0; i < nmodeA; ++i)
    {
        elementsA *= extent[i];
    }
    int64_t elementsB = 1;
    for (int i = 0; i < nmodeB; ++i)
    {
        elementsB *= extent[i];
    }
    int64_t elementsC = 1;
    for (int i = 0; i < nmodeC; ++i)
    {
        elementsC *= extent[i];
    }

    int64_t sizeA = sizeof(floatTypeA) * elementsA;
    int64_t sizeB = sizeof(floatTypeB) * elementsB;
    int64_t sizeC = sizeof(floatTypeC) * elementsC;

    uint32_t const kAlignment = 128;  // Alignment of the pointers (bytes)

    floatTypeA* A = aligned_alloc(kAlignment, sizeA);
    floatTypeB* B = aligned_alloc(kAlignment, sizeB);
    floatTypeC* C = aligned_alloc(kAlignment, sizeC);

    if (A == NULL || B == NULL || C == NULL)
    {
        printf("Error: allocation of tensor memory.\n");
        return -1;
    }

    /*******************
     * Initialize data
     *******************/

    for (int64_t i = 0; i < elementsA; i++)
        A[i] = (((floatTypeA) (rand() / RAND_MAX)) - 0.5) * 100;
    for (int64_t i = 0; i < elementsB; i++)
        B[i] = (((floatTypeB) (rand() / RAND_MAX)) - 0.5) * 100;
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
    HANDLE_ERROR(nvpltensorCreateTensorDescriptor(handle, &descA, nmodeA, extentA, NULL, /*stride*/
                                                  typeA, kAlignment));

    nvpltensorTensorDescriptor_t descB;
    HANDLE_ERROR(nvpltensorCreateTensorDescriptor(handle, &descB, nmodeB, extentB, NULL, /*stride*/
                                                  typeB, kAlignment));

    nvpltensorTensorDescriptor_t descC;
    HANDLE_ERROR(nvpltensorCreateTensorDescriptor(handle, &descC, nmodeC, extentC, NULL, /*stride*/
                                                  typeC, kAlignment));

    /*******************************
     * Create Contraction Descriptor
     *******************************/

    nvpltensorOperationDescriptor_t desc;
    HANDLE_ERROR(nvpltensorCreateContraction(handle, &desc, descA, modeA, /* unary operator A*/ NVPLTENSOR_OP_IDENTITY,
                                             descB, modeB, /* unary operator B*/ NVPLTENSOR_OP_IDENTITY, descC, modeC,
                                             /* unary operator C*/ NVPLTENSOR_OP_IDENTITY, descC, modeC, descCompute));

    /*****************************
     * Optional (but recommended): ensure that the scalar type is correct.
     *****************************/

    nvpltensorDataType_t scalarType;
    HANDLE_ERROR(nvpltensorOperationDescriptorGetAttribute(handle, desc, NVPLTENSOR_OPERATION_DESCRIPTOR_SCALAR_TYPE,
                                                           (void*) &scalarType, sizeof(scalarType)));

    assert(scalarType == NVPLTENSOR_R_32F);

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
     * Create Contraction Plan
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
        nvpltensorContract(handle, plan, (void*) &alpha, A, B, (void*) &beta, C, C, work, actualWorkspaceSize));

    /*************************/

    HANDLE_ERROR(nvpltensorDestroy(handle));
    HANDLE_ERROR(nvpltensorDestroyPlan(plan));
    HANDLE_ERROR(nvpltensorDestroyOperationDescriptor(desc));
    HANDLE_ERROR(nvpltensorDestroyTensorDescriptor(descA));
    HANDLE_ERROR(nvpltensorDestroyTensorDescriptor(descB));
    HANDLE_ERROR(nvpltensorDestroyTensorDescriptor(descC));

    if (A)
        free(A);
    if (B)
        free(B);
    if (C)
        free(C);
    if (work)
        free(work);
    return 0;
}
