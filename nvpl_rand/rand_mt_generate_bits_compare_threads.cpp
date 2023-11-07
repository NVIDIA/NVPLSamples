#include <iostream>
#include <vector>
#include "common.hpp"

// Use NVPL RAND multi-thread XORWOW generator to generate random 32-bit unsigned integers
// and compare results generated with different threads
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    const size_t length = 100000;

    std::vector<unsigned int> array_1(length);
    std::vector<unsigned int> array_2(length);

    nvplRandGenerator_t gen_1;
    nvplRandGenerator_t gen_2;
    const auto generator_type = NVPL_RAND_RNG_PSEUDO_XORWOW;

    // Create generators with different numbers of threads
    NVPL_RAND_CHECK(nvplRandMTCreateGeneratorDefault(&gen_1, generator_type));
    const unsigned int nthreads = 4;
    NVPL_RAND_CHECK(nvplRandMTCreateGenerator(&gen_2, generator_type, nthreads));
    
    // Generate
    NVPL_RAND_CHECK(nvplRandGenerate(gen_1, array_1.data(), length));
    NVPL_RAND_CHECK(nvplRandGenerate(gen_2, array_2.data(), length));

    // Cleanup
    NVPL_RAND_CHECK(nvplRandDestroyGenerator(gen_1));
    NVPL_RAND_CHECK(nvplRandDestroyGenerator(gen_2));
    
    // Compare results
    for (auto i = 0U; i < length; i+=length/5) {
        printf("array_1[%u] = %u, array_2[%u] = %u \n", i, array_1[i], i, array_2[i]);
    }
    if (array_1 == array_2) {
        std::cout << "Same sequence of random bits is generated using different numbers of threads.\n";
        std::cout << "Success\n";
        return 0;
    } else{
        std::cout << "FAILED: Different sequence of random bits is generated using different numbers of threads.\n";
        return -1;
    }
};
