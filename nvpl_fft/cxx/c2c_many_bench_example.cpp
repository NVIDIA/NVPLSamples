#include <stdio.h>
#include <cmath>
#include <vector>
#include <array>
#include <thread>
#include <chrono>

#include <nvpl_fftw.h>
#include "common.h"

// This example measures the performance of batched 1D/2D/3D C2C FFTs in single precision with contiguous data.
// Note:
// * Tested sizes include powers of 2, 3, 5, 7 (with max 50K)
// * Number of batches are chosen so that data is ~256 MB

constexpr std::array<int, 35> fft_size_per_dim_to_test = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, /* powers of 2 */
                                                          3, 9, 27, 81, 243, 729, 2187, 6561, 19683, /* powers of 3 */
                                                          5, 25, 125, 625, 3125, 15625, /* powers of 5 */
                                                          7, 49, 343, 2401, 16807 /* powers of 7 */};

double get_perf_GFlops(int fft_size, int howmany, double time_ms) {
    return 5.0 * howmany * fft_size * log2(static_cast<double>(fft_size)) / 1e9 / (1e-3 * time_ms);
}

double get_bandwidth_GBs(int fft_size, int howmany, double time_ms) {
    return (static_cast<double>(2 * howmany * fft_size * sizeof(fftwf_complex))) / 1e9 / (1e-3 * time_ms);
}

void test_performance(int rank, int fft_size_per_dim) {
    if(fftwf_init_threads() == 0) {
        std::cout << "fftw_init_threads() failed" << std::endl;
        return;
    }

    int cycles = 100;
    std::vector<int> n;
    for(int r = 0; r < rank; r++) {
        n.push_back(fft_size_per_dim);
    }
    int howmany = static_cast<int>((float) MAX_SIZE_B / (float) (std::pow(fft_size_per_dim, rank) * sizeof(fftwf_complex)));
    if (howmany == 0) {
        return;
    }
    test_info_t info(howmany, n);

    std::vector<std::complex<float>>  in(info.howmany * info.fft_size);

    fftwf_complex *in_data  = reinterpret_cast<fftwf_complex*>(in.data());

    fftwf_plan_with_nthreads(std::thread::hardware_concurrency());
    fftwf_plan plan = fftwf_plan_many_dft(info.n.size(), info.n.data(), info.howmany, in_data, nullptr, info.istride, info.idist,
                                                                                      in_data, nullptr, info.ostride, info.odist, FFTW_FORWARD, FFTW_PATIENT);

    // FFTW requries creating plan before initializing the input (see https://www.fftw.org/fftw3_doc/Complex-One_002dDimensional-DFTs.html).
    for (int i = 0; i < info.howmany * info.fft_size; i++) {
        in[i] = {(float) i, (float) -i};
    }

    // Measure start
    const auto start = std::chrono::high_resolution_clock::now();

    for(int cycle = 0; cycle < cycles; cycle++) {
        fftwf_execute_dft(plan, in_data, in_data);
    }

    // Measure stop
    const auto now = std::chrono::high_resolution_clock::now();
    const float time_ms = (float) std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
    float average_time_ms = time_ms / cycles;
    double average_perf_GFlops   = get_perf_GFlops(info.fft_size, howmany, average_time_ms);
    double average_bandwidth_GBs = get_bandwidth_GBs(info.fft_size, howmany, average_time_ms);

    printf("%ldD C2C FFT size (%8d, %8d, %8d), batch size %8d, time %7.2f [ms], perf %7.2e [GFlop/s], bandwidth %7.2e [GB/s]\n", 
        info.n.size(), info.n[0], info.n.size() > 1 ? info.n[1] : 0, info.n.size() > 2 ? info.n[2] : 0, info.howmany, average_time_ms, average_perf_GFlops, average_bandwidth_GBs);

    /* Free resources */
    fftwf_destroy_plan(plan);
    fftwf_cleanup_threads();
    fftwf_cleanup();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    for(int rank = 1; rank <= 3; rank++) {
        for (int fft_size_per_dim : fft_size_per_dim_to_test) {
            test_performance(rank, fft_size_per_dim);
        }
    }
    return EXIT_SUCCESS;
}
