#include <vector>
#include <nvpl_fftw.h>
#include "common.h"
#include <omp.h>

constexpr int batches_per_exec = 100;
#if defined(__NVCOMPILER)
constexpr int nthread_fftw = 1; // NVHPC OpenMP does not support nested parallel regions on CPU (see https://docs.nvidia.com/hpc-sdk//compilers/hpc-compilers-user-guide/index.html#openmp-subset)
#else
constexpr int nthread_fftw = 72;
#endif
constexpr int nthread_user = 2;

// This example computes a forward and a backward 1D C2C FFT in single precision with contiguous data.
// It demonstrates how user can call plan + execution functions inside a omp parallel region. The omp threads share plans but operate on different parts of input, output data.
int run_test(const std::string &test_cmd, test_scenario_t tsce, const test_info_t& info) {
    if(tsce == test_scenario_t::ONE_PLAN_PER_PROCESS && (info.howmany != nthread_user * batches_per_exec)) {
        std::cout << "test_scenario_t::ONE_PLAN_PER_PROCESS: howmany must equal to nthread_user x batches_per_exec -- skipped" << std::endl;
        return 0;
    }
    const int total_size = info.howmany * info.fft_size;

    std::vector<std::complex<float>>  in(total_size, {-1, -1});
    std::vector<std::complex<float>> out(total_size, {-1, -1});
    std::vector<std::complex<float>> ref(total_size);

    for (int i = 0; i < total_size; i++) {
        in[i] = {(float) i, (float) -i};
    }
    compute_reference(test_case_t::FFT_iFFT, in, ref, info);

    fftwf_complex *in_data  = reinterpret_cast<fftwf_complex*>(in.data() );
    fftwf_complex *out_data = reinterpret_cast<fftwf_complex*>(out.data());

    fftwf_plan_with_nthreads(nthread_fftw);

    omp_set_nested(1);
    switch(tsce) {
        case test_scenario_t::ONE_PLAN_PER_THREAD:
            #pragma omp parallel num_threads(nthread_user)
            {
                fftwf_plan plan_forward, plan_backward;

                const int tid = omp_get_thread_num();
                // Update batches_per_exec_, which can be different from batches_per_exec since howmany is capped and can be < nthread_user * batches_per_exec
                const int batches_left = (info.howmany - tid * batches_per_exec);
                const int batches_per_exec_ = batches_left > batches_per_exec ? batches_per_exec : batches_left;
                if(batches_left > 0) {
                    const int iostride = 1 + tid; // ignore the strides in info and set to a different value
                    const int iodist   = info.fft_size * iostride;

                    std::vector<std::complex<float>> inout(batches_per_exec * iodist);
                    fftwf_complex *inout_data = reinterpret_cast<fftwf_complex*>(inout.data());

                    plan_forward  = fftwf_plan_many_dft(info.n.size(), info.n.data(), batches_per_exec_,  &in_data[tid * batches_per_exec * info.fft_size], info.inembed.data(), info.istride, info.idist,
                                                                                                                                                inout_data, info.onembed.data(), iostride    , iodist    ,  FFTW_FORWARD, FFTW_ESTIMATE);
                    plan_backward = fftwf_plan_many_dft(info.n.size(), info.n.data(), batches_per_exec_,                                        inout_data, info.onembed.data(), iostride    , iodist    ,
                                                                                                         &out_data[tid * batches_per_exec * info.fft_size], info.onembed.data(), info.ostride, info.odist, FFTW_BACKWARD, FFTW_ESTIMATE);
                    if((plan_forward != nullptr) && (plan_backward != nullptr)) {
                        fftwf_execute(plan_forward);
                        fftwf_execute(plan_backward);

                        fftwf_destroy_plan(plan_forward);
                        fftwf_destroy_plan(plan_backward);
                    }
                }
            }
            break;
        case test_scenario_t::ONE_PLAN_PER_PROCESS:
            fftwf_plan plan_forward, plan_backward;
            plan_forward  = fftwf_plan_many_dft(info.n.size(), info.n.data(), batches_per_exec, in_data, info.inembed.data(), info.istride, info.idist, out_data, info.onembed.data(), info.ostride, info.odist,  FFTW_FORWARD, FFTW_ESTIMATE); // in-place
            plan_backward = fftwf_plan_many_dft(info.n.size(), info.n.data(), batches_per_exec,out_data, info.onembed.data(), info.ostride, info.odist, out_data, info.onembed.data(), info.ostride, info.odist, FFTW_BACKWARD, FFTW_ESTIMATE); // in-place
            if((plan_forward == nullptr) || (plan_backward == nullptr)) {
                std::cout << "fftwf_plan_many_dft (forward or backward) failed" << std::endl;
                return 1;
            }

            #pragma omp parallel num_threads(nthread_user)
            {
                const int tid = omp_get_thread_num();
                fftwf_execute_dft(plan_forward ,  &in_data[tid * batches_per_exec * info.idist],  &out_data[tid * batches_per_exec * info.odist]); // FFT
                fftwf_execute_dft(plan_backward, &out_data[tid * batches_per_exec * info.odist],  &out_data[tid * batches_per_exec * info.odist]); // iFFT
            }
            fftwf_destroy_plan(plan_forward);
            fftwf_destroy_plan(plan_backward);
            break;
        default:
            std::cout << "Invalid test scenario" << std::endl;
            return 1;
    }

    const double rel_l2_err = compute_error(ref, out, info);

    /* Free resources */
    fftwf_cleanup();

    return assess_error<float>(test_cmd.c_str(), rel_l2_err);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout<<"Usage: ./c2c_single_withomp_example NX [NY [NZ]]\n"
                 <<"Arguments:\n"
                 <<"\tNX: The transform size in x (outermost) dimension.\n"
                 <<"\tNY: The transform size in y             dimension.\n"
                 <<"\tNZ: The transform size in z (innermost) dimension."<<std::endl;
        return EXIT_FAILURE;
    }
    std::string test_cmd_str = get_cmd_string(argc, argv);

    std::vector<int> n = {std::stoi(argv[1])};
    if(argc > 2) { n.push_back(std::stoi(argv[2])); }
    if(argc > 3) { n.push_back(std::stoi(argv[3])); }

    const int howmany = set_howmany<float>(nthread_user * batches_per_exec, get_product<int>(n.begin(), n.end()));
    if (howmany <= 0) {
        std::cout << "Size of transform too large. Waive." << std::endl;
        return EXIT_SUCCESS;
    }

    test_info_t info(howmany, n);

    std::cout<< "Start ONE_PLAN_PER_THREAD scenario:" << std::endl;
    int fail = run_test(test_cmd_str, test_scenario_t::ONE_PLAN_PER_THREAD, info);

    std::cout<< "Start ONE_PLAN_PER_PROCESS scenario" << std::endl;
    fail = fail || run_test(test_cmd_str, test_scenario_t::ONE_PLAN_PER_PROCESS  , info);

    return fail;
}
