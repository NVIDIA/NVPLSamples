#include <stdio.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main()
{
    // Initialization.
    const nvpl_int_t n = 4; // Number of rows and columns of A.
    const nvpl_int_t lda = n; // Leading dimension of A.

    // Matrix A has n x n dimensions with lda = n for row-major ordering.
    // A = 13    3   -6   -5
    //      3   13   -6   -5
    //     -6   -6   31    7
    //     -5   -5    7   23
    double A[lda * n] = {13, 3, -6, -5, 3, 13, -6, -5, -6, -6, 31, 7, -5, -5, 7, 23};

    // Eigenvalues of A.
    double w[n] = {0};

    printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

    // Print inputs.
    print_dmatrix_rowmajor("Entry Matrix A", n, n, A, lda);
    printf("\n");
    printf("LAPACKE_dsyev (row-major, high-level) Example Program Results\n");

    // Compute eigenvalue decomposition.
    char eigen_vectors = 'V';
    char lower = 'L';
    nvpl_int_t info = LAPACKE_dsyev(LAPACK_ROW_MAJOR, eigen_vectors, lower,
            n, A, lda, w);

    // Any errors?
    if (info == LAPACK_WORK_MEMORY_ERROR)
        printf("Not enough memory for work arrays.\n");
    else if (info == LAPACK_TRANSPOSE_MEMORY_ERROR)
        printf("Not enough memory for internal transpose.\n");
    else if (info < 0)
        printf("Illegal input argument.\n");
    else if (info > 0)
        printf("The algorithm failed to converge.\n");

    if (info != 0) return info;

    // Print eigenvalues.
    print_dvector("Eigenvalues", n, w);
    printf("\n");

    // Print eigenvectors.
    print_dmatrix_rowmajor("Eigenvectors", n, n, A, lda);
    printf("\n");

    return info;
}
