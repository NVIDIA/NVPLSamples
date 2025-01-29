# NVPL LAPACK Examples

The examples demonstrate how to call NVPL LAPACK from C++ and Fortran
languages.

## Build

The examples use CMake to build. Set `PATH_TO_NVPL_INSTALLATION` to your NVPL
installation (e.g., `~/install/nvpl`) and execute in the directory of the
NVPL LAPACK examples:

```sh
cmake --preset default -Dnvpl_ROOT=$PATH_TO_NVPL_INSTALLATION
cmake --build --preset default -j
```

Or optionally, to build without CMake Presets:

```sh
mkdir -p build/default
cd build/default
cmake -Dnvpl_ROOT=$PATH_TO_NVPL_INSTALLATION ../..
make -j
```

The built examples will be located in `build/default` directory.

## Run

To run all examples at once use `ctest` command:

```sh
cd build/default
ctest
```

The examples can be run manually as well, for instance:

```sh
# 64-bit integer, sequential.
./build/default/ex_lapacke_dgels_colmajor_ilp64_seq
```

```sh
# 32-bit integer, multithread using at most 8 threads.
OMP_NUM_THREADS=8 ./build/default/ex_lapacke_dgels_colmajor_lp64_omp
```
