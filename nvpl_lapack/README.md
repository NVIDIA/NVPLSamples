# NVPL LAPACK Usage Example

## Languages

Fortran and C++ examples are provided

## How to Build

Set `PATH_TO_NVPL_INSTALLATION` to your NVPL installation (e.g., `~/install/nvpl`) and execute in the directory of the samples:

```
CMAKE_PREFIX_PATH=$PATH_TO_NVPL_INSTALLATION cmake --preset default -Dnvpl_ROOT=$PATH_TO_NVPL_INSTALLATION
cmake --build --preset default -j
```

Or build without CMake Presets:

```
mkdir -p build/release
cd build/release
CMAKE_PREFIX_PATH=$PATH_TO_NVPL_INSTALLATION cmake -DCMAKE_BUILD_TYPE=Release -Dnvpl_ROOT=$PATH_TO_NVPL_INSTALLATION ../..
make -j
```
