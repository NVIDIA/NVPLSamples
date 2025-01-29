#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <array>
#include <thread>
#include <chrono>
#include <cstring>
#include <string.h>

#include <nvpl_fftw.h>
#include <omp.h>
#include "common.h"

// This example measures the performance of batched 1D/2D/3D C2C/R2C/C2R FFTs with contiguous data.

constexpr int  stride_one = 1;
constexpr int* ionembed_nullptr = nullptr;


const char *get_preload_str() {
    static const char *str = []() {
        const char *ld_preload = std::getenv("LD_PRELOAD");
        if (!ld_preload) {
            return "none";
        } else if (strstr(ld_preload, "fftw-sve")) {
            return "fftw-sve";
        } else if (strstr(ld_preload, "fftw")) {
            return "fftw";
        } else if (strstr(ld_preload, "armpl")) {
            return "armpl";
        }
        return "none";
    }();
    return str;
}

struct data_size_desc {
  int count = MAX_SIZE_B;
  // true if count represents number of samples, false if count is data size in bytes
  bool is_batch = false;
};

enum class bench_category_t { p_2357, p_2357_r_1, f_2357_l_512_r_1, f_2357_l_512_r_2, varargs_r_1 };
struct bench_fft_config {
    bench_category_t bc = bench_category_t::p_2357;
    fft_type_t type = fft_type_t::C2C;
    fft_mode_t mode = fft_mode_t::OOP;
    std::string config_name_str;
    std::string bench_case_str;
    std::string type_str;
    std::string mode_str;
    bench_fft_config(std::string config_name_str, std::string bench_case_str, std::string fft_type_str, std::string mode_str, char **fft_sizes = nullptr, int size_count = 0) :
            config_name_str(config_name_str), bench_case_str(bench_case_str), type_str(fft_type_str), mode_str(mode_str) {

        if (bench_case_str.compare("p_2357") == 0) {
            bc = bench_category_t::p_2357;
        } else if (bench_case_str.compare("p_2357_r_1") == 0) {
            bc = bench_category_t::p_2357_r_1;
        } else if (bench_case_str.compare("f_2357_l_512_r_1") == 0){
            bc = bench_category_t::f_2357_l_512_r_1;
        } else if (bench_case_str.compare("f_2357_l_512_r_2") == 0){
            bc = bench_category_t::f_2357_l_512_r_2;
        } else if (bench_case_str.compare("varargs_r_1") == 0) {
            bc = bench_category_t::varargs_r_1;
            if (size_count == 0) {
                std::cout << "No fft sizes specifed, fall back to default (p_2357)" << std::endl;
                bench_case_str = "p_2357";
                bc = bench_category_t::p_2357;
            }
        } else {
            bc = bench_category_t::p_2357;
            bench_case_str = "p_2357";
            std::cout << "Invalid bench category " << bench_case_str <<". Fall back to default (p_2357)" << std::endl;
        }
        if (type_str.compare("c2c") == 0) {
            type = fft_type_t::C2C;
        } else if (type_str.compare("c2r") == 0) {
            type = fft_type_t::C2R;
        } else if (type_str.compare("r2c") == 0) {
            type = fft_type_t::R2C;
        } else {
            type = fft_type_t::C2C;
            type_str = "c2c";
            std::cout << "Invalid fft type " << type_str <<". Fall back to default (c2c)" << std::endl;
        }
        if (bc != bench_category_t::varargs_r_1) {
            if (size_count > 0) {
                std::cout << "Additional fft sizes that were passed will be ignored. Set `varargs_r_1` to control the fft sizes manually" << std::endl;
            }
        } else {
            for (int i = 0; i < size_count; i++) {
                sizes.push_back(std::stoi(fft_sizes[i]));
            }
        }
        if (mode_str.compare("ip") == 0) {
            mode = fft_mode_t::IP;
        } else if (mode_str.compare("oop") == 0) {
            mode = fft_mode_t::OOP;
        } else {
            mode = fft_mode_t::OOP;
            mode_str = "oop";
            std::cout << "Invalid transform mode " << mode_str <<". Fall back to default (oop)" << std::endl;
        }
    }
    std::array<int, 3> fft_rank_to_test() {
        switch (bc)
        {
        case bench_category_t::p_2357_r_1:
        case bench_category_t::varargs_r_1:
        case bench_category_t::f_2357_l_512_r_1:
            return {1};
        case bench_category_t::f_2357_l_512_r_2:
            return {2};
        default:
            return {1, 2, 3};
        }
    }
    std::vector<int> fft_size_per_dim_to_test() {
        switch (bc)
        {
        case bench_category_t::f_2357_l_512_r_1:
        case bench_category_t::f_2357_l_512_r_2:
            return {2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 24, 25, 27, 28, 30, 32, 35, 36,
                    40, 42, 45, 48, 49, 50, 54, 56, 60, 63, 64, 70, 72, 75, 80, 81, 84, 90, 96, 98, 100, 105,
                    108, 112, 120, 125, 126, 128, 135, 140, 144, 147, 150, 160, 162, 168, 175, 180, 189, 192,
                    196, 200, 210, 216, 224, 225, 240, 243, 245, 250, 252, 256, 270, 280, 288, 294, 300, 315,
                    320, 324, 336, 343, 350, 360, 375, 378, 384, 392, 400, 405, 420, 432, 441, 448, 450, 480,
                    486, 490, 500, 504, 512};
        case bench_category_t::varargs_r_1:
            return sizes;
        default:
            return {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, /* powers of 2 */
                    3, 9, 27, 81, 243, 729, 2187, 6561, 19683, /* powers of 3 */
                    5, 25, 125, 625, 3125, 15625, /* powers of 5 */
                    7, 49, 343, 2401, 16807 /* powers of 7 */};
        }
    }
    std::vector<int> sizes = {};
};

