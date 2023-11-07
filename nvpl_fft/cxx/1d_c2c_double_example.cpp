#include <vector>
#include <array>
#include <nvpl_fftw.h>
#include "common.h"

/* Modified from CUDALibrarySamples/cuFFT/1d_c2c/1d_c2c_example.cpp. */

// This example computes a forward and a backward 1D C2C FFT in double precision with contiguous data. One batch at a time.

int run_test(const std::string &test_cmd, test_case_t tcase, const test_info_t& info) {
    int total_size = info.howmany * info.n[0];

    std::vector<std::complex<double>>  in(total_size, {-1, -1});
    std::vector<std::complex<double>> out(total_size, {-1, -1});
    std::vector<std::complex<double>> ref(total_size);

    fftw_plan plan_forward, plan_backward;

    for (int i = 0; i < total_size; i++) {
        in[i] = {(double) i, (double) -i};
    }
    compute_reference(tcase, in, ref, info);

    fftw_complex *in_data  = reinterpret_cast<fftw_complex*>(in.data() );
    fftw_complex *out_data = reinterpret_cast<fftw_complex*>(out.data());
    switch(tcase) {
        case test_case_t::FFT_iFFT:
            plan_forward  = fftw_plan_dft_1d(info.fft_size, in_data , out_data, FFTW_FORWARD , FFTW_ESTIMATE); // out-of-place
            plan_backward = fftw_plan_dft_1d(info.fft_size, out_data, out_data, FFTW_BACKWARD, FFTW_ESTIMATE); // in-place

            for(int b = 0; b < info.howmany; b++) {
                fftw_execute_dft(plan_forward , in_data +b*info.fft_size, out_data+b*info.fft_size); // FFT
                fftw_execute_dft(plan_backward, out_data+b*info.fft_size, out_data+b*info.fft_size); // iFFT
            }
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
    if (argc < 2) {
        std::cout<<"Usage: ./1d_c2c_double_example NX\n"
                 <<"Arguments:\n"
                 <<"\tNX: The transform size in x dimension."<<std::endl;
        return EXIT_FAILURE;
    }
    std::string test_cmd_str = get_cmd_string(argc, argv);

    std::vector<int> n = {std::stoi(argv[1])};
    const int howmany = 2;

    test_info_t info(howmany, n);

    return run_test(test_cmd_str, test_case_t::FFT_iFFT, info);
}
