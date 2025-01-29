#include <vector>
#include <array>
#include <thread>
#include <nvpl_fftw.h>
#include "common.h"

// This example computes a forward and a backward 1D/2D/3D C2C FFT in double precision with strided data. Multiple batches at a time.

constexpr int howmany_default = 102;
constexpr std::array<int, 4> arbitrary_step = {3, 2, 1, 2};

int run_test(const int nthread, std::string& test_cmd, test_case_t tcase, const test_info_t& info) {
    if(fftw_init_threads() == 0) {
        std::cout << "fftw_init_threads() failed" << std::endl;
        return 1;
    }

    int total_size = info.howmany * info.fft_size;
    std::vector<std::complex<double>>  in(info.howmany * info.idist, {-1, -1});
    std::vector<std::complex<double>> out(info.howmany * info.odist, {-1, -1});
    std::vector<std::complex<double>> ref(total_size);

    fftw_plan plan_forward, plan_backward;

    for (int i = 0; i < total_size; i++) {
        int idx = (i / info.fft_size) * info.idist + (i % info.fft_size) * info.istride;
        in[idx] = {(double) i, (double) -i};
    }
    compute_reference(tcase, in, ref, info);

    fftw_complex *in_data  = reinterpret_cast<fftw_complex*>(in.data());
    fftw_complex *out_data = reinterpret_cast<fftw_complex*>(out.data());
    switch(tcase) {
        case test_case_t::FFT_iFFT:
            fftw_plan_with_nthreads(nthread);
            plan_forward  = fftw_plan_many_dft(info.n.size(), info.n.data(), info.howmany, in_data , info.inembed.data(), info.istride, info.idist,
                                                                                           out_data, info.onembed.data(), info.ostride, info.odist, FFTW_FORWARD , FFTW_ESTIMATE); // out-of-place
            plan_backward = fftw_plan_many_dft(info.n.size(), info.n.data(), info.howmany, out_data, info.onembed.data(), info.ostride, info.odist,
                                                                                           out_data, info.onembed.data(), info.ostride, info.odist, FFTW_BACKWARD, FFTW_ESTIMATE); // in-place
            if((plan_forward == nullptr) || (plan_backward == nullptr)) {
                std::cout << "fftw_plan_many_dft (forward or backward) failed" << std::endl;
                return 1;
            }
            fftw_execute_dft(plan_forward , in_data , out_data);
            fftw_execute_dft(plan_backward, out_data, out_data);
            break;
        default:
            std::cout << "Invalid test case" << std::endl;
            return 1;
    }

    const double rel_l2_err = compute_error(ref, out, info);

    /* Free resources */
    fftw_destroy_plan(plan_forward);
    fftw_destroy_plan(plan_backward);
    fftw_cleanup();

    return assess_error<double>(test_cmd.c_str(), rel_l2_err);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout<<"Usage: ./c2c_double_many_example NTHREAD NX [NY [NZ]]\n"
                 <<"Arguments:\n"
                 <<"\tNTHREAD: Number of thread.\n"
                 <<"\tNX     : The transform size in x (outermost) dimension.\n"
                 <<"\tNY     : The transform size in y             dimension.\n"
                 <<"\tNZ     : The transform size in z (innermost) dimension."<<std::endl;
        return EXIT_FAILURE;
    }
    std::string test_cmd = get_cmd_string(argc, argv);

    const int nthread = std::stoi(argv[1]);
    std::vector<int> n = {std::stoi(argv[2])};
    if(argc > 3) { n.push_back(std::stoi(argv[3])); }
    if(argc > 4) { n.push_back(std::stoi(argv[4])); }

    const int istride = arbitrary_step[0], ostride = arbitrary_step[1];
    const int idist = get_product<int>(n.begin(), n.end()) * istride * arbitrary_step[2];
    const int odist = get_product<int>(n.begin(), n.end()) * ostride * arbitrary_step[3];
    const int howmany = set_howmany<double>(howmany_default, std::max(idist, odist));
    if (howmany <= 0) {
        std::cout << "Size of transform too large. Waive." << std::endl;
        return EXIT_SUCCESS;
    }

    test_info_t info(howmany, n, istride, ostride, idist, odist);

    return run_test(nthread, test_cmd, test_case_t::FFT_iFFT, info);
}
