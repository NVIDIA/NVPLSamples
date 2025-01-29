#include <vector>
#include <array>
#include <iostream>
#include <fftw3.h>

// This example is meant to ensures that nvpl_fftw is added as an include path and that the include statement "#include <fftw3.h>" works

int main() {
    // call nvpl specific function from within fftw3.h to ensure nvpl is being picked up and not fftw 
    int version = nvpl_fft_get_version();

    std::cout << "nvpl version: " << version << std::endl;

    return 0;
}
