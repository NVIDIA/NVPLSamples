#ifndef NVPLFFT_EXAMPLE_COMMON_H_
#define NVPLFFT_EXAMPLE_COMMON_H_

#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <type_traits>
#include <iostream>
#include "utils.h"
#include <algorithm>
#include <numeric>
#include <cassert>
#include <tuple>
#include <vector>

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

    test_info(int howmany, const std::vector<int> &n) : howmany(howmany), n(n), istride(1), ostride(1) {
        fft_size = get_product<int>(n.begin(), n.end());
        idist = fft_size;
        odist = fft_size;
        inembed = n;
        onembed = n;
    }

    test_info(int howmany, const std::vector<int> &n, int istride, int ostride, int idist, int odist) : howmany(howmany), n(n), istride(istride), ostride(ostride), idist(idist), odist(odist) {
        fft_size = get_product<int>(n.begin(), n.end());
        inembed = n;
        onembed = n;
    }
};

using test_info_t = test_info;

template<typename precision>
int set_howmany(int howmany, int maxdist);

/**
 * @brief Returns a pair of test setups for r2c -> c2r transforms
 * First transpose meets the requirements of in-place transform.
 *
 * @tparam T underlying data type
 * @param n Shape of the transpose
 * @param howmany_default the suggested batch size
 * @param r2c_inplace_strides stride between elements of the first transpose (istride, ostride)
 * @param r2c_sample_step additional step to increase idist and odist of first transpose
 * @param c2r_output_stride stride between the outputs elements of second transpose
 */
template <typename T>
std::tuple<test_info_t, test_info_t> get_r2c_c2r_setups(const std::vector<int> &n, int howmany_default, int r2c_inplace_strides, int r2c_sample_step, int c2r_output_stride) {
    // For R2C/C2R transforms, the real    (R2C input , C2R output) array has n[0] x .. x n[rank-1] elems and
    //                         the complex (R2C output, C2R input ) array has n[1] x .. x (n[rank -1] / 2 + 1) elems
    // See https://docs.nvidia.com/cuda/cufft/index.html#data-layout for more details.
    const int real_elems    = get_product<int>(n.begin(),   n.end());
    const int complex_elems = get_product<int>(n.begin(), --n.end()) * (n.back() / 2 + 1);
    // For an in-place real-to-complex transform, the input size needs to be padded
    const int padded_real_elems = 2 * complex_elems;

    // For 2D in-place R2C transform we need to have equal input and output stride
    // otherwise we can overwrite some input locations with the output.
    // TODO: Relax this test if this limitation is lifted
    const int r2c_istride = r2c_inplace_strides,  r2c_ostride = r2c_inplace_strides;
    const int r2c_idist   = padded_real_elems * r2c_istride * r2c_sample_step;
    const int r2c_odist   =     complex_elems * r2c_ostride * r2c_sample_step;

    const int c2r_istride = r2c_ostride, c2r_idist = r2c_odist;
    const int c2r_ostride = c2r_output_stride;
    const int c2r_odist   = real_elems * c2r_ostride;

    int dists[] = {r2c_idist, r2c_odist, c2r_idist, c2r_odist};
    int max_dist = *std::max_element(std::begin(dists), std::end(dists));

    int howmany = set_howmany<T>(howmany_default, max_dist);

    test_info_t r2cinfo(howmany, n, r2c_istride, r2c_ostride, r2c_idist, r2c_odist);
    test_info_t c2rinfo(howmany, n, c2r_istride, c2r_ostride, c2r_idist, c2r_odist);
    r2cinfo.onembed.back() = (r2cinfo.onembed.back() / 2 + 1);
    c2rinfo.inembed.back() = (c2rinfo.inembed.back() / 2 + 1);
    return {r2cinfo, c2rinfo};
}

/**
 * @brief Calculate the strides of consecutive elements of each dimension for given shape.
 */
std::vector<int> get_strides(const std::vector<int> &shape) {
    std::vector<int> strides(shape.size());
    strides[shape.size() - 1] = 1;
    for (int i = shape.size() - 1; i > 0; i--) {
        strides[i - 1] = strides[i] * shape[i];
    }
    return strides;
}

