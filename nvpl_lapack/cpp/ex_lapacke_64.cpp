// This example demonstrates _64 ILP64 LAPACKE entry point: LAPACKE_dgels_64.
// It is identical to LAPACKE_dgels example except the size-related arguments
// are declared as nvpl_int64_t and the call target is the _64-suffixedvariant,
// which is exported from both LP64 and ILP64 libraries.

#include <stdio.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main() {
    // Initialization.
    const nvpl_int64_t m = 5;    // Number of rows of A and b matrices.
    const nvpl_int64_t n = 3;    // Number of columns of A.
    const nvpl_int64_t nrhs = 2; // Number of columns of b.
    const nvpl_int64_t lda = m;  // Leading dimension of A.
    const nvpl_int64_t ldb = m;  // Leading dimension of b.

    // Matrix A has m x n dimensions with lda = m for column-major ordering.
    // A = 1   1   1
    //     2   3   4
    //     3   5   2
    //     4   2   5
    //     5   4   3
    double A[lda * n] = {1, 2, 3, 4, 5, 1, 3, 5, 2, 4, 1, 4, 2, 5, 3};

    // Matrix b has m x nrhs dimensions with ldb = m for column-major ordering.
    // b = -10  -3
    //      12  14
    //      14  12
    //      16  16
    //      18  16
    double b[ldb * nrhs] = {-10, 12, 14, 16, 18, -3, 14, 12, 16, 16};

    printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

    // Print inputs.
    print_dmatrix_colmajor(
            "Entry Matrix A", (nvpl_int_t)m, (nvpl_int_t)n, A, (nvpl_int_t)lda);
    print_dmatrix_colmajor("Right Hand Side b", (nvpl_int_t)m, (nvpl_int_t)nrhs,
            b, (nvpl_int_t)ldb);
    printf("\n");
    printf("LAPACKE_dgels_64 (col-major, high-level) Example Program "
           "Results\n");

    // Solve least square problem: min_x || A * x - b ||.
    char no_transpose = 'N';
    nvpl_int64_t info = LAPACKE_dgels_64(
            LAPACK_COL_MAJOR, no_transpose, m, n, nrhs, A, lda, b, ldb);

    // Any errors?
    if (info == LAPACK_WORK_MEMORY_ERROR)
        printf("Not enough memory for work arrays.\n");
    else if (info == LAPACK_TRANSPOSE_MEMORY_ERROR)
        printf("Not enough memory for internal transpose.\n");
    else if (info < 0)
        printf("Illegal input argument.\n");
    else if (info > 0)
        printf("Matrix A doesn't have a full rank.\n");

    if (info != 0) return info;

    // Print solution.
    print_dmatrix_colmajor(
            "Solution", (nvpl_int_t)n, (nvpl_int_t)nrhs, b, (nvpl_int_t)ldb);
    printf("\n");

    return info;
}
