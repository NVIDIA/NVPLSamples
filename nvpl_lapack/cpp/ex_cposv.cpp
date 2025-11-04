#include <stdio.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main() {
    // Initialization.
    const nvpl_int_t n = 4;    // Number of rows of A and b matrices.
    const nvpl_int_t nrhs = 2; // Number of columns of b.
    const nvpl_int_t lda = n;  // Leading dimension of A.
    const nvpl_int_t ldb = n;  // Leading dimension of b.

    // Matrix A has n x n dimensions with lda = n for column-major ordering.
    // A =  4    5   -3    7
    //      5   16    0   17
    //     -3    0   15   -8
    //      7   17   -8   23
    nvpl_scomplex_t A[lda * n]
            = {4, 5, -3, 7, 5, 16, 0, 17, -3, 0, 15, -8, 7, 17, -8, 23};

    // Matrix b has n x nrhs dimensions with ldb = n for column-major ordering.
    // b =  -3   17
    //       9   55
    //      47   11
    //     -16   54
    nvpl_scomplex_t b[ldb * nrhs] = {-3, 9, 47, -16, 17, 55, 11, 54};

    printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

    // Print inputs.
    print_cmatrix_colmajor("Entry Matrix A", n, n, A, lda);
    print_cmatrix_colmajor("Right Hand Side b", n, nrhs, b, ldb);
    printf("\n");
    printf("cposv Example Program Results\n");

    // Solve A * x = b using Cholesky decomposition.
    char uplo = 'L';
    nvpl_int_t info;
    NVPL_LAPACK_cposv(&uplo, &n, &nrhs, A, &lda, b, &ldb, &info);

    // Any errors?
    if (info < 0)
        printf("Illegal input argument.\n");
    else if (info > 0)
        printf("Matrix A is not positive definite.\n");

    if (info != 0) return info;

    // Print solution.
    print_cmatrix_colmajor("Solution", n, nrhs, b, ldb);
    printf("\n");

    // Print Cholesky factors.
    print_cmatrix_colmajor("Cholesky factors", n, n, A, lda);
    printf("\n");

    return info;
}