template<fft_prec_t prec>
struct fftw;

template<>
struct fftw<fft_prec_t::SINGLE> {
    using real_t    = float;
    using complex_t = fftwf_complex;
    using plan_t    = fftwf_plan;
    static int init_threads() { return fftwf_init_threads(); }
    static void plan_with_nthreads(int nthreads) { fftwf_plan_with_nthreads(nthreads); }
    static int planner_nthreads() { return fftwf_planner_nthreads(); }
    static plan_t plan_many(fft_type_t type, int rank, const int* n, int howmany, void* in, void* out, const int complex_elems) {
        switch (type)
        {
        case fft_type_t::C2C:
            return fftwf_plan_many_dft(rank, n, howmany, reinterpret_cast<complex_t*>(in), ionembed_nullptr, stride_one, complex_elems, reinterpret_cast<complex_t*>(out), ionembed_nullptr, stride_one, complex_elems, FFTW_FORWARD, FFTW_MEASURE);
        case fft_type_t::C2R:
            return fftwf_plan_many_dft_c2r(rank, n, howmany, reinterpret_cast<complex_t*>(in), ionembed_nullptr, stride_one, complex_elems, reinterpret_cast<real_t*>(out), ionembed_nullptr, stride_one, 2*complex_elems, FFTW_MEASURE);
        case fft_type_t::R2C:
            return fftwf_plan_many_dft_r2c(rank, n, howmany, reinterpret_cast<real_t*>(in), ionembed_nullptr, stride_one, 2*complex_elems, reinterpret_cast<complex_t*>(out), ionembed_nullptr, stride_one, complex_elems, FFTW_MEASURE);
        default:
            return nullptr;
        }
    }
    static void execute(plan_t plan) { fftwf_execute(plan); }
    static void destroy_and_cleanup(plan_t plan) {
        fftwf_destroy_plan(plan);
        fftwf_cleanup_threads();
        fftwf_cleanup();
    }
};

