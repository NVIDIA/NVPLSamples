#include <stdio.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main() {
    // Initialization.
    const nvpl_int_t n = 4;      // Number of rows of A and b matrices.
    const nvpl_int_t nrhs = 2;   // Number of columns of b.
    const nvpl_int_t lda = n;    // Leading dimension of A.
    const nvpl_int_t ldb = nrhs; // Leading dimension of b.

    // Matrix A has n x n dimensions with lda = n for row-major ordering.
    // A =  1   2   2  -3
    //      0   1   2   1
    //     -1   2   0   1
    //      3  -1   2   2
    nvpl_scomplex_t A[lda * n]
            = {1, 2, 2, -3, 0, 1, 2, 1, -1, 2, 0, 1, 3, -1, 2, 2};

    // Matrix b has n x nrhs dimensions with ldb = nrhs for row-major ordering.
    // b = 13    1
    //      6    7
    //     -1    3
    //      9   10
    nvpl_scomplex_t b[ldb * n] = {13, 1, 6, 7, -1, 3, 9, 10};

    // Pivot indices that define permutation matrix P of LU factorization.
    nvpl_int_t ipiv[n] = {0};
    printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

    // Print inputs.
    print_cmatrix_rowmajor("Entry Matrix A", n, n, A, lda);
    print_cmatrix_rowmajor("Right Hand Side b", n, nrhs, b, ldb);
    printf("\n");
    printf("LAPACKE_cgesv (row-major, high-level) Example Program Results\n");

    // Solve A * x = b.
    nvpl_int_t info
            = LAPACKE_cgesv(LAPACK_ROW_MAJOR, n, nrhs, A, lda, ipiv, b, ldb);

    // Any errors?
    if (info == LAPACK_WORK_MEMORY_ERROR)
        printf("Not enough memory for work arrays.\n");
    else if (info == LAPACK_TRANSPOSE_MEMORY_ERROR)
        printf("Not enough memory for internal transpose.\n");
    else if (info < 0)
        printf("Illegal input argument.\n");
    else if (info > 0)
        printf("Matrix A is singular.\n");

    if (info != 0) return info;

    // Print solution.
    print_cmatrix_rowmajor("Solution", n, nrhs, b, ldb);
    printf("\n");

    // Print LU factors.
    print_cmatrix_rowmajor("LU factors", n, n, A, lda);
    printf("\n");

    // Print pivot indices.
    print_vector("Pivot indices", n, ipiv);
    printf("\n");

    return info;
}
