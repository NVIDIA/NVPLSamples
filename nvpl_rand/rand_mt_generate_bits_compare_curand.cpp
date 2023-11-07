#include <iostream>
#include <vector>
#include <chrono>
#include "common.hpp"

// Use NVPL RAND Philox and cuRAND Philox generators to generate random 32-bit unsigned integers and compare results
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    const size_t length = 100000;

    std::vector<unsigned int> array_rand(length);

    nvplRandGenerator_t gen_rand;
    const auto          gen_type = NVPL_RAND_RNG_PSEUDO_PHILOX4_32_10;

    // Create multi-threaded generator using 64 threads and set seed, offset and ordering
    NVPL_RAND_CHECK(nvplRandMTCreateGenerator(&gen_rand, gen_type, 64));
    NVPL_RAND_CHECK(nvplRandSetPseudoRandomGeneratorSeed(gen_rand, 123456ULL));
    NVPL_RAND_CHECK(nvplRandSetGeneratorOffset(gen_rand, 123456ULL));
    NVPL_RAND_CHECK(nvplRandMTSetGeneratorOrdering(gen_rand, NVPL_RAND_ORDERING_CURAND_LEGACY));

    // Generate 32-bit random bits
    auto start = std::chrono::high_resolution_clock::now();
    NVPL_RAND_CHECK(nvplRandGenerate(gen_rand, array_rand.data(), length));
    auto end = std::chrono::high_resolution_clock::now();
    auto ms  = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Multi thread nvplRandGenerate with 64 nthreads takes " << float(ms.count()) / 1000.f << " ms \n";

    // Cleanup
    NVPL_RAND_CHECK(nvplRandDestroyGenerator(gen_rand));

#ifdef NVPL_RAND_EXAMPLE_CURAND_AVAILABLE
    // CURAND
    curandGenerator_t gen_curand;
    const auto        curand_gen_type = CURAND_RNG_PSEUDO_PHILOX4_32_10;

    unsigned int*             devData;
    std::vector<unsigned int> array_curand(length);

    /* Allocate n floats on device */
    CUDA_CHECK(cudaMalloc(reinterpret_cast<void**>(&devData), length * sizeof(unsigned int)));

    /* Create pseudo-random number generator */
    CURAND_CHECK(curandCreateGenerator(&gen_curand, curand_gen_type));

    CURAND_CHECK(curandSetPseudoRandomGeneratorSeed(gen_curand, 123456ULL));
    CURAND_CHECK(curandSetGeneratorOffset(gen_curand, 123456ULL));
    CURAND_CHECK(curandSetGeneratorOrdering(gen_curand, CURAND_ORDERING_PSEUDO_LEGACY));

    //Generate 32-bit random bits on device
    start = std::chrono::high_resolution_clock::now();
    CURAND_CHECK(curandGenerate(gen_curand, devData, length));
    end = std::chrono::high_resolution_clock::now();
    ms  = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "cuRandGenerate with CURAND_ORDERING_PSEUDO_LEGACY takes " << float(ms.count()) / 1000.f << " ms \n";

    /* Copy device memory to host */
    CUDA_CHECK(cudaMemcpy(array_curand.data(), devData, length * sizeof(unsigned int), cudaMemcpyDeviceToHost));

    CURAND_CHECK(curandDestroyGenerator(gen_curand));
    CUDA_CHECK(cudaFree(devData));

    // compute hash
    unsigned int xor_curand = 0x0;
    for (auto i = 0U; i < length; i++) {
        xor_curand ^= array_curand[i];
    }
    std::cout << "CURAND xor output is " << std::hex << xor_curand << std::endl;

    // Compare results
    for (auto i = 0U; i < length; i += length / 5) {
        printf("array_rand[%u] = %u, array_curand[%u] = %u \n", i, array_rand[i], i, array_curand[i]);
    }
    if (array_rand == array_curand) {
        std::cout << "Same sequence is generated with NVPL RAND and cuRAND generator using LEGACY ordering.\n";
        std::cout << "Success\n";
        return 0;
    } else {
        std::cout << "FAILED: sequence is generated with NVPL RAND and cuRAND generator using LEGACY ordering.\n";
        return -1;
    }
#else
    // Compare hash
    unsigned int xor_rand = 0x0;
    for (auto i = 0U; i < length; i++) {
        xor_rand ^= array_rand[i];
    }
    if (xor_rand == 0xc7042bf2) {
        std::cout << "Same sequence is generated with NVPL RAND and cuRAND generator using LEGACY ordering.\n";
        std::cout << "Success\n";
        return 0;
    } else {
        std::cout << "FAILED: sequence is generated with NVPL RAND and cuRAND generator using LEGACY ordering.\n";
        return -1;
    }
#endif
};