template<>
struct fftw<fft_prec_t::DOUBLE> {
    using real_t    = double;
    using complex_t = fftw_complex;
    using plan_t    = fftw_plan;
    static int init_threads() { return fftw_init_threads(); }
    static void plan_with_nthreads(int nthreads) { fftw_plan_with_nthreads(nthreads); }
    static int planner_nthreads() { return fftw_planner_nthreads(); }
    static plan_t plan_many(fft_type_t type, int rank, const int* n, int howmany, void* in, void* out, const int complex_elems) {
        switch (type)
        {
        case fft_type_t::C2C:
            return fftw_plan_many_dft(rank, n, howmany, reinterpret_cast<complex_t*>(in), ionembed_nullptr, stride_one, complex_elems, reinterpret_cast<complex_t*>(out), ionembed_nullptr, stride_one, complex_elems, FFTW_FORWARD, FFTW_MEASURE);
        case fft_type_t::C2R:
            return fftw_plan_many_dft_c2r(rank, n, howmany, reinterpret_cast<complex_t*>(in), ionembed_nullptr, stride_one, complex_elems, reinterpret_cast<real_t*>(out), ionembed_nullptr, stride_one, 2*complex_elems, FFTW_MEASURE);
        case fft_type_t::R2C:
            return fftw_plan_many_dft_r2c(rank, n, howmany, reinterpret_cast<real_t*>(in), ionembed_nullptr, stride_one, 2*complex_elems, reinterpret_cast<complex_t*>(out), ionembed_nullptr, stride_one, complex_elems, FFTW_MEASURE);
        default:
            return nullptr;
        }
    }
    static void execute(plan_t plan) { fftw_execute(plan); }
    static void destroy_and_cleanup(plan_t plan) {
        fftw_destroy_plan(plan);
        fftw_cleanup_threads();
        fftw_cleanup();
    }
};

double get_perf_GFlops(fft_type_t type, int fft_size, int howmany, double time_ms) {
    if (0 == time_ms) {
        return 0;
    }
    double factor = (type == fft_type_t::C2C) ? 5.0f : 2.5f;
    return factor * howmany * fft_size * log2(static_cast<double>(fft_size)) / 1e9 / (1e-3 * time_ms);
}

template<fft_prec_t prec>
double get_bandwidth_GBs(fft_type_t type, const std::vector<int>& n, int howmany, double time_ms) {
    using real_t    = typename fftw<prec>::real_t;
    using complex_t = typename fftw<prec>::complex_t;

    int complex_elem = (type == fft_type_t::C2C) ? get_product<int>(n.begin(), n.end()) : get_product<int>(n.begin(), --n.end()) * (n.back() / 2 + 1);
    int real_elem    = (type == fft_type_t::C2C) ? 0                                    : get_product<int>(n.begin(), n.end());

    double input_b = 0, output_b = 0;
    switch (type)
    {
    case fft_type_t::C2C:
        input_b  = howmany * complex_elem * sizeof(complex_t);
        output_b = howmany * complex_elem * sizeof(complex_t);
        break;
    case fft_type_t::C2R:
        input_b  = howmany * complex_elem * sizeof(complex_t);
        output_b = howmany * real_elem    * sizeof(real_t);
        break;
    case fft_type_t::R2C:
        input_b  = howmany * real_elem    * sizeof(real_t);
        output_b = howmany * complex_elem * sizeof(complex_t);
        break;
    default:
        break;
    }
    return (input_b + output_b) / 1e9 / (1e-3 * time_ms);
}

std::string get_test_case_string(const bench_fft_config& config, fft_prec_t prec, int nth, int rank, int fft_size_per_dim) {
    std::string str;
    str.append(config.bench_case_str); str.append("_");
    str.append(config.type_str); str.append("_");
    str.append(to_string(prec)); str.append("_");
    str.append(config.mode_str); str.append("_");
    str.append(std::to_string(omp_get_proc_bind())); str.append("_");
    str.append(std::to_string(nth)); str.append("_");
    str.append(std::to_string(rank)); str.append("_");
    str.append(std::to_string(fft_size_per_dim));
    return str;
}

