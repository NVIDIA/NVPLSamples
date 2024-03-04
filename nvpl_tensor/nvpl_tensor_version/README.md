# hello_nvpl_tensor

A hello-world application demonstrating correct header include paths and linker lines for C and CXX languages.  Print version and exit.

## Build

If NVPL is not installed under a default system prefix (e.g., `/usr`), then ensure environment variable `nvpl_ROOT` is set to the alternate NVPL root prefix (e.g., `$HOME/nvpl`).

### CMake

```bash
$ cmake -S . -B build/default
$ cmake --build build/default
```

## Run

```bash
$ ./build/default/nvpl_tensor_version_c
$ ./build/default/nvpl_tensor_version_cxx
```
