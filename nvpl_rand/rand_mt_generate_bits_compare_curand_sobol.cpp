#include <iostream>
#include <vector>
#include <chrono>
#include "common.hpp"

// Uses NVPL RAND SOBOL and cuRAND SOBOL generators to generate random 64-bit unsigned integers and compare results
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    const size_t             length   = 10000;
    const unsigned long long offset   = 0x123456ULL;
    const unsigned int       ndims    = 2000;
    const unsigned int       nthreads = 64;

    std::vector<unsigned long long> array_rand(length);

    nvplRandGenerator_t gen_rand;
    const auto          gen_type = NVPL_RAND_RNG_QUASI_SCRAMBLED_SOBOL64;

    // NVPL RAND
    NVPL_RAND_CHECK(nvplRandMTCreateGenerator(&gen_rand, gen_type, nthreads));
    NVPL_RAND_CHECK(nvplRandSetGeneratorOffset(gen_rand, offset));
    NVPL_RAND_CHECK(nvplRandSetQuasiRandomGeneratorDimensions(gen_rand, ndims));

    // Generate on CPU
    auto start = std::chrono::high_resolution_clock::now();
    NVPL_RAND_CHECK(nvplRandGenerateLongLong(gen_rand, array_rand.data(), length));
    auto end = std::chrono::high_resolution_clock::now();
    auto ms  = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Multi thread nvplRandGenerate with SCRAMBLED_SOBOL64 using" << nthreads << " nthreads takes "
              << float(ms.count()) / 1000.f << " ms \n";

    NVPL_RAND_CHECK(nvplRandDestroyGenerator(gen_rand));

#ifdef NVPL_RAND_EXAMPLE_CURAND_AVAILABLE
    // CURAND
    curandGenerator_t gen_curand;
    const auto        curand_gen_type = CURAND_RNG_QUASI_SCRAMBLED_SOBOL64;

    unsigned long long*             devData;
    std::vector<unsigned long long> array_curand(length);

    /* Allocate n floats on device */
    CUDA_CHECK(cudaMalloc(reinterpret_cast<void**>(&devData), length * sizeof(unsigned long long)));

    CURAND_CHECK(curandCreateGenerator(&gen_curand, curand_gen_type));
    CURAND_CHECK(curandSetGeneratorOffset(gen_curand, offset));
    CURAND_CHECK(curandSetQuasiRandomGeneratorDimensions(gen_curand, ndims));

    // Generate 64-bit random bits on GPU
    start = std::chrono::high_resolution_clock::now();
    CURAND_CHECK(curandGenerateLongLong(gen_curand, devData, length));
    end = std::chrono::high_resolution_clock::now();
    ms  = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "cuRandGenerate with SCRAMBLED_SOBOL64 takes " << float(ms.count()) / 1000.f << " ms \n";

    /* Copy device memory to host */
    CUDA_CHECK(cudaMemcpy(array_curand.data(), devData, length * sizeof(unsigned long long), cudaMemcpyDeviceToHost));

    CURAND_CHECK(curandDestroyGenerator(gen_curand));
    CUDA_CHECK(cudaFree(devData));

    // compute hash
    unsigned long long xor_curand = 0x0;
    for (auto i = 0U; i < length; i++) {
        xor_curand ^= array_curand[i];
    }
    std::cout << "CURAND xor output is " << std::hex << xor_curand << std::endl;

    // Compare results
    for (auto i = 0U; i < length; i += length / 5) {
        printf("array_rand[%u] = %llu, array_curand[%u] = %llu \n", i, array_rand[i], i, array_curand[i]);
    }
    for (auto i = 0U; i < length; i++) {
        if (array_rand[i] != array_curand[i])
            printf("array_rand[%u] = %llu, array_curand[%u] = %llu \n", i, array_rand[i], i, array_curand[i]);
    }
    if (array_rand == array_curand) {
        std::cout << "Same sequence is generated with NVPL RAND and cuRAND SOBOL64 generator using LEGACY ordering.\n";
        std::cout << "Success\n";
        return 0;
    } else {
        std::cout
            << "FAILED: different sequence is generated with NVPL RAND and cuRAND SOBOL64 generator using LEGACY ordering.\n";
        return -1;
    }
#else
    // Compare hash
    unsigned long long xor_rand = 0x0;
    for (auto i = 0U; i < length; i++) {
        xor_rand ^= array_rand[i];
    }
    std::cout << "NVPL RAND xor output is " << std::hex << xor_rand << std::endl;
    if (xor_rand == 0xa75c795246aa956c) {
        std::cout << "Same sequence is generated with NVPL RAND and cuRAND SOBOL64 generator using LEGACY ordering.\n";
        std::cout << "Success\n";
        return 0;
    } else {
        std::cout
            << "FAILED: Different sequence is generated with NVPL RAND and cuRAND SOBOL64 generator using LEGACY ordering.\n";
        return -1;
    }
#endif
};
