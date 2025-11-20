program usageexample
    implicit none
    real, allocatable :: a(:, :)
    real, allocatable :: b(:, :)
    integer, allocatable :: ipiv(:)
    external sgesv

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
    allocate(ipiv(n))

    ! Initialize Matrix A (4x4): column-major storage
    ! A = 2   1   1   0
    !     4   3   3   1  
    !     8   7   9   5
    !     6   7   9   8
    a = reshape([2.0, 4.0, 8.0, 6.0, &
                 1.0, 3.0, 7.0, 7.0, &
                 1.0, 3.0, 9.0, 9.0, &
                 0.0, 1.0, 5.0, 8.0], [lda, n])

    ! Initialize Matrix b (4x2): column-major storage
    ! b = 2   1
    !     7   6
    !     18  15
    !     22  19
    b = reshape([2.0, 7.0, 18.0, 22.0, &
                 1.0, 6.0, 15.0, 19.0], [ldb, nrhs])
    
    ! Print input matrices
    call print_smatrix(a, n, n, lda, 'Entry Matrix A')
    call print_smatrix(b, n, nrhs, ldb, 'Right Hand Side Matrix b')

    print *, "sgesv Example Program Results"
    print *

    ! Solve linear system: A * x = b
    call sgesv(n, nrhs, a, lda, ipiv, b, ldb, info)

    ! Any errors?
    if (info < 0) then
        print *, "Illegal input argument."
    else if (info > 0) then
        print *, "Matrix A doesn't have a full rank."
    end if

    if (info .ne. 0) then
        deallocate(a, b, ipiv)
        stop info
    end if

    print *, "Solution x:"
    call print_smatrix(b, n, nrhs, ldb, 'x')

    ! Print LU factors (A is overwritten with LU factorization)
    call print_smatrix(a, n, n, lda, 'LU factors')

    ! Print pivot indices
    call print_ivector(ipiv, n, 'Pivot indices')

    ! Clean up
    deallocate(a, b, ipiv)

end program usageexample