template<fft_prec_t prec>
void test_performance(bench_fft_config config, int rank, int fft_size_per_dim, data_size_desc data_size, const int cycles, const int warmup_runs) {
    using real_t         = typename fftw<prec>::real_t;
    using fftw_complex_t = typename fftw<prec>::complex_t;
    if ((rank == 0) || (fft_size_per_dim == 0)) {
        return;
    }
    if(fftw<prec>::init_threads() == 0) {
        std::cout << "fftw(f)_init_threads() failed" << std::endl;
        return;
    }

    std::vector<int> n;
    for(int r = 0; r < rank; r++) {
        n.push_back(fft_size_per_dim);
    }

    int howmany = 0;
    if (data_size.is_batch) {
        howmany = data_size.count;
    } else {
        howmany = static_cast<int>((float) data_size.count / (float) (std::pow(fft_size_per_dim, rank) * sizeof(fftw_complex_t)));
    }
    if (howmany == 0) {
        return;
    }

    const int complex_elems = (config.type == fft_type_t::C2C) ? get_product<int>(n.begin(), n.end()) : get_product<int>(n.begin(), --n.end()) * (n.back() / 2 + 1);
    std::vector<std::complex<real_t>>  in(howmany * complex_elems);
    std::vector<std::complex<real_t>> out(howmany * complex_elems);

    fftw<prec>::plan_with_nthreads(omp_get_max_threads());

    auto plan = fftw<prec>::plan_many(config.type, n.size(), n.data(), howmany, in.data(), (config.mode == fft_mode_t::IP) ? in.data() : out.data(), complex_elems);
    if(plan == nullptr) {
        std::cout << "fftw(f)_plan_many_dft failed" << std::endl;
        return;
    }

    // FFTW requries creating plan before initializing the input (see https://www.fftw.org/fftw3_doc/Complex-One_002dDimensional-DFTs.html).
    for (int i = 0; i < howmany * complex_elems; i++) {
        in[i] = {(float) i, (float) -i};
    }

    // Warm up runs
    for (int i = 0; i < warmup_runs; i++) {
        fftw<prec>::execute(plan);
    }

    std::vector<double> timer;
    for(int cycle = 0; cycle < cycles; cycle++) {
        const auto start = std::chrono::high_resolution_clock::now();
        fftw<prec>::execute(plan);
        const auto now = std::chrono::high_resolution_clock::now();
        timer.push_back((double) std::chrono::duration_cast<std::chrono::microseconds>(now - start).count() / 1e3);
    }

    auto stats = compute_statistics(timer);
    double average_time_ms       = stats.median; // use median for more reliable statistics
    double average_perf_GFlops   = get_perf_GFlops(config.type, get_product<int>(n.begin(), n.end()), howmany, average_time_ms);
    double average_bandwidth_GBs = get_bandwidth_GBs<prec>(config.type, n, howmany, average_time_ms);

    printf("++++CSV-Data, %s, %s, %s, %d, %d, %s, %s, %s, %ld, %d, %d, %d, %d, %d, %d, %e, %e, %e, %e, %e, %e, %e\n",
        get_test_case_string(config, prec, fftw<prec>::planner_nthreads(), rank, fft_size_per_dim).c_str(),
        config.config_name_str.c_str(), get_preload_str(),
        omp_get_proc_bind(), fftw<prec>::planner_nthreads(),
        config.type_str.c_str(), to_string(prec).c_str(), config.mode_str.c_str(),
        n.size(), n[0], n.size() > 1 ? n[1] : 0, n.size() > 2 ? n[2] : 0, howmany, cycles, warmup_runs, stats.average, stats.median, stats.stdev, stats.pctl10, stats.pctl90, average_perf_GFlops, average_bandwidth_GBs);

    /* Free resources */
    fftw<prec>::destroy_and_cleanup(plan);
}