/**
 * @brief Convert between flattened element offset that uses the original FFT shape without any padding
 * to a flat offset that includes the stride and inembed or onembed coordinates.
 */
int pad_offset(int element_offset, const std::vector<int> &shape, const std::vector<int> &embed, int stride) {
    auto strides_regular = get_strides(shape);
    auto strides_padded  = get_strides(embed);
    int result = 0;
    for (size_t r = 0; r < shape.size(); r++) {
        int coord = element_offset / strides_regular[r];
        element_offset -= coord * strides_regular[r];
        result += coord * strides_padded[r];
    }
    return result * stride;
}

/**
 * @brief Fill vector of batches of real data according to input layout specification passed
 * in info.
 */
template<typename T>
void fill_real_input_data(test_case_t tcase, std::vector<T> &target, const test_info_t &info) {
    if (tcase != test_case_t::FFT_iFFT) {
        std::cout << "Invalid test case" << std::endl;
        return;
    }
    target.resize(info.howmany * info.idist, -1);

    for (int sample_idx = 0; sample_idx < info.howmany; sample_idx++) {
        for (int i = 0; i < info.fft_size; i++) {
            int padded_offset = pad_offset(i, info.n, info.inembed, info.istride);
            target[sample_idx * info.idist + padded_offset] = sample_idx * info.fft_size + i;
        }
    }
}

template<typename T>
void compute_reference(test_case_t tcase, std::vector<T>& in, std::vector<T>& ref, test_info_t info){
    using scalar_type_t = typename type_util<T>::scalar_type;
    switch(tcase) {
        case test_case_t::FFT_iFFT:
            for (int b = 0; b < info.howmany; b++) {
                for (int i = 0; i < info.fft_size; i++) {
                    int padded_offset = pad_offset(i, info.n, info.inembed, info.istride);
                    int ref_idx = b * info.fft_size + i;
                    ref[ref_idx] = in[b * info.idist + padded_offset] * (scalar_type_t) info.fft_size;
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
            int padded_offset = pad_offset(i, info.n, info.onembed, info.ostride);
            int ref_idx = b * info.fft_size + i;
            // Note that std::norm(z) = z * conj(z), not the usual sqrt(z * conj(z))
            squared_diff += std::norm(ref[ref_idx] - out[b * info.odist + padded_offset]);
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

template<typename T>
struct statistics {
    T average, median, stdev, stdev_rel, pctl10, pctl90;
};

template<typename T>
statistics<T> compute_statistics(const std::vector<T>& v) {

    assert(v.size() > 0);

    std::vector<T> w = v;
    std::sort(w.begin(), w.end());
    statistics<T> stats;

    stats.median = w[w.size()/2];

    T sum = std::accumulate(v.begin(), v.end(), 0.0);
    stats.average = sum / v.size();

    T sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
    stats.stdev = sq_sum / v.size() - stats.average * stats.average;
    stats.stdev = stats.stdev > 0 ? std::sqrt(stats.stdev) : stats.stdev;

    stats.stdev_rel = stats.stdev / stats.average;

    stats.pctl10 = w[std::floor(w.size()*0.1)];      // 10th percentile
    stats.pctl90 = w[std::ceil (w.size()*0.9) - 1];  // 90th percentile

    return stats;
}

enum class fft_prec_t {
    SINGLE,
    DOUBLE
};

enum class fft_type_t {
    C2C,
    C2R,
    R2C
};

enum class fft_mode_t {
    IP,
    OOP,
};

std::string to_string(fft_prec_t prec) {
    switch (prec)
    {
    case fft_prec_t::SINGLE:
        return "fp32";
    case fft_prec_t::DOUBLE:
        return "fp64";
    default:
        return "<invalid>";
    }
};

std::string to_string(fft_type_t type) {
    switch (type)
    {
    case fft_type_t::C2C:
        return "c2c";
    case fft_type_t::C2R:
        return "c2r";
    case fft_type_t::R2C:
        return "r2c";
    default:
        return "<invalid>";
    }
};

std::string to_string(fft_mode_t mode) {
    switch (mode)
    {
    case fft_mode_t::IP:
        return "ip";
    case fft_mode_t::OOP:
        return "oop";
    default:
        return "<invalid>";
    }
};

#endif // NVPLFFT_EXAMPLE_COMMON_H_
