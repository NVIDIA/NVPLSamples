# NVPL FFT Examples

## Description
### C++ examples
* `cxx/1d_c2c_<single/double>_example` computes a forward and a backward 1D C2C FFTs in single/double precision with contiguous data. One batch at a time.
* `cxx/c2c_<single/double>_many_example` computes batched forward and backward 1D/2D/3D C2C FFTs in single/double precision with strided data.
* `cxx/r2c_c2r_<single/double>_many_example` computes batched forward R2C and backward C2R 1D/2D/3D FFTs in single/double precision with strided data.
* `cxx/<r2c_c2r/c2c>_single_withomp_example` computes batched forward and backward 1D FFTs in single precision with contiguous data. Plan creation and execution are called inside an omp parallel region.
* `cxx/c2c_c2r_r2c_many_bench_example` measures performance of  1D/2D/3D C2C/C2R/R2C FFTs with single and double precision.
* `cxx/c2c_c2r_r2c_single_apis_example` demonstrates usage of simple and advanced FFTW APIs for computing C2C / R2C / C2R FFTs with inplace / out-of-place data.
* `cxx/auxiliary_apis_example` demonstrates usage of few auxiliary APIs.
* `cxx/include_header_example` demonstrates inclusion of `fftw3.h` (as opposed to using `nvpl_fftw3.h`).

### Fortran 77 / 2003 examples
* `c2c_c2r_r2c_double_apis_example_<03/77>` demonstrates usage of fortran API by computing double precision 1D/2D/3D C2C/C2R/R2C FFTs.
* `c2c_c2r_r2c_single_apis_example_03` demonstrates usage of fortran API by computing single preicision 1D/2D/3D C2C/C2R/R2C FFTs.

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
./cxx/1d_c2c_single_example NX
./cxx/1d_c2c_double_example NX
./cxx/c2c_single_many_example NTHREAD NX [NY [NZ]]
./cxx/c2c_double_many_example NTHREAD NX [NY [NZ]]
./cxx/r2c_c2r_single_many_example NTHREAD NX [NY [NZ]]
./cxx/r2c_c2r_double_many_example NTHREAD NX [NY [NZ]]
./cxx/c2c_single_withomp_example NX [NY [NZ]]
./cxx/r2c_c2r_single_withomp_example NX [NY [NZ]]
./cxx/c2c_c2r_r2c_single_apis_example NX [NY [NZ]]
./cxx/auxiliary_apis_example
./cxx/include_header_example
./fortran/c2c_c2r_r2c_double_apis_example.f
./fortran/c2c_c2r_r2c_double_apis_example.f90
./fortran/c2c_c2r_r2c_single_apis_example.f90

```
### c2c_c2r_r2c_many_bench_example
```
Usage: ./c2c_r2c_c2r_many_bench_example
Arguments:
	--prec precision:          The precision of the transform fp32 or fp64.
	--fft_type fft_type:       The type of the transform c2c, r2c or c2r.
	--mode mode:               (optional) The mode of the transform ip or oop (default: ip).
	--config config_name:      (optional) Name of the config to be logged (default: no_config).
	--cat bench_category:      (optional) The case to benchmark p_2357, f_2357_l_512_r_1, f_2357_l_512_r_2, varargs_r_1 (default: p_2357).
	--size data_size:          (optional) Transform data size. Supported options:
	                           * 0 - default, total data size is 256 MB.
	                           * <number> - number of batches to process for each FFT size.
	                           * <number>k or <number>m - for example 64m - the size of data in KB or MB to process.
	--cycles cycles:           (optional) The number of cycles (default: 100).
	--warmup warmup_runs:      (optional) The number of warm-up runs (default: 10).
	--fft_sizes *fft_sizes:    (optional) If `varargs_r_1` is selected, fft sizes can be listed manually (for rank 1). This must be the last argument!
```
