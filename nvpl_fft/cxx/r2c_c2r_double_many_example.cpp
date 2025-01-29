#include <vector>
#include <array>
#include <thread>
#include <nvpl_fftw.h>
#include "common.h"

// This example computes a forward R2C and a backward C2R 1D/2D/3D FFT in double precision with strided data. Multiple batches at a time.

constexpr int howmany_default = 120;
constexpr std::array<int, 3> arbitrary_step = {2, 3, 5};

int run_test(const int nthread, const std::string& test_cmd, test_case_t tcase, test_info_t& r2c, const test_info_t& c2r) {
    const int total_size = r2c.howmany * r2c.fft_size;

    r2c.idist = std::max(r2c.idist, 2*r2c.odist); // For an in-place real-to-complex transform, the input size needs to be padded
    r2c.inembed.back() = 2 * r2c.onembed.back();

    std::vector<double> real_inout(r2c.howmany * r2c.idist, -1);
    std::vector<double> real_out(c2r.howmany * c2r.odist, -1);
    std::vector<double> real_ref(total_size);

    fftw_plan plan_forward, plan_backward;

    fill_real_input_data(tcase, real_inout, r2c);
    compute_reference(tcase, real_inout, real_ref, r2c);

    fftw_complex *complex_inout_data = reinterpret_cast<fftw_complex*>(real_inout.data());
    switch(tcase) {
        case test_case_t::FFT_iFFT:
            fftw_plan_with_nthreads(nthread);
            plan_forward  = fftw_plan_many_dft_r2c(r2c.n.size(), r2c.n.data(), r2c.howmany, real_inout.data(),  r2c.inembed.data(), r2c.istride, r2c.idist,
                                                                                            complex_inout_data, r2c.onembed.data(), r2c.ostride, r2c.odist, FFTW_MEASURE); // in-place
            plan_backward = fftw_plan_many_dft_c2r(c2r.n.size(), c2r.n.data(), c2r.howmany, complex_inout_data, c2r.inembed.data(), c2r.istride, c2r.idist,
                                                                                            real_out.data(),    c2r.onembed.data(), c2r.ostride, c2r.odist, FFTW_MEASURE); // out-of-place
            if((plan_forward == nullptr) || (plan_backward == nullptr)) {
                std::cout << "fftw_plan_many_dft (forward or backward) failed" << std::endl;
                return 1;
            }
            fftw_execute(plan_forward);
            fftw_execute(plan_backward);
            break;
        default:
            std::cout << "Invalid test case" << std::endl;
            return 1;
    }

    const double rel_l2_err = compute_error(real_ref, real_out, c2r);

    /* Free resources */
    fftw_destroy_plan(plan_forward);
    fftw_destroy_plan(plan_backward);
    fftw_cleanup();

    return assess_error<double>(test_cmd, rel_l2_err);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout<<"Usage: ./r2c_c2r_double_many_example NTHREAD NX [NY [NZ]]\n"
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

    auto [r2cinfo, c2rinfo] = get_r2c_c2r_setups<double>(n, howmany_default, arbitrary_step[0], arbitrary_step[1], arbitrary_step[2]);

    if (r2cinfo.howmany <= 0) {
        std::cout << "Size of transform too large. Waive." << std::endl;
        return EXIT_SUCCESS;
    }

    return run_test(nthread, test_cmd, test_case_t::FFT_iFFT, r2cinfo, c2rinfo);
}
