#include <vector>
#include <nvpl_fftw.h>
#include "common.h"

/* Modified from CUDALibrarySamples/cuFFT/1d_c2c/1d_c2c_example.cpp. */

// This example computes a forward and a backward 1D C2C FFT in single precision with contiguous data. One batch at a time.

int run_test(const std::string &test_cmd, test_case_t tcase, const test_info_t& info) {
    int total_size = info.howmany * info.n[0];

    std::vector<std::complex<float>>  in(total_size, {-1, -1});
    std::vector<std::complex<float>> out(total_size, {-1, -1});
    std::vector<std::complex<float>> ref(total_size);

    fftwf_plan plan_forward, plan_backward;

    for (int i = 0; i < total_size; i++) {
        in[i] = {(float) i, (float) -i};
    }
    compute_reference(tcase, in, ref, info);

    fftwf_complex *in_data  = reinterpret_cast<fftwf_complex*>(in.data() );
    fftwf_complex *out_data = reinterpret_cast<fftwf_complex*>(out.data());
    switch(tcase) {
        case test_case_t::FFT_iFFT:
            plan_forward  = fftwf_plan_dft_1d(info.fft_size, in_data , out_data, FFTW_FORWARD , FFTW_ESTIMATE); // out-of-place
            plan_backward = fftwf_plan_dft_1d(info.fft_size, out_data, out_data, FFTW_BACKWARD, FFTW_ESTIMATE); // in-place
            if((plan_forward == nullptr) || (plan_backward == nullptr)) {
                std::cout << "fftwf_plan_many_dft (forward or backward) failed" << std::endl;
                return 1;
            }

            for(int b = 0; b < info.howmany; b++) {
                fftwf_execute_dft(plan_forward , in_data +b*info.fft_size, out_data+b*info.fft_size); // FFT
                fftwf_execute_dft(plan_backward, out_data+b*info.fft_size, out_data+b*info.fft_size); // iFFT
            }
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
        std::cout<<"Usage: ./1d_c2c_single_example NX\n"
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
