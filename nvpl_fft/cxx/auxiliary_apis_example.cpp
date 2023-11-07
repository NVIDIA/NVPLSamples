#include <vector>
#include <nvpl_fftw.h>
#include "common.h"

constexpr unsigned howmany = 10;
constexpr unsigned fft_size = 4;
constexpr double   seconds = 10;
constexpr int      fftw_wisdom_success = 1; // https://www.fftw.org/fftw3_doc/Wisdom-Import.html
                                            // The return value of import wisdom APIs is 1 if the wisdom was read successfully and 0 otherwise.

int run_api_test() {

    std::vector<std::complex<double>> inout(howmany*fft_size);
    fftwf_complex *inpout_data = reinterpret_cast<fftwf_complex*>(inout.data() );

    // get version
    std::cout << "NVPL FFT version " << nvpl_fft_get_version() << std::endl;

    // wisdom from system
    int status = fftw_import_system_wisdom();
    if (status != fftw_wisdom_success) {
        std::cout << "fftw_import_system_wisdom: FAILED" << std::endl;
        return 1;
    }

    fftw_plan plan = fftwf_plan_dft_1d(fft_size, inpout_data, inpout_data, FFTW_FORWARD, FFTW_ESTIMATE);

    // wisdom to/from file
    FILE *out_fptr = fopen("wisdom_file.txt", "w");
    if(out_fptr == nullptr) {
        std::cout << "file open FAILED " << std::endl;
        return 1;
    }
    fftw_export_wisdom_to_file(out_fptr);
    fclose(out_fptr);
    std::cout << "fftw_export_wisdom_to_file: PASSED" << std::endl;

    FILE *in_fptr = fopen("wisdom_file.txt", "r");
    if(in_fptr == nullptr) {
        std::cout << "file open FAILED " << std::endl;
        return 1;
    }
    status = fftw_import_wisdom_from_file(in_fptr);
    if (status != fftw_wisdom_success) {
        std::cout << "fftw_import_wisdom_from_file: FAILED" << std::endl;
        return 1;
    }
    fclose(in_fptr);
    std::cout << "fftw_import_wisdom_from_file: PASSED" << std::endl;

    // wisdom to/from string
    char* wisdom_str = fftw_export_wisdom_to_string();
    if(wisdom_str == nullptr) {
        std::cout << "fftw_export_wisdom_to_string: FAILED" << std::endl;
        return 1;
    }
    std::cout << "fftw_export_wisdom_to_string: " << wisdom_str << " PASSED" << std::endl;

    status = fftw_import_wisdom_from_string(wisdom_str);
    if (status != fftw_wisdom_success) {
        std::cout << "fftw_import_wisdom_from_string: FAILED" << std::endl;
        return 1;
    }
    std::cout << "fftw_import_wisdom_from_string: PASSED" << std::endl;
    free(wisdom_str);

    // print plan
    fftw_print_plan(plan);
    std::cout << "fftw_print_plan: PASSED" << std::endl;

    // set timelimit
    fftw_set_timelimit(seconds);
    std::cout << "fftw_set_timelimit: PASSED" << std::endl;

    // cost
    double cost = fftw_cost(plan);
    std::cout << "fftw_cost: cost " << cost << " PASSED" << std::endl;

    // flops
    double add = 0.0, mul = 0.0, fma = 0.0;
    fftw_flops(plan, &add, &mul, &fma);
    std::cout << "fftw_flops: add " << add << ", mul " << mul  << ", fma " << fma << " PASSED" << std::endl;

    fftw_destroy_plan(plan);

    return 0;
}

int main(int argc, [[maybe_unused]] char *argv[]) {
    if (argc < 1) {
        std::cout<<"Usage: ./auxiliary_apis_example\n"<<std::endl;
        return EXIT_FAILURE;
    }
    return run_api_test();
}
