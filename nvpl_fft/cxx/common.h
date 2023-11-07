#ifndef NVPLFFT_EXAMPLE_COMMON_H_
#define NVPLFFT_EXAMPLE_COMMON_H_

#include <stdio.h>
#include <cmath>
#include <type_traits>
#include <iostream>
#include "utils.h"

static constexpr int MAX_SIZE_B = 256 * 1024 * 1024;

enum class test_case_t {
    FFT_iFFT,
};

enum class test_scenario_t {
    ONE_PLAN_PER_THREAD,
    ONE_PLAN_PER_PROCESS
};

struct test_info {
    int howmany;
    std::vector<int> n;
    int fft_size;
    int istride;
    int ostride;
    int idist;
    int odist;
    std::vector<int> inembed;
    std::vector<int> onembed;

    test_info(int howmany, std::vector<int> &n) : howmany(howmany), n(n), istride(1), ostride(1) {
        fft_size = get_product<int>(n.begin(), n.end());
        idist = fft_size;
        odist = fft_size;
        inembed = n;
        onembed = n;
    }

    test_info(int howmany, std::vector<int> &n, int istride, int ostride, int idist, int odist) : howmany(howmany), n(n), istride(istride), ostride(ostride), idist(idist), odist(odist) {
        fft_size = get_product<int>(n.begin(), n.end());
        inembed = n;
        onembed = n;
    }
};

using test_info_t = test_info;

template<typename T>
void compute_reference(test_case_t tcase, std::vector<T>& in, std::vector<T>& ref, test_info_t info){
    using scalar_type_t = typename type_util<T>::scalar_type;
    switch(tcase) {
        case test_case_t::FFT_iFFT:
            for (int b = 0; b < info.howmany; b++) {
                for (int i = 0; i < info.fft_size; i++) {
                    int input_idx = b * info.idist  + i * info.istride;
                    int   ref_idx = b * info.fft_size + i;
                    ref[ref_idx] = in[input_idx] * (scalar_type_t) info.fft_size;
                }
            }
            break;
        default:
            std::cout << "Invalid test case" << std::endl;
            break;
    }        
}

template<typename T>
double compute_error(T& ref, T& out, test_info_t info){
    double squared_diff = 0;
    double squared_norm = 0;
    for (int b = 0; b < info.howmany; b++) {
        for (int i = 0; i < info.fft_size; i++) {
            int output_idx = b * info.odist + i * info.ostride;
            int    ref_idx = b * info.fft_size + i;
            squared_diff += std::norm(ref[ref_idx] - out[output_idx]); // Note that std::norm(z) = z * conj(z), not the usual sqrt(z * conj(z))
            squared_norm += std::norm(ref[ref_idx]);
        }
    }
    return std::sqrt(squared_diff / squared_norm);
}

template<typename precision>
int assess_error(const std::string& test_cmd, double error) {
    if(error > tolerance<precision>::value) {
        printf("\"%s\" FAILED with error %e\n", test_cmd.c_str(), error);
        return 1;
    } else {
        printf("\"%s\" PASSED with error %e\n", test_cmd.c_str(), error);
        return 0;
    }
}

template<typename precision>
int set_howmany(int howmany, int maxdist){
    int howmany_update = howmany;
    float max_size_B_per_batch = maxdist * (sizeof(precision) * 2) /* assume complex data */;
    if(max_size_B_per_batch * howmany > MAX_SIZE_B) {
        howmany_update = static_cast<int>((float) MAX_SIZE_B / max_size_B_per_batch);
    }
    return howmany_update;
}
#endif // NVPLFFT_EXAMPLE_COMMON_H_
