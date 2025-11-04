#include <stdio.h>
#include <stdlib.h>
#include <nvpl_lapack.h>

#include "utils.h"

int main() {
    // Initialization.
    const nvpl_int_t n = 4;   // Number of rows and columns of A.
    const nvpl_int_t lda = n; // Leading dimension of A.

    // Matrix A has n x n dimensions with lda = n for column-major ordering.
    // A = 13    3   -6   -5
    //      3   13   -6   -5
    //     -6   -6   31    7
    //     -5   -5    7   23
    float A[lda * n]
            = {13, 3, -6, -5, 3, 13, -6, -5, -6, -6, 31, 7, -5, -5, 7, 23};

    // Eigenvalues of A.
    float w[n] = {0};

    printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

    // Print inputs.
    print_smatrix_colmajor("Entry Matrix A", n, n, A, lda);
    printf("\n");
    printf("ssyev Example Program Results\n");

    // Compute eigenvalue decomposition.
    char jobz = 'V';
    char uplo = 'L';
    float *work = NULL;
    float work_query;
    nvpl_int_t lwork = -1;
    nvpl_int_t info;

    // Workspace query.
    NVPL_LAPACK_ssyev(&jobz, &uplo, &n, A, &lda, w, &work_query, &lwork, &info);
    lwork = (nvpl_int_t)work_query;
    work = (float *)malloc(sizeof(float) * lwork);
    if (!work) {
        printf("Not enough memory for work arrays.\n");
        exit(-1);
    }

    // Compute eigenvalues and eigenvectors.
    NVPL_LAPACK_ssyev(&jobz, &uplo, &n, A, &lda, w, work, &lwork, &info);

    // Any errors?
    if (info < 0)
        printf("Illegal input argument.\n");
    else if (info > 0)
        printf("The algorithm failed to converge.\n");

    if (info != 0) {
        free(work);
        return info;
    }

    // Print eigenvalues.
    print_svector("Eigenvalues", n, w);
    printf("\n");

    // Print eigenvectors.
    print_smatrix_colmajor("Eigenvectors", n, n, A, lda);
    printf("\n");

    free(work);
    return info;
}