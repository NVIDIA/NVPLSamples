#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include "common.hpp"

// Use NVPL RAND multi-thread PCG generator to generate double-precision normally-distributed random values.
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    const size_t        length = 100000;
    std::vector<double> array(length);

    nvplRandGenerator_t generator_ptr;
    const auto          generator_type = NVPL_RAND_RNG_PSEUDO_PCG;
    NVPL_RAND_CHECK(nvplRandMTCreateGeneratorDefault(&generator_ptr, generator_type));

    const double mean   = 5.0;
    const double stddev = 7.0;

    NVPL_RAND_CHECK(nvplRandGenerateNormalDouble(generator_ptr, array.data(), length, mean, stddev));
    NVPL_RAND_CHECK(nvplRandDestroyGenerator(generator_ptr));

    // We have an array of normally distributed values, get the mean and stddev to
    // see if it's correct:

    // Get the average and stddev in a robust floating-point fashion (Kahan Summation):
    double sum = 0.0;
    double c   = 0.0;

    for (auto i = 0U; i < length; i++) {
        double y = array[i] - c;
        double t = sum + y;
        c        = (t - sum) - y;
        sum      = t;
    }

    double calculated_mean = sum / double(length);

    // Get the stdsev:
    double stddev_sum = 0.0;
    double stddev_c   = 0.0;

    for (auto i = 0U; i < length; i++) {
        double diff_sq = (array[i] - calculated_mean) * (array[i] - calculated_mean);

        double y   = diff_sq - stddev_c;
        double t   = stddev_sum + y;
        stddev_c   = (t - stddev_sum) - y;
        stddev_sum = t;
    }

    stddev_sum = std::sqrt(stddev_sum / double(length));

    std::cout << std::fixed << std::setprecision(13);
    std::cout << "n = " << length << std::endl;
    std::cout << "Set  mean = " << mean            << "\tSet  stddev = " << stddev << std::endl;
    std::cout << "Calc mean = " << calculated_mean << "\tCalc stddev = " << stddev_sum << std::endl;
    std::cout << "Success\n";
    return 0;
}
