# NVPL Tensor Examples

* [nvpl_tensor_version](nvpl_tensor_version/README.md) - Hello world example.  Report version and exit.
* [contraction](contraction/README.md) - Contraction example.
* [reduction](reduction/README.md) - Reduction example.
* [permutation](permutation/README.md) - Permutation example.
* [elementwise_binary](elementwise_binary/README.md) - Binary elementwise example.
* [elementwise_trinary](elementwise_trinary/README.md) - Trinary elementwise example.

## Build

If NVPL is not installed under a default system prefix (e.g., `/usr`), then ensure environment variable `nvpl_ROOT` is set to the alternate NVPL root prefix (e.g., `$HOME/nvpl`).

### CMake

```bash
$ cmake -S . -B build/default
$ cmake --build build/default
```

### CMake Presets

Alternatively use CMake presets which require `>=CMake-3.23`. See:
* [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html).
* [Installing CMake](https://cmake.org/install/).

```bash
$ cmake --preset default
$ cmake --build --preset default
$ ctest --preset default
```

## Run

```bash
$ ./build/default/<example-exe>
```
