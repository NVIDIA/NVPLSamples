#include <stdio.h>
#include <cmath>
#include <vector>
#include <array>
#include <thread>
#include <chrono>

#include <nvpl_fftw.h>
#include <omp.h>
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

void test_performance(int rank, int fft_size_per_dim, const int cycles, const int warmup_runs) {
    if(fftwf_init_threads() == 0) {
        std::cout << "fftw_init_threads() failed" << std::endl;
        return;
    }

    std::vector<int> n;
    for(int r = 0; r < rank; r++) {
        n.push_back(fft_size_per_dim);
    }

    const int howmany = static_cast<int>((float) MAX_SIZE_B / (float) (std::pow(fft_size_per_dim, rank) * sizeof(fftwf_complex)));
    if (howmany == 0) {
        return;
    }
    test_info_t info(howmany, n);

    std::vector<std::complex<float>>  in(info.howmany * info.fft_size);

    fftwf_complex *in_data  = reinterpret_cast<fftwf_complex*>(in.data());

    fftwf_plan_with_nthreads(omp_get_max_threads());
    fftwf_plan plan = fftwf_plan_many_dft(info.n.size(), info.n.data(), info.howmany, in_data, nullptr, info.istride, info.idist,
                                                                                      in_data, nullptr, info.ostride, info.odist, FFTW_FORWARD, FFTW_PATIENT);
    if(plan == nullptr) {
        std::cout << "fftwf_plan_many_dft failed" << std::endl;
        return;
    }

    // FFTW requries creating plan before initializing the input (see https://www.fftw.org/fftw3_doc/Complex-One_002dDimensional-DFTs.html).
    for (int i = 0; i < info.howmany * info.fft_size; i++) {
        in[i] = {(float) i, (float) -i};
    }

    for (int i = 0; i < warmup_runs; i++) {
        fftwf_execute_dft(plan, in_data, in_data);
    }

    std::vector<double> timer;

    for(int cycle = 0; cycle < cycles; cycle++) {
        // Measure start
        const auto start = std::chrono::high_resolution_clock::now();

        fftwf_execute_dft(plan, in_data, in_data);

        // Measure stop
        const auto now = std::chrono::high_resolution_clock::now();

        timer.push_back((double) std::chrono::duration_cast<std::chrono::microseconds>(now - start).count() / 1e3);
    }

    auto stats = compute_statistics(timer);
    double average_time_ms = stats.median; // use median for more reliable statistics
    double average_perf_GFlops   = get_perf_GFlops(info.fft_size, howmany, average_time_ms);
    double average_bandwidth_GBs = get_bandwidth_GBs(info.fft_size, howmany, average_time_ms);

    printf("C2C FFT: %ldD, size: (%8d; %8d; %8d), batch size: %8d, cycles: %8d, warmup_runs: %8d, time_average[ms]: %7.2f, time_median[ms]: %7.2f, time_stddev[ms]: %7.2f, time_10pctl[ms]: %7.2f, time_90pctl[ms]: %7.2f, perf[GFlop/s]: %7.2e, bandwidth[GB/s]: %7.2e\n",
        info.n.size(), info.n[0], info.n.size() > 1 ? info.n[1] : 0, info.n.size() > 2 ? info.n[2] : 0, info.howmany, cycles, warmup_runs, stats.average, stats.median, stats.stdev, stats.pctl10, stats.pctl90, average_perf_GFlops, average_bandwidth_GBs);

    /* Free resources */
    fftwf_destroy_plan(plan);
    fftwf_cleanup_threads();
    fftwf_cleanup();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {

    int cycles = 100;
    int warmup_runs = 10;

    if (argc < 3) {
        std::cout<<"Usage: ./c2c_many_bench_example cycles warmup_runs\n"
                 <<"Arguments:\n"
                 <<"\tcycles:      The number of cycles (default: 100).\n"
                 <<"\twarmup_runs: The number of warm-up runs (default: 10).\n" <<std::endl;
    }

    std::string test_cmd = get_cmd_string(argc, argv);

    if(argc > 1) cycles = std::stoi(argv[1]);
    if(argc > 2) warmup_runs = std::stoi(argv[2]);

    for(int rank = 1; rank <= 3; rank++) {
        for (int fft_size_per_dim : fft_size_per_dim_to_test) {
            test_performance(rank, fft_size_per_dim, cycles, warmup_runs);
        }
    }
    return EXIT_SUCCESS;
}
