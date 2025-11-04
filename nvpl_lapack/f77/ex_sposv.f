program usageexample
    implicit none
    real, allocatable :: a(:, :)
    real, allocatable :: b(:, :)
    external sposv

    integer n, nrhs
    integer lda, ldb
    integer info

    ! Problem dimensions
    n = 4      ! Order of matrix A (square matrix)
    nrhs = 2   ! Number of columns of b
    lda = n    ! Leading dimension of A
    ldb = n    ! Leading dimension of b
    info = 0

    ! Allocate matrices
    allocate(a(lda, n))
    allocate(b(ldb, nrhs))

    ! Initialize Matrix A (4x4): column-major storage, positive definite
    ! A = 4   5  -3   7
    !     5  16   0  17  
    !    -3   0  15  -8
    !     7  17  -8  23
    a = reshape([4.0, 5.0, -3.0, 7.0, &
                 5.0, 16.0, 0.0, 17.0, &
                 -3.0, 0.0, 15.0, -8.0, &
                 7.0, 17.0, -8.0, 23.0], [lda, n])

    ! Initialize Matrix b (4x2): column-major storage
    ! b = -3  17
    !      9  55
    !     47  11
    !    -16  54
    b = reshape([-3.0, 9.0, 47.0, -16.0, &
                 17.0, 55.0, 11.0, 54.0], [ldb, nrhs])
    
    ! Print input matrices
    call print_smatrix(a, n, n, lda, 'Entry Matrix A')
    call print_smatrix(b, n, nrhs, ldb, 'Right Hand Side Matrix b')

    print *, "sposv Example Program Results"
    print *

    ! Solve linear system: A * x = b using Cholesky decomposition
    call sposv('L', n, nrhs, a, lda, b, ldb, info)

    ! Any errors?
    if (info < 0) then
        print *, "Illegal input argument."
    else if (info > 0) then
        print *, "Matrix A is not positive definite."
    end if

    if (info .ne. 0) then
        deallocate(a, b)
        stop info
    end if

    print *, "Solution x:"
    call print_smatrix(b, n, nrhs, ldb, 'x')

    ! Print Cholesky factor (lower triangular part of A is overwritten with L factor)
    call print_smatrix(a, n, n, lda, 'Cholesky factor L')

    ! Clean up
    deallocate(a, b)

end program usageexample
