#include <iostream>

// NVPL_RAND
#include <nvpl_rand.h>

#include "common.hpp"

int main(int , char** ) {
    int version = 0;
    NVPL_RAND_CHECK(nvplRandGetVersion(&version));

    if (version == NVPL_RAND_VERSION) {
        std::cout << "Success\n";
        return 0;
    }
    std::cout << "Failure\n";
    return 1;
}
