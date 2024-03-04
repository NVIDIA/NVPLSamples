#include <iostream>
#include <vector>
#include "common.hpp"

// Uses NVPL RAND Philox single-threaded and multi-threaded generators to generate random 32-bit unsigned integers 
// and compare results
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    const size_t length = 100000;

    std::vector<unsigned int> array_st(length);
    std::vector<unsigned int> array_mt(length);

    nvplRandGenerator_t gen_st;
    nvplRandGenerator_t gen_mt;
    const auto gen_type = NVPL_RAND_RNG_PSEUDO_PHILOX4_32_10;

    // Create single-threaded generator and set seed/offset
    NVPL_RAND_CHECK(nvplRandCreateGenerator(&gen_st, gen_type));
    NVPL_RAND_CHECK(nvplRandSetPseudoRandomGeneratorSeed(gen_st, 123456ULL));
    NVPL_RAND_CHECK(nvplRandSetGeneratorOffset(gen_st, 123456ULL));

    // Create multi-threaded generator using 64 threads and set seed/offset
    NVPL_RAND_CHECK(nvplRandMTCreateGenerator(&gen_mt, gen_type, 64));
    NVPL_RAND_CHECK(nvplRandSetPseudoRandomGeneratorSeed(gen_mt, 123456ULL));
    NVPL_RAND_CHECK(nvplRandSetGeneratorOffset(gen_mt, 123456ULL));

    // Set ordering to be STRICT
    NVPL_RAND_CHECK(nvplRandMTSetGeneratorOrdering(gen_mt, NVPL_RAND_ORDERING_STRICT));

    // Generate 32-bit random bits
    NVPL_RAND_CHECK(nvplRandGenerate(gen_st, array_st.data(), length));
    NVPL_RAND_CHECK(nvplRandGenerate(gen_mt, array_mt.data(), length));

    // Cleanup
    NVPL_RAND_CHECK(nvplRandDestroyGenerator(gen_st));
    NVPL_RAND_CHECK(nvplRandDestroyGenerator(gen_mt));
    
    // Compare results
    for (auto i = 0U; i < length; i+=length/5) {
        printf("array_st[%u] = %u, array_mt[%u] = %u \n", i, array_st[i], i, array_mt[i]);
    }
    if (array_st == array_mt) {
        std::cout << "Same sequence of random bits is generated using the single-threaded and multi-threaded generators.\n";
        std::cout << "Success\n";
        return 0;
    } else{
        std::cout << "FAILED: Different sequence of random bits is generated using the single-threaded and multi-threaded generators.\n";
        return -1;
    }
};
