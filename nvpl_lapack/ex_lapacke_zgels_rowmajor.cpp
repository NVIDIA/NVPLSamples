#include <stdio.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main()
{
    // Initialization.
    const nvpl_int_t m = 5; // Number of rows of A and b matrices.
    const nvpl_int_t n = 3; // Number of columns of A.
    const nvpl_int_t nrhs = 2; // Number of columns of b.
    const nvpl_int_t lda = n; // Leading dimension of A.
    const nvpl_int_t ldb = nrhs; // Leading dimension of b.

    // Matrix A has m x n dimensions with lda = m for row-major ordering.
    // A = 1   1   1
    //     2   3   4
    //     3   5   2
    //     4   2   5
    //     5   4   3
    nvpl_dcomplex_t A[lda * m] = {1, 1, 1, 2, 3, 4, 3, 5, 2, 4, 2, 5, 5, 4, 3};

    // Matrix b has m x nrhs dimensions with ldb = m for row-major ordering.
    // b = -10  -3
    //      12  14
    //      14  12
    //      16  16
    //      18  16
    nvpl_dcomplex_t b[ldb * m] = {-10, -3, 12, 14, 14, 12, 16, 16, 18, 16};

    printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

    // Print inputs.
    print_zmatrix_rowmajor("Entry Matrix A", m, n, A, lda);
    print_zmatrix_rowmajor("Right Hand Side b", m, nrhs, b, ldb);
    printf("\n");
    printf("LAPACKE_zgels (row-major, high-level) Example Program Results\n");

    // Solve least square problem: min_x || A * x - b ||.
    char no_transpose = 'N';
    nvpl_int_t info = LAPACKE_zgels(LAPACK_ROW_MAJOR, no_transpose, m, n, nrhs,
            A, lda, b, ldb);

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
    print_zmatrix_rowmajor("Solution", n, nrhs, b, ldb);
    printf("\n");

    return info;
}
