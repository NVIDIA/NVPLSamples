PROGRAM usageexample
    REAL, ALLOCATABLE :: A(:, :)
    EXTERNAL SPOTRF
    INTEGER M, N
    INTEGER lda
    INTEGER info
    REAL, POINTER :: ipiv => NULL()
    M=10
    N=10
    lda = M
    info = 0
    ALLOCATE (A(lda, N))
    CALL RANDOM_NUMBER(A)
    DO 10 i = 1, M
        A(i, i) = A(i, i) + REAL(M) * 2.0
        DO 20 j = 1, N
            PRINT *, i, j, A(i, j)
20      CONTINUE
10  CONTINUE

    PRINT *, "lda = ", lda
    CALL SGETRF(M, N, A, lda, ipiv, info)
    PRINT *, "info = ", info
    DEALLOCATE(A)
END PROGRAM usageexample
