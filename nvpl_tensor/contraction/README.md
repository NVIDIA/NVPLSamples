# contraction_nvpl_tensor

A application demonstrating contraction computation using nvplTensor for C language.

## Build

If NVPL is not installed under a default system prefix (e.g., `/usr`), then ensure environment variable `nvpl_ROOT` is set to the alternate NVPL root prefix (e.g., `$HOME/nvpl`).

### CMake

```bash
$ cmake -S . -B build/default
$ cmake --build build/default
```

## Run

```bash
$ ./build/default/nvpl_tensor_contraction_c
```
