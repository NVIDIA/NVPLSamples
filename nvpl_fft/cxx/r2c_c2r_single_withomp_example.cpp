#include <vector>
#include <array>
#include <nvpl_fftw.h>
#include "common.h"
#include <omp.h>

constexpr int batches_per_exec = 100;
#if defined(__NVCOMPILER)
constexpr int nthread_fftw = 1; // NVHPC OpenMP does not support nested parallel regions on CPU (see https://docs.nvidia.com/hpc-sdk//compilers/hpc-compilers-user-guide/index.html#openmp-subset)
#else
constexpr int nthread_fftw = 72;
#endif
constexpr int nthread_user = 4;

// This example computes a forward R2C and a backward C2R 1D FFT in single precision with contiguous data.
// It demonstrates how user can call plan + execution functions inside a omp parallel region. The omp threads share plans but operate on different parts of input, output data.
int run_test(const std::string &test_cmd, test_scenario_t tsce, test_info_t& r2c, const test_info_t& c2r) {
    if(tsce == test_scenario_t::ONE_PLAN_PER_PROCESS && (r2c.howmany != nthread_user * batches_per_exec)) {
        std::cout << "test_scenario_t::ONE_PLAN_PER_PROCESS: howmany must equal to nthread_user x batches_per_exec -- skipped" << std::endl;
        return 0;
    }
    int total_size = r2c.howmany * r2c.fft_size;

    std::vector<float>   real_in(r2c.howmany * r2c.idist, -1);
    std::vector<float>  real_out(c2r.howmany * c2r.odist, -1);
    std::vector<float>  real_ref(r2c.howmany * r2c.fft_size);

    for (int i = 0; i < total_size; i++) {
        int idx = (i / r2c.fft_size) * r2c.idist + (i % r2c.fft_size) * r2c.istride;
        real_in[idx] = i;
    }
    compute_reference(test_case_t::FFT_iFFT, real_in, real_ref, r2c);

    float *real_in_data  = real_in.data();
    float *real_out_data = real_out.data();

    fftwf_plan_with_nthreads(nthread_fftw);

    omp_set_nested(1);
    switch(tsce) {
        case test_scenario_t::ONE_PLAN_PER_THREAD:
            #pragma omp parallel num_threads(nthread_user)
            {
                fftwf_plan plan_forward, plan_backward;

                int tid = omp_get_thread_num();
                // Update batches_per_exec_, which can be different from batches_per_exec since howmany is capped and can be < nthread_user * batches_per_exec
                int batches_left = (r2c.howmany - tid * batches_per_exec);
                int batches_per_exec_ = batches_left > batches_per_exec ? batches_per_exec : batches_left;
                if(batches_left > 0) {
                    int r2c_ostride = 1 + tid; // ignore the strides in r2c info and set to a different value
                    int r2c_odist   = get_product<int>(r2c.n.begin(), --r2c.n.end()) * (r2c.n.back() / 2 + 1) * r2c_ostride; // #complex_elems x ostride

                    std::vector<std::complex<float>> complex_inout(batches_per_exec_ * r2c_odist);
                    fftwf_complex *complex_inout_data = reinterpret_cast<fftwf_complex*>(complex_inout.data());

                    plan_forward  = fftwf_plan_many_dft_r2c(r2c.n.size(), r2c.n.data(), batches_per_exec_,  &real_in_data[tid * batches_per_exec * r2c.idist], r2c.inembed.data(), r2c.istride, r2c.idist,
                                                                                                                                           complex_inout_data, r2c.onembed.data(), r2c_ostride, r2c_odist, FFTW_MEASURE); // in-place
                    plan_backward = fftwf_plan_many_dft_c2r(c2r.n.size(), c2r.n.data(), batches_per_exec_,                                 complex_inout_data, c2r.inembed.data(), r2c_ostride, r2c_odist,
                                                                                                           &real_out_data[tid * batches_per_exec * c2r.odist], c2r.onembed.data(), c2r.ostride, c2r.odist, FFTW_MEASURE); // out-of-place
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
            {
                std::vector<std::complex<float>> complex_inout(r2c.howmany * r2c.odist, {-1,-1});
                fftwf_complex *complex_inout_data = reinterpret_cast<fftwf_complex*>(complex_inout.data());
                fftwf_plan plan_forward, plan_backward;
                plan_forward  = fftwf_plan_many_dft_r2c(r2c.n.size(), r2c.n.data(), batches_per_exec,     real_in.data(), r2c.inembed.data(), r2c.istride, r2c.idist,
                                                                                                      complex_inout_data, r2c.onembed.data(), r2c.ostride, r2c.odist, FFTW_MEASURE); // in-place
                plan_backward = fftwf_plan_many_dft_c2r(c2r.n.size(), c2r.n.data(), batches_per_exec, complex_inout_data, c2r.inembed.data(), c2r.istride, c2r.idist,
                                                                                                         real_out.data(), c2r.onembed.data(), c2r.ostride, c2r.odist, FFTW_MEASURE); // out-of-place
                if((plan_forward == nullptr) || (plan_backward == nullptr)) {
                    std::cout << "fftwf_plan_many_dft (forward or backward) failed" << std::endl;
                    return 1;
                }

                #pragma omp parallel num_threads(nthread_user)
                {
                    unsigned tid = omp_get_thread_num();
                    fftwf_execute_dft_r2c(plan_forward ,       &real_in_data[tid * batches_per_exec * r2c.idist],  &complex_inout_data[tid * batches_per_exec * r2c.odist]); // FFT
                    fftwf_execute_dft_c2r(plan_backward, &complex_inout_data[tid * batches_per_exec * c2r.idist],       &real_out_data[tid * batches_per_exec * c2r.odist]); // iFFT
                }
                fftwf_destroy_plan(plan_forward);
                fftwf_destroy_plan(plan_backward);
            }
            break;
        default:
            std::cout << "Invalid test scenario" << std::endl;
            return 1;
    }

    double rel_l2_err = compute_error(real_ref, real_out, c2r);

    /* Free resources */
    fftwf_cleanup();

    return assess_error<float>(test_cmd.c_str(), rel_l2_err);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout<<"Usage: ./r2c_c2r_single_withomp_example NX [NY [NZ]]\n"
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

    // For R2C/C2R transforms, the real    (R2C input , C2R output) array has n[0] x .. x n[rank-1] elems and
    //                         the complex (R2C output, C2R input ) array has n[1] x .. x (n[rank -1] / 2 + 1) elems
    // See https://docs.nvidia.com/cuda/cufft/index.html#data-layout for more details.
    const int real_elems    = get_product<int>(n.begin(),   n.end());
    const int complex_elems = get_product<int>(n.begin(), --n.end()) * (n.back() / 2 + 1);

    const int r2c_istride = 1, r2c_ostride = 1;
    const int r2c_idist   = real_elems    * r2c_istride;
    const int r2c_odist   = complex_elems * r2c_ostride;

    const int c2r_istride = r2c_ostride, c2r_idist = r2c_odist;
    const int c2r_ostride = 1;
    const int c2r_odist   = real_elems * c2r_ostride;

    const int howmany = set_howmany<float>(nthread_user * batches_per_exec, std::max(std::max(r2c_idist, r2c_odist), std::max(c2r_idist, c2r_odist)));
    if (howmany <= 0) {
        std::cout << "Size of transform too large. Waive." << std::endl;
        return EXIT_SUCCESS;
    }

    test_info_t r2cinfo(howmany, n, r2c_istride, r2c_ostride, r2c_idist, r2c_odist);
    test_info_t c2rinfo(howmany, n, c2r_istride, c2r_ostride, c2r_idist, c2r_odist);
    r2cinfo.onembed.back() = (r2cinfo.onembed.back() / 2 + 1);
    c2rinfo.inembed.back() = (c2rinfo.inembed.back() / 2 + 1);

    std::cout<< "Start ONE_PLAN_PER_THREAD scenario:" << std::endl;
    int fail = run_test(test_cmd_str, test_scenario_t::ONE_PLAN_PER_THREAD, r2cinfo, c2rinfo);

    std::cout<< "Start ONE_PLAN_PER_PROCESS scenario" << std::endl;
    fail = fail || run_test(test_cmd_str, test_scenario_t::ONE_PLAN_PER_PROCESS  , r2cinfo, c2rinfo);

    return fail;

}
