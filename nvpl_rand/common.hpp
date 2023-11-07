#ifndef NVPL_RAND_EXAMPLE_COMMON_H
#define NVPL_RAND_EXAMPLE_COMMON_H

// NVPL_RAND
#include <nvpl_rand.h>

#define NVPL_RAND_CHECK(x)                                  \
    do {                                                    \
        if ((x) != NVPL_RAND_STATUS_SUCCESS) {              \
            printf("Error at %s:%d\n", __FILE__, __LINE__); \
            _Exit(EXIT_FAILURE);                            \
        }                                                   \
    } while (0)


#ifdef NVPL_RAND_EXAMPLE_CURAND_AVAILABLE
#    include <curand.h>

#    define CUDA_CHECK(x)                                       \
        do {                                                    \
            if ((x) != cudaSuccess) {                           \
                printf("Error at %s:%d\n", __FILE__, __LINE__); \
                _Exit(EXIT_FAILURE);                            \
            }                                                   \
        } while (0)
#    define CURAND_CHECK(x)                                     \
        do {                                                    \
            if ((x) != CURAND_STATUS_SUCCESS) {                 \
                printf("Error at %s:%d\n", __FILE__, __LINE__); \
                _Exit(EXIT_FAILURE);                            \
            }                                                   \
        } while (0)
#endif // NVPL_RAND_EXAMPLE_CURAND_AVAILABLE

#endif // NVPL_RAND_EXAMPLE_COMMON_H
