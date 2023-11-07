# NVPL BLAS Examples

The examples demonstrate how to call NVPL BLAS from C and Fortran languages.
Please check the corresponding subdirectories.

## Build

The examples use CMake to build.

``` sh
cmake -S. -Bbuild <cmake-options>
cmake --build build
```

The built examples will be located in `build` directory.
Optionally, the examples can be installed with `cmake --install build`.

The following options are available:

CMake Option     | Supported Values (**defaults in bold**) | Description
---------------- | --------------------------------------- | -----------
`INT_TYPE`       | **lp64**, ilp64                         | Specifies the integer type: 32-bit (`lp64`), 64-bit (`ilp64`)
`THR_TYPE`       | **omp**, seq                            | Specifies the threading runtime: OpenMP (`omp`), sequential (`seq`)


## Run

The simplest way to run all examples is using `ctest`:
``` sh
cd build
ctest
```

The examples can be run manually as well, for instance:
``` sh
./build/c/c_ilp64_omp_sgemm
./build/f77/f77_ilp64_omp_icamax
```
