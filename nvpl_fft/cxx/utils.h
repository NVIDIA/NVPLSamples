#ifndef NVPLFFT_EXAMPLE_UNTILS_H_
#define NVPLFFT_EXAMPLE_UNTILS_H_

#include <string>
#include <complex>

template<typename precision>
struct tolerance;

template<>
struct tolerance<double>
{
    static constexpr double value = 1e-6;
};

template<>
struct tolerance<float>
{
    static constexpr double value = 1e-5;
};


template<typename T>
struct type_util
{
    using scalar_type = T;
};

template<typename T>
struct type_util<std::complex<T> >
{
    using scalar_type = T;
};

std::string get_cmd_string(int argc, char *argv[]) {
    std::string test_cmd= "";
    for (int i = 0; i < argc; i++) {
        test_cmd.append(std::string(argv[i]).append(" "));
    }
    return test_cmd;
}

template<typename T, typename Iter>
T get_product(Iter begin, Iter end) {
    T result = 1;
    for (auto it = begin; it != end; ++it) {
        result *= (*it);
    }
    return result;
}
#endif // NVPLFFT_EXAMPLE_UNTILS_H_
