#include <stdio.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main() {
    // Initialization.
    const nvpl_int_t m = 4;   // Number of rows of A matrix.
    const nvpl_int_t n = 4;   // Number of columns of A matrix.
    const nvpl_int_t lda = n; // Leading dimension of A (row-major: lda = n).

    // Matrix A has m x n dimensions with lda = n for row-major ordering.
    // A =  1   2   2  -3
    //      0   1   2   1
    //     -1   2   0   1
    //      3  -1   2   2
    double A[lda * m] = {1, 2, 2, -3, 0, 1, 2, 1, -1, 2, 0, 1, 3, -1, 2, 2};

    // Pivot indices that define permutation matrix P of LU factorization.
    nvpl_int_t ipiv[m] = {0};
    printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

    // Print inputs.
    print_dmatrix_rowmajor("Entry Matrix A (row-major layout)", m, n, A, lda);
    printf("\n");
    printf("LAPACKE_dgetrf (row-major) Example Program Results\n");

    // Compute LU factorization P * A = L * U.
    nvpl_int_t info = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, m, n, A, lda, ipiv);

    // Check results
    if (info == LAPACK_WORK_MEMORY_ERROR)
        printf("Not enough memory for work arrays.\n");
    else if (info == LAPACK_TRANSPOSE_MEMORY_ERROR)
        printf("Not enough memory for internal transpose.\n");
    else if (info < 0)
        printf("Illegal input argument.\n");
    else if (info > 0)
        printf("Matrix A is singular\n");

    if (info != 0) return info;

    // Print LU factors.
    print_dmatrix_rowmajor("LU factors", m, n, A, lda);
    printf("\n");

    // Print pivot indices.
    print_vector("Pivot indices", m, ipiv);
    printf("\n");

    return info;
}
