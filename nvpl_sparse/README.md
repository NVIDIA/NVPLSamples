# NVPL SPARSE Examples

The examples demonstrate how to call NVPL SPARSE from C language.

## Build

The examples use CMake to build.

``` sh
cmake -S. -Bbuild <cmake-options>
cmake --build build
```

## Run

The simplest way to run all examples is using `ctest`:
``` sh
cd build
ctest
```

The examples can be run manually as well, for instance:
``` sh
./build/c/c_spmv_coo
```
