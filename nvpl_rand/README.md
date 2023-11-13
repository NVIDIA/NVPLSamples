# NVPL RAND Examples

The examples demonstrate how to use single- and multi-threaded NVPL RAND.
Please check the documentation for the descriptions of the examples.

## Build

The examples use CMake to build.

``` sh
mkdir build && cd build
nvpl_ROOT=<NVPL install location> cmake ../.
make
```

## Run

The simplest way to run all examples is by executing `ctest` in `build` directory:

``` sh
cd build
ctest
```

The examples can be run manually as well, for instance:

``` sh
./build/example/rand/rand_st_distribution_config
./build/example/rand/rand_mt_generate_normal_fp64
```
