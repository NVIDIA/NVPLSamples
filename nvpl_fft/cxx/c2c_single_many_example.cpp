#include <vector>
#include <array>
#include <thread>
#include <nvpl_fftw.h>
#include "common.h"

// This example computes a forward and a backward 1D/2D/3D C2C FFT in single precision with strided data. Multiple batches at a time.

constexpr int howmany_default = 102;
constexpr std::array<int, 4> arbitrary_step = {2, 4, 3, 2};

const int nthread_forward  = std::thread::hardware_concurrency();
const int nthread_backward = std::thread::hardware_concurrency();

int run_test(std::string& test_cmd, test_case_t tcase, const test_info_t& info) {
    if(fftw_init_threads() == 0) {
        std::cout << "fftw_init_threads() failed" << std::endl;
        return 1;
    }

    const int total_size = info.howmany * info.fft_size;
    std::vector<std::complex<float>>  in(info.howmany * info.idist, {-1, -1});
    std::vector<std::complex<float>> out(info.howmany * info.odist, {-1, -1});
    std::vector<std::complex<float>> ref(total_size);

    fftwf_plan plan_forward, plan_backward;

    for (int i = 0; i < total_size; i++) {
        int idx = (i / info.fft_size) * info.idist + (i % info.fft_size) * info.istride;
        in[idx] = {(float) i, (float) -i};
    }
    compute_reference(tcase, in, ref, info);

    fftwf_complex *in_data  = reinterpret_cast<fftwf_complex*>(in.data());
    fftwf_complex *out_data = reinterpret_cast<fftwf_complex*>(out.data());
    switch(tcase) {
        case test_case_t::FFT_iFFT:
            fftwf_plan_with_nthreads(nthread_forward);
            plan_forward  = fftwf_plan_many_dft(info.n.size(), info.n.data(), info.howmany, in_data , info.inembed.data(), info.istride, info.idist,
                                                                                            out_data, info.onembed.data(), info.ostride, info.odist, FFTW_FORWARD , FFTW_ESTIMATE); // out-of-place
            fftwf_plan_with_nthreads(nthread_backward);
            plan_backward = fftwf_plan_many_dft(info.n.size(), info.n.data(), info.howmany, out_data, info.onembed.data(), info.ostride, info.odist,
                                                                                            out_data, info.onembed.data(), info.ostride, info.odist, FFTW_BACKWARD, FFTW_ESTIMATE); // in-place
            fftwf_execute_dft(plan_forward , in_data , out_data);
            fftwf_execute_dft(plan_backward, out_data, out_data);
            break;
        default:
            std::cout << "Invalid test case" << std::endl;
            return 1;
    }

    const double rel_l2_err = compute_error(ref, out, info);

    /* Free resources */
    fftwf_destroy_plan(plan_forward);
    fftwf_destroy_plan(plan_backward);
    fftwf_cleanup();

    return assess_error<float>(test_cmd.c_str(), rel_l2_err);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout<<"Usage: ./c2c_single_many_example NX [NY [NZ]]\n"
                 <<"Arguments:\n"
                 <<"\tNX: The transform size in x (outermost) dimension.\n"
                 <<"\tNY: The transform size in y             dimension.\n"
                 <<"\tNZ: The transform size in z (innermost) dimension."<<std::endl;
        return EXIT_FAILURE;
    }
    std::string test_cmd = get_cmd_string(argc, argv);

    std::vector<int> n = {std::stoi(argv[1])};
    if(argc > 2) { n.push_back(std::stoi(argv[2])); }
    if(argc > 3) { n.push_back(std::stoi(argv[3])); }

    const int istride = arbitrary_step[0], ostride = arbitrary_step[1];
    const int idist = get_product<int>(n.begin(), n.end()) * istride * arbitrary_step[2];
    const int odist = get_product<int>(n.begin(), n.end()) * ostride * arbitrary_step[3];
    const int howmany = set_howmany<float>(howmany_default, std::max(idist, odist));
    if (howmany <= 0) {
        std::cout << "Size of transform too large. Waive." << std::endl;
        return EXIT_SUCCESS;
    }

    test_info_t info(howmany, n, istride, ostride, idist, odist);

    return run_test(test_cmd, test_case_t::FFT_iFFT, info);
}
