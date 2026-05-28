# NVPL Samples

The NVIDIA Performance Libraries (NVPL) are a collection of high-performance mathematical libraries optimized for NVIDIA aarch64 CPUs.

These CPU-only libraries have no dependencies on CUDA or CTK. They are drop-in replacements for standard C and Fortran mathematical APIs, allowing HPC applications to achieve maximum performance on NVIDIA CPU platforms.

The provided samples show how to call and link to NVPL libraries from Fortran, C, and C++ applications. Examples use [CMake](#cmake-usage), but can be adapted for custom build environments.

* [NVPL Documentation](https://docs.nvidia.com/nvpl/)

## Installation

* [NVPL Downloads](https://developer.nvidia.com/nvpl-downloads/)
* Latest release: **NVPL-26.5**

## Library Samples

Samples are compatible with the latest NVPL release. Compatibility with older releases is not guaranteed.

* [NVPL BLAS Samples](nvpl_blas/README.md)
* [NVPL FFT Samples](nvpl_fft/README.md)
* [NVPL LAPACK Samples](nvpl_lapack/README.md)
* [NVPL RAND Samples](nvpl_rand/README.md)
* [NVPL ScaLAPACK Samples](nvpl_scalapack/README.md)
* [NVPL Sparse Samples](nvpl_sparse/README.md)
* [NVPL Tensor Samples](nvpl_tensor/README.md)

## Support

### Systems

* Architecture: aarch64-linux
* Platform: Arm SBSA
* CPUs Supported
  * [NVIDIA Vera](https://www.nvidia.com/en-us/data-center/vera-cpu/) (Armv9.2-A Olympus)
  * [NVIDIA Grace](https://www.nvidia.com/en-us/data-center/grace-cpu/) (Armv9.0-A Neoverse-V2)
  * [NVIDIA DGX Spark / GB10](https://www.nvidia.com/en-us/products/workstations/dgx-spark/) (Armv9.2-A Cortex-X925/A725)
  * AWS Graviton 5 (Armv9.2-A Neoverse-V3)
  * AWS Graviton 4 (Armv9.0-A Neoverse-V2)
  * AWS Graviton 3/3e (Armv8.4-A Neoverse-V1)
  * AWS Graviton 2 (Armv8.2-A Neoverse-N1)
  * Ampere Altra (Armv8.2-A Neoverse-N1)
  * Any CPU with [Armv8.1-A or later architecture](https://developer.arm.com/documentation/109697/0100/Feature-descriptions/The-Armv8-1-architecture-extension)

* OS (Linux)
  * Amazon Linux: 2 (*EOL), 2023
  * Debian: 12, 13
  * Fedora: 42, 43
  * RHEL: RHEL8 (8.10), RHEL9 (9.7), RHEL10 (10.1)
  * openSUSE/Leap: 15.6, 16.0
  * SLES: SLES15 (15.6), SLES16 (16.0)
  * Ubuntu: 20.04, 22.04, 24.04, 26.04
  * Generally any Linux OS with support for aarch64
  * Minimum glibc supported: 2.26 for NVPL 26.5. The next NVPL release will support systems with glibc 2.28 or newer.

### Compilers

* GCC-8 - GCC-16+
* Clang-14 - Clang-22+
* [Clang for NVIDIA Grace](https://developer.nvidia.com/grace/clang/downloads): 16.x -- 21.x
* [NVIDIA HPC Compilers](https://developer.nvidia.com/hpc-compilers): 23.9 - 26.3+

### Languages

* C: All libraries
* C++: All libraries via C interfaces
* Fortran: Selected libraries
  * GFortran ABI
  * NVPL BLAS, LAPACK, and ScaLAPACK provide `lp64` and `ilp64` integer ABIs
  * NVPL FFT provides FFTW Fortran '77 and '03 compatible interfaces
  * See individual library sample documentation for further details

### OpenMP

All libraries support the following OpenMP runtime libraries. See individual library documentation for details and API extensions supporting nested parallelism.

* GCC: `libgomp.so`
* Clang: `libomp.so`
* NVHPC: `libnvomp.so`

### MPI

NVPL provides standard BLACS interfaces for the following MPI distributions. See the [NVPL ScaLAPACK Samples Documentation](nvpl_scalapack/README.md) for details.

* [MPICH](https://www.mpich.org/): Runtime support for `>=mpich-4.0`
* [OpenMPI-3.x](https://www.open-mpi.org/doc/v3.1/)
* [OpenMPI-4.x](https://www.open-mpi.org/doc/v4.1/)
* [OpenMPI-5.x](https://docs.open-mpi.org/en/v5.0.x/)
* [NVIDIA HPC-X](https://developer.nvidia.com/networking/hpc-x): Use `openmpi4` BLACS interface

## CMake Usage

NVPL provides [CMake Package
Config](https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html)
files for each component library.

### Finding NVPL Packages

If NVPL was installed via the OS package manager under the `/usr`
directory, the NVPL packages will already be on the default
`CMAKE_PREFIX_PATH`. The `nvpl_ROOT` environment can be used to override
the default search path and force finding NVPL under a specific prefix.

The
[find_package()](https://cmake.org/cmake/help/latest/command/find_package.html)
command is used to find NVPL and any component libraries:

```cmake
find_package(nvpl)
```

Each NVPL component library found will print a brief status message with
important locations.

* Variable `nvpl_FOUND` will be true if NVPL is successfully found
* Variable `nvpl_VERSION` will contain the found version
* Pass the `REQUIRED` keyword to raise an error if `nvpl` package is
  not found.
* Regardless of the `COMPONENTS` keyword, all available NVPL component
  libraries installed in the same prefix will be found.
* To raise an error if a particular component is not found, use
  `REQUIRED COMPONENTS ...`
* Set `QUIET` to suppress status messages from `find_package()`. Use
  `REQUIRED` if a missing package should be a fatal error.
* `find_package(nvpl)` can safely be called multiple times from
  different locations in a project.

### Linking to NVPL Packages

The NVPL component libraries provide [Imported Interface
Targets](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#imported-targets)
under the common `nvpl::` namespace. To add all the necessary flags to
compile and link against NVPL libraries, use the
[target_link_libraries()](https://cmake.org/cmake/help/latest/command/target_link_libraries.html)
command:

```cmake
target_link_libraries(my_target PUBLIC nvpl::<lib>_<opts>)
```

Here `<lib>` is the lowercase shorthand for the library/API, and `<opts>`
are defined by the library.

### NVPL Targets

NVPL component and target names use an all-lowercase naming scheme. See individual
library documentation for details on available options.

| Component | Targets | Options / Notes |
| :--- | :--- | :--- |
| blas | `nvpl::blas_<int>_<thr>` | `<int>`: `lp64`, `ilp64`; `<thr>`: `seq`, `omp` |
| fft | `nvpl::fftw` | FFTW API interface |
| lapack | `nvpl::lapack_<int>_<thr>` | `<int>`: `lp64`, `ilp64`; `<thr>`: `seq`, `omp` |
| rand | `nvpl::rand`, `nvpl::rand_mt` | Single-threaded; multi-threaded (OpenMP) |
| scalapack | `nvpl::blacs_<int>_<mpi>`, `nvpl::scalapack_<int>` | `<int>`: `lp64`, `ilp64`; `<mpi>`: `mpich`, `openmpi3`, `openmpi4`, `openmpi5` |
| sparse | `nvpl::sparse` | |
| tensor | `nvpl::tensor` | |

### NVPL Variables

Each NVPL component library also exports variables

* `nvpl_<comp>_VERSION` - Version of component library
* `nvpl_<comp>_INCLUDE_DIR` - Full path to component headers directory
* `nvpl_<comp>_LIBRARY_DIR` - Full path to component libraries directory

## LICENSE

These samples are provided under the [NVIDIA Software License for NVPL SDK](./LICENSE).
