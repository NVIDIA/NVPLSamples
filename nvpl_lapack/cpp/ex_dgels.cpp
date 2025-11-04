#include <stdio.h>
#include <stdlib.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main() {
    // Initialization.
    const nvpl_int_t m = 5;    // Number of rows of A and b matrices.
    const nvpl_int_t n = 3;    // Number of columns of A.
    const nvpl_int_t nrhs = 2; // Number of columns of b.
    const nvpl_int_t lda = m;  // Leading dimension of A.
    const nvpl_int_t ldb = m;  // Leading dimension of b.

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
    print_dmatrix_colmajor("Entry Matrix A", m, n, A, lda);
    print_dmatrix_colmajor("Right Hand Side b", m, nrhs, b, ldb);
    printf("\n");
    printf("dgels Example Program Results\n");

    // Solve least squares problem: min_x || A * x - b ||.
    char trans = 'N';
    double *work = NULL;
    double work_query;
    nvpl_int_t lwork = -1;
    nvpl_int_t info;

    // Workspace query.
    NVPL_LAPACK_dgels(&trans, &m, &n, &nrhs, A, &lda, b, &ldb, &work_query,
            &lwork, &info);
    lwork = (nvpl_int_t)work_query;
    work = (double *)malloc(sizeof(double) * lwork);
    if (!work) {
        printf("Not enough memory for work arrays.\n");
        exit(-1);
    }

    // Solve least squares problem.
    NVPL_LAPACK_dgels(
            &trans, &m, &n, &nrhs, A, &lda, b, &ldb, work, &lwork, &info);

    // Any errors?
    if (info < 0)
        printf("Illegal input argument.\n");
    else if (info > 0)
        printf("Matrix A doesn't have a full rank.\n");

    if (info != 0) {
        free(work);
        return info;
    }

    // Print solution.
    print_dmatrix_colmajor("Solution", n, nrhs, b, ldb);
    printf("\n");

    free(work);
    return info;
}