#include <stdio.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main()
{
    // Initialization.
    const nvpl_int_t n = 4; // Number of rows of A and b matrices.
    const nvpl_int_t nrhs = 2; // Number of columns of b.
    const nvpl_int_t lda = n; // Leading dimension of A.
    const nvpl_int_t ldb = nrhs; // Leading dimension of b.

    // Matrix A has n x n dimensions with lda = n for row-major ordering.
    // A =  4    5   -3    7
    //      5   16    0   17
    //     -3    0   15   -8
    //      7   17   -8   23
    float A[lda * n] = {4, 5, -3, 7, 5, 16, 0, 17, -3, 0, 15, -8, 7, 17, -8, 23};

    // Matrix b has n x nrhs dimensions with ldb = n for row-major ordering.
    // b =  -3   17
    //       9   55
    //      47   11
    //     -16   54
    float b[ldb * n] = {-3, 17, 9, 55, 47, 11, -16, 54};

    printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

    // Print inputs.
    print_smatrix_rowmajor("Entry Matrix A", n, n, A, lda);
    print_smatrix_rowmajor("Right Hand Side b", n, nrhs, b, ldb);
    printf("\n");
    printf("LAPACKE_sposv (row-major, high-level) Example Program Results\n");

    // Solve A * x = b using Cholesky decomposition.
    char lower = 'L';
    nvpl_int_t info = LAPACKE_sposv(LAPACK_ROW_MAJOR, lower, n, nrhs, A, lda,
            b, ldb);

    // Any errors?
    if (info == LAPACK_WORK_MEMORY_ERROR)
        printf("Not enough memory for work arrays.\n");
    else if (info == LAPACK_TRANSPOSE_MEMORY_ERROR)
        printf("Not enough memory for internal transpose.\n");
    else if (info < 0)
        printf("Illegal input argument.\n");
    else if (info > 0)
        printf("Matrix A is not positive-definite.\n");

    if (info != 0) return info;

    // Print solution.
    print_smatrix_rowmajor("Solution", n, nrhs, b, ldb);
    printf("\n");

    // Print Cholesky factor.
    print_smatrix_rowmajor("Cholesky factor", n, n, A, lda);
    printf("\n");

    return info;
}
