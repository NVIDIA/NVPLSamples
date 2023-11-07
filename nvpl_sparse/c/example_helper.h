#ifndef EXAMPLE_HELPER_H
#define EXAMPLE_HELPER_H
#include <stdio.h> // printf
#include <stdlib.h> // EXIT_FAILURE
#include "nvpl_sparse.h"

static const char*  sparseGetStatusString(nvpl_sparse_status_t status) {
  switch (status) {
    case NVPL_SPARSE_STATUS_SUCCESS:
      return "success";
    case NVPL_SPARSE_STATUS_NOT_INITIALIZED:
      return "initialized error";
    case NVPL_SPARSE_STATUS_ALLOC_FAILED:
      return "out of memory";
    case NVPL_SPARSE_STATUS_INVALID_VALUE:
      return "invalid value";
    case NVPL_SPARSE_STATUS_ARCH_MISMATCH:
      return "architecture mismatch";
    case NVPL_SPARSE_STATUS_MAPPING_ERROR:
      return "texture memory mapping error";
    case NVPL_SPARSE_STATUS_EXECUTION_FAILED:
      return "kernel launch failure";
    case NVPL_SPARSE_STATUS_INTERNAL_ERROR:
      return "internal error";
    case NVPL_SPARSE_STATUS_MATRIX_TYPE_NOT_SUPPORTED:
      return "matrix type not supported";
    case NVPL_SPARSE_STATUS_ZERO_PIVOT:
      return "zero pivot";
    case NVPL_SPARSE_STATUS_NOT_SUPPORTED:
      return "operation not supported";
    case NVPL_SPARSE_STATUS_INSUFFICIENT_RESOURCES:
      return "insufficient resources";
  }

  return "unrecognized error code";
}

#define CHECK_SPARSE(func)                                                   \
do {                                                                         \
    nvpl_sparse_status_t status = (func);                                    \
    if (status != NVPL_SPARSE_STATUS_SUCCESS) {                              \
        printf("NVPL SPARSE API failed at line %d with error: %s (%d)\n",       \
               __LINE__, sparseGetStatusString(status), status);             \
        return EXIT_FAILURE;                                                 \
    }                                                                        \
} while(0)

#endif // #ifndef EXAMPLE_HELPER_H
