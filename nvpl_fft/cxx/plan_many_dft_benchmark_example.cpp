#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <array>
#include <thread>
#include <chrono>
#include <cstring>
#include <string.h>
#include <optional>

#include <nvpl_fftw.h>
#include <omp.h>
#include "common.h"

// This example measures the performance of batched 1D/2D/3D C2C/R2C/C2R FFTs with any data layout.
// Note, that we assume that inembed and onembed are NULL, indicating the default padding explained
// in the FFTW documentation:
// https://stackoverflow.com/questions/16905944/fftw-advanced-layout-inembed-n-and-inembed-null-give-different-results/17583226#17583226
// http://www.fftw.org/doc/Advanced-Real_002ddata-DFTs.html
// http://www.fftw.org/doc/Multi_002dDimensional-DFTs-of-Real-Data.html
// http://www.fftw.org/doc/Real_002ddata-DFT-Array-Format.html
constexpr int* ionembed_nullptr = nullptr;

// The real dist and stride is treated in the units of floats/double, and for the complex elements
// it's treated in the units of complex elements.


const char *get_preload_str() {
    static const char *str = []() {
        const char *ld_preload = std::getenv("LD_PRELOAD");
        if (!ld_preload) {
            return "none";
        } else if (strstr(ld_preload, "nvpl")) {
            return "nvpl";
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

fft_type_t get_fft_type(char *type_str) {
    if (strncmp(type_str, "c2c", 3) == 0) {
        return fft_type_t::C2C;
    } else if (strncmp(type_str, "c2r", 3) == 0) {
        return fft_type_t::C2R;
    } else if (strncmp(type_str, "r2c", 3) == 0) {
        return fft_type_t::R2C;
    }
    std::cout << "Invalid fft type " << type_str <<". Fall back to default (c2c)" << std::endl;
    return fft_type_t::C2C;
}

fft_mode_t get_fft_mode(char *mode_str) {
    if (strncmp(mode_str, "ip", 2) == 0) {
        return fft_mode_t::IP;
    } else if (strncmp(mode_str, "oop", 3) == 0) {
        return fft_mode_t::OOP;
    }
    std::cout << "Invalid transform mode " << mode_str <<". Fall back to default (oop)" << std::endl;
    return fft_mode_t::OOP;
}


bool parse_rank_array(const char* arg_name, int rank, int argc, char** argv, int& arg_pointer, std::vector<int>& output) {
    if (arg_pointer + rank >= argc) {
        fprintf(stderr, "Error: %s requires %d values for rank %d\n", arg_name, rank, rank);
        return false;
    }
    for (int r = 0; r < rank; r++) {
        try {
            output.push_back(std::stoi(argv[arg_pointer + 1 + r]));
        } catch (const std::exception& e) {
            std::cout << "Error parsing " << arg_name << "[" << r << "]: " << e.what() << std::endl;
            return false;
        }
    }
    // we assume that we move the arg_pointer by the param name and first rank size,
    // we need to move by the other (optional) ranks
    arg_pointer += (rank - 1);

    return true;
}


/**
 * @brief Get the number of complex elements in the FFT. Takes into account the fact that the last
 * dimension is approximately halved for the R2C (and C2R) transforms.
 */
int get_complex_elems(const std::vector<int> &fft_size, fft_type_t type) {
    if (type == fft_type_t::C2C) {
        return get_product<int>(fft_size.begin(), fft_size.end());
    }
    return get_product<int>(fft_size.begin(), --fft_size.end()) * (fft_size.back() / 2 + 1);
}

struct bench_fft_config {
    std::string config_name_str;
    fft_type_t type = fft_type_t::C2C;
    fft_mode_t mode = fft_mode_t::OOP;
    bench_fft_config(std::string config_name_str, fft_type_t type, fft_mode_t mode):
            config_name_str(config_name_str), type(type), mode(mode) {
    }
};

struct striding_confing {
    std::optional<int> istride;
    std::optional<int> idist;
    std::optional<int> ostride;
    std::optional<int> odist;
    std::optional<std::vector<int>> inembed;
    std::optional<std::vector<int>> onembed;
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
    static plan_t plan_many(fft_type_t type, int rank, const int* n, int howmany,
                            void* in,  const int* inembed, int istride, int idist,
                            void* out, const int* onembed, int ostride, int odist) {
        switch (type)
        {
        case fft_type_t::C2C:
            return fftwf_plan_many_dft(rank, n, howmany,
                                       reinterpret_cast<complex_t*>(in),
                                       inembed, istride, idist,
                                       reinterpret_cast<complex_t*>(out),
                                       onembed, ostride, odist,
                                       FFTW_FORWARD, FFTW_MEASURE);
        case fft_type_t::C2R:
            return fftwf_plan_many_dft_c2r(rank, n, howmany,
                                           reinterpret_cast<complex_t*>(in),
                                           inembed, istride, idist,
                                           reinterpret_cast<real_t*>(out),
                                           onembed, ostride, odist,
                                           FFTW_MEASURE);
        case fft_type_t::R2C:
            return fftwf_plan_many_dft_r2c(rank, n, howmany,
                                           reinterpret_cast<real_t*>(in),
                                           inembed, istride, idist,
                                           reinterpret_cast<complex_t*>(out),
                                           onembed, ostride, odist,
                                           FFTW_MEASURE);
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
    static plan_t plan_many(fft_type_t type, int rank, const int* n, int howmany,
                            void* in, const int* inembed, int istride, int idist,
                            void* out, const int* onembed, int ostride, int odist) {
        switch (type)
        {
        case fft_type_t::C2C:
            return fftw_plan_many_dft(rank, n, howmany,
                                      reinterpret_cast<complex_t*>(in),
                                      inembed, istride, idist,
                                      reinterpret_cast<complex_t*>(out),
                                      onembed, ostride, odist,
                                      FFTW_FORWARD, FFTW_MEASURE);
        case fft_type_t::C2R:
            return fftw_plan_many_dft_c2r(rank, n, howmany,
                                          reinterpret_cast<complex_t*>(in),
                                          inembed, istride, idist,
                                          reinterpret_cast<real_t*>(out),
                                          onembed, ostride, odist,
                                          FFTW_MEASURE);
        case fft_type_t::R2C:
            return fftw_plan_many_dft_r2c(rank, n, howmany,
                                          reinterpret_cast<real_t*>(in),
                                          inembed, istride, idist,
                                          reinterpret_cast<complex_t*>(out),
                                          onembed, ostride, odist,
                                          FFTW_MEASURE);
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
    if (0 == time_ms) {
        return 0;
    }

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

template<fft_prec_t prec>
void test_performance(const std::string &test_cmd, bench_fft_config config, int rank, const std::vector<int> &fft_size,
                      data_size_desc data_size, const int cycles, const int warmup_runs,
                      const striding_confing strides, bool detailed_timing = false) {
    using real_t         = typename fftw<prec>::real_t;
    using fftw_complex_t = typename fftw<prec>::complex_t;
    if ((rank == 0) || (fft_size.empty())) {
        return;
    }
    if(fftw<prec>::init_threads() == 0) {
        std::cout << "fftw(f)_init_threads() failed" << std::endl;
        return;
    }

    const int complex_elems = get_complex_elems(fft_size, config.type);

    const int istride = strides.istride.value_or(1);
    const int ostride = strides.ostride.value_or(1);
    const int idist = strides.idist.value_or(
        config.type == fft_type_t::R2C ? complex_elems * istride * 2 : complex_elems * istride);
    const int odist = strides.odist.value_or(
        config.type == fft_type_t::C2R ? complex_elems * ostride * 2 : complex_elems * ostride);

    int howmany = 0;
    if (data_size.is_batch) {
        howmany = data_size.count;
    } else {
        howmany = static_cast<int>((float) data_size.count / (float) (idist * sizeof(fftw_complex_t)));
    }
    if (howmany == 0) {
        return;
    }

    if (config.mode == fft_mode_t::IP) {
        // There are additional requirements
        if (rank >= 2) {
            if (istride != ostride) {
                std::cout << "For in-place transforms of rank >= 2 the strides of input and output"
                             " must match." <<std::endl;
            }
        }
    }

    // Calculate the size needed for input/output arrays based on embed dimensions
    int in_size = 0;
    int out_size = 0;

    if (strides.inembed) {
        in_size = get_product<int>(strides.inembed->begin(), strides.inembed->end());
    } else {
        in_size = complex_elems;
    }

    if (strides.onembed) {
        out_size = get_product<int>(strides.onembed->begin(), strides.onembed->end());
    } else {
        out_size = complex_elems;
    }

    // Allocate vectors with the appropriate sizes
    std::vector<std::complex<real_t>>  in(howmany * idist + istride * in_size);
    std::vector<std::complex<real_t>> out(howmany * odist + ostride * out_size);

    fftw<prec>::plan_with_nthreads(omp_get_max_threads());

    auto *out_ptr = (config.mode == fft_mode_t::IP) ? in.data() : out.data();
    auto plan = fftw<prec>::plan_many(config.type, rank, fft_size.data(), howmany,
                                      in.data(),
                                      strides.inembed ? strides.inembed->data() : ionembed_nullptr,
                                      istride, idist,
                                      out_ptr,
                                      strides.onembed ? strides.onembed->data() : ionembed_nullptr,
                                      ostride, odist);

    if(plan == nullptr) {
        std::cout << "fftw(f)_plan_many_dft failed" << std::endl;
        return;
    }

    // FFTW requries creating plan before initializing the input
    // (see https://www.fftw.org/fftw3_doc/Complex-One_002dDimensional-DFTs.html).
    for (unsigned int i = 0; i < in.size(); i++) {
        in[i] = {(float) i, (float) -i};
    }

    // Warm up runs
    for (int i = 0; i < warmup_runs; i++) {
        fftw<prec>::execute(plan);
    }

    double average_time_ms, average_perf_GFlops, average_bandwidth_GBs;
    statistics<double> stats = {};  // Initialize empty statistics

    if (!detailed_timing) {
        // variant 1 - simple timing with lower overhead
        const auto start = std::chrono::steady_clock::now();
        for(int cycle = 0; cycle < cycles; cycle++) {
            fftw<prec>::execute(plan);
        }
        const auto end = std::chrono::steady_clock::now();

        average_time_ms = ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1e6) / cycles;
    } else {
        // variant 2 - detailed timing with statistics
        std::vector<double> timer;
        timer.reserve(cycles);
        for(int cycle = 0; cycle < cycles; cycle++) {
            const auto start = std::chrono::steady_clock::now();
            fftw<prec>::execute(plan);
            const auto now = std::chrono::steady_clock::now();
            timer.push_back((double) std::chrono::duration_cast<std::chrono::nanoseconds>(now - start).count() / 1e6);
        }

        stats = compute_statistics(timer);
        average_time_ms = stats.median;
    }

    average_perf_GFlops = get_perf_GFlops(config.type, get_product<int>(fft_size.begin(), fft_size.end()), howmany, average_time_ms);
    average_bandwidth_GBs = get_bandwidth_GBs<prec>(config.type, fft_size, howmany, average_time_ms);

    // Common data format with conditional statistics
    printf("++++CSV-Data,"
           " %s, "                                // test command
           " %s, %s,"                             // config name, preload
           " %d, %d,"                             // proc_bind, max_threads
           " %s, %s, %s,"                         // fft type, precision, mode
           " %ld,"                                // fft_size.size()
           " %d, %d, %d,"                         // nx, ny, nz
           " %d,"                                 // howmany
           " %d, %d, %d, %d,"                     // strides and distances
           " %d, %d,"                             // cycles, warmup
           " %e, %e, %e",                         // basic stats: time, perf, bandwidth
           test_cmd.c_str(),
           config.config_name_str.c_str(), get_preload_str(),
           omp_get_proc_bind(), omp_get_max_threads(),
           to_string(config.type).c_str(), to_string(prec).c_str(), to_string(config.mode).c_str(),
           fft_size.size(),
           fft_size[0], fft_size.size() > 1 ? fft_size[1] : 0, fft_size.size() > 2 ? fft_size[2] : 0,
           howmany,
           istride, idist, ostride, odist,
           cycles, warmup_runs,
           average_time_ms, average_perf_GFlops, average_bandwidth_GBs);

    if (detailed_timing) {
        printf(", %e, %e, %e, %e, %e",       // detailed stats
               stats.average, stats.median, stats.stdev,
               stats.pctl10, stats.pctl90);
    }

    printf("\n");  // End the line

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

std::string get_test_cmd(int argc, char *argv[], int nthreads) {
    std::stringstream ss;

    ss << "OMP_NUM_THREADS=" << nthreads << " ";

    // For the first argument (program name), remove directory path
    if (argc > 0) {
        std::string program = argv[0];
        size_t pos = program.find_last_of("/\\");
        if (pos != std::string::npos) {
            // Extract only the program name without path
            ss << program.substr(pos + 1);
        } else {
            // No path separator found, use as is
            ss << program;
        }
    }

    // Add all other arguments
    for (int i = 1; i < argc; i++) {
        ss << " " << argv[i];
    }
    return ss.str();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    if(argc == 1) {
        std::cout<<"Usage: ./plan_many_dft_benchmark_example\n"
                 <<"Arguments:\n"
                 <<"\t--prec precision:          The precision of the transform fp32 or fp64.\n"
                 <<"\t--fft_type fft_type:       (optional) The type of the transform c2c, r2c or c2r (default: c2c).\n"
                 <<"\t--mode mode:               (optional) The mode of the transform ip or oop (default: ip).\n"
                 <<"\t--rank rank:               (optional) Rank of the transform (default: 1).\n"
                 <<"\t--size data_size:          (optional) Transform data size. Supported options: \n"
                 <<"\t                           * 0 - default, total data size is 256 MB.\n"
                 <<"\t                           * <number> - number of batches to process for each FFT size.\n"
                 <<"\t                           * <number>k or <number>m - for example 64m - the size of data in KB or MB to process.\n"
                 <<"\t--cycles cycles:           (optional) The number of cycles (default: 100).\n"
                 <<"\t--warmup warmup_runs:      (optional) The number of warm-up runs (default: 10).\n"
                 <<"\t--fft_sizes *fft_sizes:    (optional) Size of the fft transform. If the rank != 1, it must be specified earlier!\n"
                 <<"\t--inembed *inembed:        (optional) Input array embedding. Must match rank size if specified.\n"
                 <<"\t--onembed *onembed:        (optional) Output array embedding. Must match rank size if specified.\n"
                 <<"\t--istride istride:         (optional) Input stride - distance between elements of the sample (default: 1)\n"
                 <<"\t--idist idist:             (optional) Distance between start of each input sample (~ number of transformed elements)\n"
                 <<"\t--ostride ostride:         (optional) Output stride - distance between elements of the sample (default: 1)\n"
                 <<"\t--odist odist:             (optional) Distance between start of each output sample (~ number of transformed elements)\n"
                 <<"\t--detailed_timing:         (optional) Enable detailed timing statistics with median calculation\n"
                 <<std::endl;
        return EXIT_SUCCESS;
    }
    int cycles = 100;
    int warmup_runs = 10;
    int rank = 1;
    data_size_desc data_size = {};
    std::vector<int> fft_sizes;
    striding_confing strides;
    bool detailed_timing = false;

    int arg_pointer = 1;

    std::string prec_str, config_name_str = "no_config";
    fft_type_t type = fft_type_t::C2C;
    fft_mode_t mode = fft_mode_t::IP;

    std::string header_str = "all";

    while (arg_pointer < argc) {
         if (strcmp(argv[arg_pointer], "--header") == 0) {
            header_str = argv[arg_pointer + 1];
        } else if (strcmp(argv[arg_pointer], "--prec") == 0) {
            prec_str = argv[arg_pointer + 1];
        } else if (strcmp(argv[arg_pointer], "--fft_type") == 0) {
            type = get_fft_type(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--mode") == 0) {
            mode = get_fft_mode(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--config") == 0) {
            config_name_str = argv[arg_pointer + 1];
        } else if (strcmp(argv[arg_pointer], "--rank") == 0) {
            rank = std::stoi(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--size") == 0) {
            data_size = parse_data_size(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--cycles") == 0) {
            cycles = std::stoi(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--warmup") == 0) {
            warmup_runs = std::stoi(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--fft_sizes") == 0) {
            if (!parse_rank_array("--fft_sizes", rank, argc, argv, arg_pointer, fft_sizes)) {
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[arg_pointer], "--inembed") == 0) {
            strides.inembed = std::vector<int>();
            if (!parse_rank_array("--inembed", rank, argc, argv, arg_pointer, strides.inembed.value())) {
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[arg_pointer], "--onembed") == 0) {
            strides.onembed = std::vector<int>();
            if (!parse_rank_array("--onembed", rank, argc, argv, arg_pointer, strides.onembed.value())) {
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[arg_pointer], "--istride") == 0) {
            strides.istride = std::stoi(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--ostride") == 0) {
            strides.ostride = std::stoi(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--idist") == 0) {
            strides.idist = std::stoi(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--odist") == 0) {
            strides.odist = std::stoi(argv[arg_pointer + 1]);
        } else if (strcmp(argv[arg_pointer], "--detailed_timing") == 0) {
            detailed_timing = true;
            arg_pointer += 1;  // Only increment by 1 since this is a flag without value
            continue;  // Skip the normal increment of 2
        }
        arg_pointer += 2;
    }

    bench_fft_config config(config_name_str, type, mode);

    // Get complete test command string
    std::string test_cmd = get_test_cmd(argc, argv, omp_get_max_threads());

    if (header_str == "all" || header_str == "header") {
        printf("++++CSV-Header,"
               " s_test_cmd, "
               " s_config_name, s_preload,"
               " l_omp_proc_bind, l_nthreads,"
               " s_fft_type, s_fft_precision, s_fft_mode,"
               " l_rank,"
               " l_nx, l_ny, l_nz,"
               " l_batch,"
               " l_istride, l_idist, l_ostride, l_odist,"
               " l_cycles, l_warmup_cycles,"
               " d_time_ms, d_perf_GFlops, d_bandwidth_GBs");

        if (detailed_timing) {
            printf(", d_time_avg, d_time_median, d_time_stdev,"
                   " d_time_pctl10, d_time_pctl90");
        }

        printf("\n");
    }

    if (header_str == "header") {
        return EXIT_SUCCESS;
    }

    // body

    if (prec_str.compare("fp32") == 0) {
        test_performance<fft_prec_t::SINGLE>(test_cmd, config, rank, fft_sizes, data_size, cycles, warmup_runs, strides, detailed_timing);
    } else if (prec_str.compare("fp64") == 0) {
        test_performance<fft_prec_t::DOUBLE>(test_cmd, config, rank, fft_sizes, data_size, cycles, warmup_runs, strides, detailed_timing);
    } else {
        fprintf(stderr, "Parameter error: invalid precision %s\n", prec_str.c_str());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
