#include <vector>
#include <array>
#include <thread>
#include <nvpl_fftw.h>
#include <cassert>
#include "common.h"

void initialize_data(std::vector<std::complex<float>>& c2c_in, std::vector<std::complex<float>>& c2c_inout,
                     std::vector<std::complex<float>>& c2r_in, std::vector<float>& c2r_inout,
                     std::vector<float>& r2c_in, std::vector<float>& r2c_inout) {
    // C2C
    size_t i = 0;
    for (auto& v: c2c_in) {
        v = {(float)i, -(float)i};
        i++;
    }
    i = 0;
    for (auto& v: c2c_inout) {
        v = {(float)i, -(float)i};
        i++;
    }

    // C2R
    i = 0;
    for (auto& v: c2r_in) {
        v = {(float)i, -(float)i};
        i++;
    }
    i = 0;
    for (auto& v: c2r_inout) {
        v = i;
        i++;
    }

    // R2C
    i = 0;
    for (auto& v: r2c_in) {
        v = i;
        i++;
    }
    i = 0;
    for (auto& v: r2c_inout) {
        v = i;
        i++;
    }
}

void print_results(const std::string& api_cat,
                   const std::vector<std::complex<float>>& c2c_out, const std::vector<std::complex<float>>& c2c_inout,
                   const std::vector<float>& c2r_out              , const std::vector<float>& c2r_inout,
                   const std::vector<std::complex<float>>& r2c_out, const std::vector<float>& r2c_inout) {
    std::cout << api_cat << " C2C OOP: ";
    for (const auto& v: c2c_out) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << api_cat << " C2C IP : ";
    for (const auto& v: c2c_inout) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << api_cat << " C2R OOP: ";
    for (const auto& v: c2r_out) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << api_cat << " C2R IP : ";
    for (const auto& v: c2r_inout) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << api_cat << " R2C OOP: ";
    for (const auto& v: r2c_out) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << api_cat << " R2C IP : ";
    for (const auto& v: r2c_inout) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

void run_simple_api_test(const std::vector<int>& n) {

    const int howmany = 1;
    const int c2c_complex_elems     = get_product<int>(n.begin(),   n.end());
    const int c2r_r2c_real_elems    = get_product<int>(n.begin(),   n.end());
    const int c2r_r2c_complex_elems = get_product<int>(n.begin(), --n.end()) * (n.back() / 2 + 1);

    const int c2r_idist     =     c2r_r2c_complex_elems;
    const int c2r_ip_odist  = 2 * c2r_r2c_complex_elems;
    const int c2r_oop_odist =        c2r_r2c_real_elems;
    const int r2c_odist     =     c2r_r2c_complex_elems;
    const int r2c_ip_idist  = 2 * c2r_r2c_complex_elems;
    const int r2c_oop_idist =        c2r_r2c_real_elems;
    const int c2c_idist     =         c2c_complex_elems;
    const int c2c_odist     =         c2c_complex_elems;

    std::vector<std::complex<float>>  c2c_in(howmany * c2c_idist    , {0, 0});
    std::vector<std::complex<float>> c2c_out(howmany * c2c_odist    , {0, 0});
    std::vector<float>                r2c_in(howmany * r2c_oop_idist,      0);
    std::vector<std::complex<float>> r2c_out(howmany * r2c_odist    , {0, 0});
    std::vector<std::complex<float>>  c2r_in(howmany * c2r_idist    , {0, 0});
    std::vector<float>               c2r_out(howmany * c2r_oop_odist,      0);

    std::vector<std::complex<float>> c2c_inout(std::max(howmany * c2c_idist   , howmany * c2c_odist)   , {0, 0});
    std::vector<float>               r2c_inout(std::max(howmany * r2c_ip_idist, howmany * r2c_odist)   , 0);
    std::vector<float>               c2r_inout(std::max(howmany * c2r_idist   , howmany * c2r_ip_odist), 0);

    fftwf_plan plan_c2c_oop, plan_c2c_inplace, plan_c2r_oop, plan_c2r_inplace, plan_r2c_oop, plan_r2c_inplace;

    plan_c2c_oop     = fftwf_plan_dft(    n.size(), n.data(), reinterpret_cast<fftwf_complex*>(   c2c_in.data()),   reinterpret_cast<fftwf_complex*>(c2c_out.data()), FFTW_FORWARD, FFTW_ESTIMATE); // out-of-place
    plan_c2c_inplace = fftwf_plan_dft(    n.size(), n.data(), reinterpret_cast<fftwf_complex*>(c2c_inout.data()), reinterpret_cast<fftwf_complex*>(c2c_inout.data()), FFTW_FORWARD, FFTW_ESTIMATE); // in-place
    plan_c2r_oop     = fftwf_plan_dft_c2r(n.size(), n.data(), reinterpret_cast<fftwf_complex*>(   c2r_in.data()),                                     c2r_out.data(), FFTW_MEASURE); // out-of-place
    plan_c2r_inplace = fftwf_plan_dft_c2r(n.size(), n.data(), reinterpret_cast<fftwf_complex*>(c2r_inout.data()),                                   c2r_inout.data(), FFTW_MEASURE); // in-place
    plan_r2c_oop     = fftwf_plan_dft_r2c(n.size(), n.data(),                                      r2c_in.data(),   reinterpret_cast<fftwf_complex*>(r2c_out.data()), FFTW_MEASURE); // out-of-place
    plan_r2c_inplace = fftwf_plan_dft_r2c(n.size(), n.data(),                                   r2c_inout.data(), reinterpret_cast<fftwf_complex*>(r2c_inout.data()), FFTW_MEASURE); // in-place

    initialize_data(c2c_in, c2c_inout, c2r_in, c2r_inout, r2c_in, r2c_inout);

    fftwf_execute(plan_c2c_oop);
    fftwf_execute(plan_c2c_inplace);
    fftwf_execute(plan_c2r_oop);
    fftwf_execute(plan_c2r_inplace);
    fftwf_execute(plan_r2c_oop);
    fftwf_execute(plan_r2c_inplace);

    print_results("SIMPLE", c2c_out, c2c_inout, c2r_out, c2r_inout, r2c_out, r2c_inout);

    /* Free resources */
    fftwf_destroy_plan(plan_c2c_inplace);
    fftwf_destroy_plan(plan_c2c_oop);
    fftwf_destroy_plan(plan_c2r_inplace);
    fftwf_destroy_plan(plan_c2r_oop);
    fftwf_destroy_plan(plan_r2c_inplace);
    fftwf_destroy_plan(plan_r2c_oop);

    fftwf_cleanup();
}

void run_many_api_test(const std::vector<int>& n) {

    const int howmany = 1;
    const int istride = 2, ostride = istride;
    const int c2c_complex_elems     = get_product<int>(n.begin(),   n.end());
    const int c2r_r2c_real_elems    = get_product<int>(n.begin(),   n.end());
    const int c2r_r2c_complex_elems = get_product<int>(n.begin(), --n.end()) * (n.back() / 2 + 1);

    const int c2r_idist     =     c2r_r2c_complex_elems * istride;
    const int c2r_ip_odist  = 2 * c2r_r2c_complex_elems * ostride;
    const int c2r_oop_odist =        c2r_r2c_real_elems * ostride;
    const int r2c_odist     =     c2r_r2c_complex_elems * ostride;
    const int r2c_ip_idist  = 2 * c2r_r2c_complex_elems * istride;
    const int r2c_oop_idist =        c2r_r2c_real_elems * istride;
    const int c2c_idist     =         c2c_complex_elems * istride;
    const int c2c_odist     =         c2c_complex_elems * ostride;

    std::vector<std::complex<float>>  c2c_in(howmany * c2c_idist    , {0, 0});
    std::vector<std::complex<float>> c2c_out(howmany * c2c_odist    , {0, 0});
    std::vector<float>                r2c_in(howmany * r2c_oop_idist,      0);
    std::vector<std::complex<float>> r2c_out(howmany * r2c_odist    , {0, 0});
    std::vector<std::complex<float>>  c2r_in(howmany * c2r_idist    , {0, 0});
    std::vector<float>               c2r_out(howmany * c2r_oop_odist,      0);

    std::vector<std::complex<float>> c2c_inout(std::max(howmany * c2c_idist   , howmany * c2c_odist)   , {0, 0});
    std::vector<float>               r2c_inout(std::max(howmany * r2c_ip_idist, howmany * r2c_odist)   , 0);
    std::vector<float>               c2r_inout(std::max(howmany * c2r_idist   , howmany * c2r_ip_odist), 0);

    fftwf_plan plan_c2c_oop, plan_c2c_inplace, plan_c2r_oop, plan_c2r_inplace, plan_r2c_oop, plan_r2c_inplace;

    plan_c2c_oop     = fftwf_plan_many_dft(    n.size(), n.data(), howmany, reinterpret_cast<fftwf_complex*>(   c2c_in.data()), nullptr, istride,     c2c_idist,   reinterpret_cast<fftwf_complex*>(c2c_out.data()), nullptr, ostride,     c2c_odist, FFTW_FORWARD, FFTW_ESTIMATE); // out-of-place
    plan_c2c_inplace = fftwf_plan_many_dft(    n.size(), n.data(), howmany, reinterpret_cast<fftwf_complex*>(c2c_inout.data()), nullptr, istride,     c2c_idist, reinterpret_cast<fftwf_complex*>(c2c_inout.data()), nullptr, ostride,     c2c_odist, FFTW_FORWARD, FFTW_ESTIMATE); // in-place
    plan_c2r_oop     = fftwf_plan_many_dft_c2r(n.size(), n.data(), howmany, reinterpret_cast<fftwf_complex*>(   c2r_in.data()), nullptr, istride,     c2r_idist,                                     c2r_out.data(), nullptr, ostride, c2r_oop_odist, FFTW_MEASURE); // out-of-place
    plan_c2r_inplace = fftwf_plan_many_dft_c2r(n.size(), n.data(), howmany, reinterpret_cast<fftwf_complex*>(c2r_inout.data()), nullptr, istride,     c2r_idist,                                   c2r_inout.data(), nullptr, ostride,  c2r_ip_odist, FFTW_MEASURE); // in-place
    plan_r2c_oop     = fftwf_plan_many_dft_r2c(n.size(), n.data(), howmany,                                      r2c_in.data(), nullptr, ostride, r2c_oop_idist,   reinterpret_cast<fftwf_complex*>(r2c_out.data()), nullptr, istride,     r2c_odist, FFTW_MEASURE); // out-of-place
    plan_r2c_inplace = fftwf_plan_many_dft_r2c(n.size(), n.data(), howmany,                                   r2c_inout.data(), nullptr, ostride,  r2c_ip_idist, reinterpret_cast<fftwf_complex*>(r2c_inout.data()), nullptr, istride,     r2c_odist, FFTW_MEASURE); // in-place

    initialize_data(c2c_in, c2c_inout, c2r_in, c2r_inout, r2c_in, r2c_inout);

    fftwf_execute(plan_c2c_oop);
    fftwf_execute(plan_c2c_inplace);
    fftwf_execute(plan_c2r_oop);
    fftwf_execute(plan_c2r_inplace);
    fftwf_execute(plan_r2c_oop);
    fftwf_execute(plan_r2c_inplace);

    print_results("MANY", c2c_out, c2c_inout, c2r_out, c2r_inout, r2c_out, r2c_inout);

    /* Free resources */
    fftwf_destroy_plan(plan_c2c_inplace);
    fftwf_destroy_plan(plan_c2c_oop);
    fftwf_destroy_plan(plan_c2r_inplace);
    fftwf_destroy_plan(plan_c2r_oop);
    fftwf_destroy_plan(plan_r2c_inplace);
    fftwf_destroy_plan(plan_r2c_oop);

    fftwf_cleanup();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout<<"Usage: ./c2c_c2r_r2c_single_apis_example NX [NY [NZ]]\n"
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

    run_simple_api_test(n);
    run_many_api_test(n);

    return 0;
}
