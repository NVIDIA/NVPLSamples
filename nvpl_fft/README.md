# NVPL FFT Examples

## Description
* `1d_c2c_<single/double>_example` computes a forward and a backward 1D C2C FFTs in single/double precision with contiguous data. One batch at a time.
* `c2c_<single/double>_many_example` computes batched forward and backward 1D/2D/3D C2C FFTs in single/double precision with strided data.
* `r2c_c2r_<single/double>_many_example` computes batched forward R2C and backward C2R 1D/2D/3D FFTs in single/double precision with strided data.
* `c2c_many_bench_example` measures performance of batched 1D/2D/3D C2C FFTs in single precision with contiguous data.
* `<r2c_c2r/c2c>_single_withomp_example` computes batched forward and backward 1D FFTs in single precision with contiguous data. Plan creation and execution are called inside an omp parallel region.

More details can be found in the source files.

## Build
```
mkdir build_example
cd build_example
cmake -Dnvpl_ROOT=/path/to/nvpl/ ../nvpl_fft/example/
make
```

## Run
```
./1d_c2c_single_example NX
./1d_c2c_double_example NX
./c2c_single_many_example NX [NY [NZ]]
./c2c_double_many_example NX [NY [NZ]]
./r2c_c2r_single_many_example NX [NY [NZ]]
./r2c_c2r_double_many_example NX [NY [NZ]]
./c2c_single_withomp_example NX [NY [NZ]]
./r2c_c2r_single_withomp_example NX [NY [NZ]]
./c2c_many_bench_example
```
