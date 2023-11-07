#include <iostream>
#include <vector>
#include "common.hpp"

// Use single-threaded NVPL RAND library to generate a sequence of distributions with different APIs
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    nvplRandGenerator_t gen;

    const size_t        length = 10000;
    std::vector<double> out_uniform1(length);
    std::vector<double> out_uniform2(length);

    // Create a handle to MRG generator
    NVPL_RAND_CHECK(nvplRandCreateGenerator(&gen, NVPL_RAND_RNG_PSEUDO_MRG32K3A));

    // Set offset (optional)
    NVPL_RAND_CHECK(nvplRandSetGeneratorOffset(gen, 1234ULL));

    // Generate n uniformly-distributed doubles between -1. and 1. */
    NVPL_RAND_CHECK(nvplRandGenerateUniformRangeDouble(gen, out_uniform1.data(), length, -1., 1.));

    // Alternatively, use a generic distribution API to generate n uniformly-distributed doubles between -1. and 1.
    nvplRandDistributionConfig_t config = {
        NVPL_RAND_CONTINUOUS_DIST_UNIFORM_RANGE, -1, 1, nullptr, 0, 0}; // 0 means not used

    NVPL_RAND_CHECK(nvplRandSetGeneratorOffset(gen, 1234ULL));
    NVPL_RAND_CHECK(nvplRandGenerateDistributionDouble(gen, out_uniform2.data(), config, length));

    // Show results
    for (auto i = 0U; i < length; i += length / 5) {
        printf("out_uniform1[%u] = %10.6f, array_uniform2[%u] = %10.6f \n", i, out_uniform1[i], i, out_uniform2[i]);
    }

    /* Next, Generate FP32 samples with Dirichlet distribution of order 4 with specified alpha parameters */
    const size_t       order = 4;
    std::vector<double> out_dirichlet(length * order);

    double alpha[] = {0.2, 0.1, 0.3, 0.6};
    config         = {NVPL_RAND_CONTINUOUS_DIST_DIRICHLET, 0, 0, alpha, order, 0}; // 0 means not used

    nvplRandGenerateDistributionDouble(gen, out_dirichlet.data(), config, length);

    // Show results
    for (auto i = 0U; i < length; i += length / 5) {
        printf("out_dirichlet[%u] = ", i);
        for (auto j = 0U; j < order; j++) {
            printf("%10.6f, ", out_dirichlet[i * order + j]);
        }
        std::cout << std::endl;
    }

    /* Cleanup */
    nvplRandDestroyGenerator(gen);

    std::cout << "Success\n";
    return 0;
};