data_size_desc parse_data_size(char *data_size) {
    auto len = std::strlen(data_size);
    static char size_suffix[] = {'k', 'K', 'm', 'M'};
    data_size_desc result;
    result.count = std::stoi(data_size);
    result.is_batch = true;
    if (result.count == 0) {
        return {MAX_SIZE_B, false};
    }
    for (size_t unit_index = 0; unit_index < sizeof(size_suffix) / sizeof(char); unit_index++) {
        if (data_size[len - 1] == size_suffix[unit_index]) {
            result.is_batch = false;
            for (size_t i = 0; i < unit_index / 2 + 1; i++) {
                result.count *= 1024;
            }
        }
    }
    return result;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    if(argc == 1) {
        std::cout<<"Usage: ./c2c_c2r_r2c_many_bench_example\n"
                 <<"Arguments:\n"
                 <<"\t--prec precision:          The precision of the transform fp32 or fp64.\n"
                 <<"\t--fft_type fft_type:       The type of the transform c2c, r2c or c2r.\n"
                 <<"\t--mode mode:               (optional) The mode of the transform ip or oop (default: ip).\n"
                 <<"\t--config config_name:      (optional) Name of the config to be logged (default: no_config).\n"
                 <<"\t--cat bench_category:      (optional) The case to benchmark p_2357, f_2357_l_512_r_1, f_2357_l_512_r_2, varargs_r_1 (default: p_2357).\n"
                 <<"\t--size data_size:          (optional) Transform data size. Supported options: \n"
                 <<"\t                           * 0 - default, total data size is 256 MB.\n"
                 <<"\t                           * <number> - number of batches to process for each FFT size.\n"
                 <<"\t                           * <number>k or <number>m - for example 64m - the size of data in KB or MB to process.\n"
                 <<"\t--cycles cycles:           (optional) The number of cycles (default: 100).\n"
                 <<"\t--warmup warmup_runs:      (optional) The number of warm-up runs (default: 10).\n"
                 <<"\t--fft_sizes *fft_sizes:    (optional) If `varargs_r_1` is selected, fft sizes can be listed manually (for rank 1). This must be the last argument!\n"
                 <<std::endl;
        return EXIT_SUCCESS;
    }
    int cycles = 100;
    int warmup_runs = 10;
    data_size_desc data_size = {};
    char **fft_sizes = nullptr;
    std::string bench_cat_str = "p_2357";

    int arg_pointer = 1;
    std::string prec_str, type_str, mode_str = "ip", config_name_str = "no_config";

    while (arg_pointer < argc) {
        if (strcmp(argv[arg_pointer], "--prec") == 0) {
            prec_str = argv[arg_pointer + 1];
        } else if (strcmp(argv[arg_pointer], "--fft_type") == 0) {
            type_str = argv[arg_pointer + 1];
        } else if (strcmp(argv[arg_pointer], "--mode") == 0) {
            mode_str = argv[arg_pointer + 1];
        } else if (strcmp(argv[arg_pointer], "--config") == 0) {
            config_name_str = argv[arg_pointer + 1];
        } else if (strcmp(argv[arg_pointer], "--cat") == 0) {
            bench_cat_str = argv[arg_pointer + 1];
        } else if (strcmp(argv[arg_pointer], "--size") == 0) {
            data_size = parse_data_size(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--cycles") == 0) {
            cycles = std::stoi(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--warmup") == 0) {
            warmup_runs = std::stoi(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--fft_sizes") == 0) {
            fft_sizes = &argv[arg_pointer + 1];
            break;
        }
        arg_pointer += 2;
    }
    if (prec_str.empty() || type_str.empty()) {
        fprintf(stderr, "Mandatory --prec or --fft_type were not provided\n");
        return EXIT_FAILURE;
    }

    bench_fft_config config(config_name_str, bench_cat_str, type_str, mode_str, fft_sizes, argc > arg_pointer + 1 ? argc - arg_pointer - 1: 0);

    printf("++++CSV-Header, s_test_case, s_config_name, s_preload, l_omp_proc_bind, l_nthreads,"
           " s_fft_type, s_fft_precision, s_fft_mode,"
           " l_rank, l_nx, l_ny, l_nz, l_batch, l_cycles, l_warmup_cycles,"
           " d_time_average_ms, d_time_median_ms, d_time_stddev_ms, d_time_10pctl_ms,"
           " d_time_90pctl_ms, d_perf_GFlops, d_bandwidth_GBs\n");

    for(int rank : config.fft_rank_to_test()) {
        for (int fft_size_per_dim : config.fft_size_per_dim_to_test()) {
            if (prec_str.compare("fp32") == 0) {
                test_performance<fft_prec_t::SINGLE>(config, rank, fft_size_per_dim, data_size, cycles, warmup_runs);
            } else if (prec_str.compare("fp64") == 0) {
                test_performance<fft_prec_t::DOUBLE>(config, rank, fft_size_per_dim, data_size, cycles, warmup_runs);
            } else {
                fprintf(stderr, "Parameter error: invalid precision %s\n", prec_str.c_str());
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}
