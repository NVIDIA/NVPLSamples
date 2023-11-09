# NVPL ScaLAPACK Usage Example

## Languages

ScaLAPACK Fortran and C++ examples are provided demonstrating use with CMake build systems.

# Configuring

If NVPL libraries are installed by the packaged manager they should be already on CMAKE_PREFIX_PATH.
Otherwise, when NVPL libraries are installed to a custom prefix,
  * `export nvpl_ROOT=/opt/nvpl`, or wherever the NVPL installation is based.
  * `export LD_LIBRARY_PATH=${nvpl_ROOT}/lib:$LD_LIBRARY_PATH`

## Configure MPI

The examples use `FindMPI.cmake` to find MPI.
  *[CMake FindMPI documentation](https://cmake.org/cmake/help/latest/module/FindMPI.html)

### Supported MPI Distributions:
 * [NVIDIA HPC-X](https://www.nvidia.com/en-eu/networking/infiniband/hpc-x/)
 * [OpenMPI-5.x](https://www.open-mpi.org/software/ompi/v5.0/)
 * [OpenMPI-4.x](https://www.open-mpi.org/software/ompi/v4.1/)
 * [OpenMPI-3.x](https://www.open-mpi.org/software/ompi/v3.1/)
 * [MPICH-4.x](https://www.mpich.org/downloads/)
 * And any [MPICH ABI compatible distributions](https://www.mpich.org/abi/).

**NOTE:**  Application using NVHPC-23.9 compilers together with NVPL ScaLAPACK should use
the `nvhpc-hpcx-cuda12` modules from the [NVIDIA HPC SDK](https://developer.nvidia.com/hpc-sdk).
These communication libraries currently provide better performance even when
CUDA-12 is not used by the application.

### Ubuntu/Debian
Ubuntu, Debian and other apt-based systems use a executable suffixes to distinguish different MPI Distributions.

#### OpenMPI-4
Use the `openmpi4` BLACS interface with GCC or CLANG compilers and the `libopenmpi-dev` package.
```sh
$ sudo apt install libopenmpi-dev
$ export MPI_EXECUTABLE_SUFFIX=.openmpi
```

#### MPICH
Use the `mpich` BLACS interface with GCC or CLANG compilers and the `libmpich-dev` package.
```sh
$ sudo apt install libmpich-dev
$ export MPI_EXECUTABLE_SUFFIX=.mpich
```

#### NVHPC-HPCX
First [install the NVIDIA HPC SDK](https://developer.nvidia.com/hpc-sdk-downloads).
For NVHPC compilers and HPC-X communication libraries, use environment modules, and
select the `openmpi4` NVPL BLACS interface.
```sh
$ sudo apt install nvhpc-23-9 environment-modules
$ . /etc/profile
$ module use /opt/nvidia/hpc_sdk/modulefiles/
$ module purge
$ module load nvhpc-hpcx-cuda12
$ . /opt/nvidia/hpc_sdk/Linux_aarch64/2023/comm_libs/12.2/hpcx/latest/hpcx-init.sh
$ hpcx_load
```

### RHEL/Fedora/AmazonLinux

Redhat, Fedora and AmazonLinux primarily use environment modules to swtich between multiple MPI distributions.

#### OpenMPI-4
Use the `openmpi4` BLACS interface with GCC or CLANG compilers and the `openmpi-devel` package.
```sh
$ sudo yum install openmpi-devel
$ . /etc/profile
$ module load mpi/openmpi-aarch64
```

#### MPICH
Use the `mpich` BLACS interface with GCC or CLANG compilers and the `mpich-devel` package.
```sh
$ sudo yum install mpich-devel
$ . /etc/profile
$ module load mpi/mpich-aarch64
```
Note that `amazonlinux-2` does not have support for MPICH.  To MPICH on
amazonliunx, users should upgrade to `amazonlinux-2023`.

#### NVHPC-HPCX
First [install the NVIDIA HPC SDK](https://developer.nvidia.com/hpc-sdk-downloads).
Use the `openmpi4` BLACS interface with NVHPC compilers and HPC-X communication libraries.
```
$ sudo yum install nvhpc-23-9 environment-modules
$ . /etc/profile
$ module use /opt/nvidia/hpc_sdk/modulefiles/
$ module purge
$ module load nvhpc-hpcx-cuda12
$ . /opt/nvidia/hpc_sdk/Linux_aarch64/2023/comm_libs/12.2/hpcx/latest/hpcx-init.sh
$ hpcx_load
```


### OpenSUSE / LEAP

OpenSUSE uses an `mpi-selector` utility to allow switching between multiple MPI distributions.

#### OpenMPI-3
Use the `openmpi3` BLACS interface with GCC or CLANG compilers and the `openmpi3-devel` package.
```sh
$ sudo zypper install openmpi3-devel
$ mpi-selector --user --set openmpi3
$  . /etc/profile.d/mpi-selector.sh
```

#### OpenMPI-4
Use the `openmpi4` BLACS interface with GCC or CLANG compilers and the `openmpi-devel` package.
```sh
$ sudo zypper install openmpi-devel
$ mpi-selector --user --set openmpi
$  . /etc/profile.d/mpi-selector.sh
```

#### MPICH
Use the `mpich` BLACS interface with GCC or CLANG compilers and the `mpich-devel` package.
```sh
$ sudo zypper install mpich-devel
$ mpi-selector --user --set mpich
$  . /etc/profile.d/mpi-selector.sh
```

#### NVHPC-HPCX
First [install the NVIDIA HPC SDK](https://developer.nvidia.com/hpc-sdk-downloads).
Then use the `openmpi4` BLACS interface with NVHPC compilers and HPC-X communication libraries.
```sh
$ sudo zypper install nvhpc-23-9 environment-modules
$ . /etc/profile
$ module use /opt/nvidia/hpc_sdk/modulefiles/
$ module purge
$ module load nvhpc-hpcx-cuda12
$ . /opt/nvidia/hpc_sdk/Linux_aarch64/2023/comm_libs/12.2/hpcx/latest/hpcx-init.sh
$ hpcx_load
```

## Building examples

Build the examples with normal CMake build procedures, and set cmake variables `INT_TYPE` ,`THR_TYPE` , `MPI_INTERFACE`.
For example:

```sh
    $ cmake -S. -Bbuild -DINT_TYPE=lp64 -DTHR_TYPE=seq -DMPI_INTERFACE=openmpi4
    $ cmake --build build
```

### CTest

```sh
    $ ctest --test-dir build
```
### CMake Presets

Alternatively with `>=CMake-3.23`, [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) are provided.

```sh
    $ cmake --preset default
    $ cmake --build --preset default
    $ ctest --preset default

```

Or find appropriate interface preset with:
```sh
    $ cmake --list-presets
```

### Non-CMake build systems

If NVPL has been installed by the package manager include files and libraries should already be
on the paths for the compiler.

Otherwise when NVPL has been installed to a custom location given by the `nvpl_ROOT` environment variable.
* Set include path: `export CPATH=${nvpl_ROOT}/include:$CPATH`
* Set library path: `export LIBRARY_PATH=${nvpl_ROOT}/include:$CPATH`

Ensure the appropriate compiler wrappers are used to build the application with MPI support, or MPI support is otherwise properly configured.

Next add the NVPL libraries to the application's link dependencies:
* `-lnvpl_scalapack_<int>`
* `-lnvpl_blacs_<int>_<mpi>`
* `-lnvpl_lapack_<int>_<thr>`
* `-lnvpl_blas_<int>_<thr>`

Where the following options are configurable:
* `<int>`:= `lp64`, `ilp64`
* `<thr>`:= `seq`, `omp`
* `<mpi>`:= `openmpi3`,`openmpi4`, `openmpi5`, `mpich`


# Acknowledgments

These samples use code from [Netlib ScaLAPACK](https://www.netlib.org/scalapack/), under the
terms of the [Netlib LICENSE](https://www.netlib.org/scalapack/LICENSE).
```
Copyright (c) 1992-2011 The University of Tennessee and The University
                        of Tennessee Research Foundation.  All rights
                        reserved.
Copyright (c) 2000-2011 The University of California Berkeley. All
                        rights reserved.
Copyright (c) 2006-2011 The University of Colorado Denver.  All rights
                        reserved.

$COPYRIGHT$

Additional copyrights may follow

$HEADER$

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

- Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer listed
  in this license in the documentation and/or other materials
  provided with the distribution.

- Neither the name of the copyright holders nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

The copyright holders provide no reassurances that the source code
provided does not infringe any patent, copyright, or any other
intellectual property rights of third parties.  The copyright holders
disclaim any liability to any recipient for claims brought against
recipient by any third party for infringement of that parties
intellectual property rights.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
