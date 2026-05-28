#include <stdio.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main() {
    // Initialization.
    const nvpl_int_t n = 4;   // Order of matrix A.
    const nvpl_int_t lda = n; // Leading dimension of A.

    // Matrix A has n x n dimensions with lda = n for column-major ordering.
    // A =  1   2   2  -3
    //      0   1   2   1
    //     -1   2   0   1
    //      3  -1   2   2
    nvpl_dcomplex_t A[lda * n]
            = {1, 0, -1, 3, 2, 1, 2, -1, 2, 2, 0, 2, -3, 1, 1, 2};

    // Pivot indices that define permutation matrix P of LU factorization.
    nvpl_int_t ipiv[n] = {0};

    printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

    // Print inputs.
    print_zmatrix_colmajor("Entry Matrix A", n, n, A, lda);
    printf("\n");
    printf("zgetrf Example Program Results\n");

    // Compute LU factorization: P * A = L * U
    nvpl_int_t info;
    NVPL_LAPACK_zgetrf(&n, &n, A, &lda, ipiv, &info);

    // Any errors?
    if (info < 0)
        printf("Illegal input argument.\n");
    else if (info > 0)
        printf("Matrix A is singular.\n");

    if (info != 0) return info;

    // Print LU factors.
    print_zmatrix_colmajor("LU factors", n, n, A, lda);
    printf("\n");

    // Print pivot indices.
    print_vector("Pivot indices", n, ipiv);
    printf("\n");

    return info;
}
