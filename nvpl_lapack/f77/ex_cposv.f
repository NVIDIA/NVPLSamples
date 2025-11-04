program usageexample
    implicit none
    complex, allocatable :: a(:, :)
    complex, allocatable :: b(:, :)
    external cposv

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
    a = reshape([cmplx(4.0,0.0), cmplx(5.0,0.0), cmplx(-3.0,0.0), cmplx(7.0,0.0), &
                 cmplx(5.0,0.0), cmplx(16.0,0.0), cmplx(0.0,0.0), cmplx(17.0,0.0), &
                 cmplx(-3.0,0.0), cmplx(0.0,0.0), cmplx(15.0,0.0), cmplx(-8.0,0.0), &
                 cmplx(7.0,0.0), cmplx(17.0,0.0), cmplx(-8.0,0.0), cmplx(23.0,0.0)], [lda, n])

    ! Initialize Matrix b (4x2): column-major storage
    ! b = -3  17
    !      9  55
    !     47  11
    !    -16  54
    b = reshape([cmplx(-3.0,0.0), cmplx(9.0,0.0), cmplx(47.0,0.0), cmplx(-16.0,0.0), &
                 cmplx(17.0,0.0), cmplx(55.0,0.0), cmplx(11.0,0.0), cmplx(54.0,0.0)], [ldb, nrhs])
    
    ! Print input matrices
    call print_cmatrix(a, n, n, lda, 'Entry Matrix A')
    call print_cmatrix(b, n, nrhs, ldb, 'Right Hand Side Matrix b')

    print *, "cposv Example Program Results"
    print *

    ! Solve linear system: A * x = b using Cholesky decomposition
    call cposv('L', n, nrhs, a, lda, b, ldb, info)

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
    call print_cmatrix(b, n, nrhs, ldb, 'x')

    ! Print Cholesky factor (lower triangular part of A is overwritten with L factor)
    call print_cmatrix(a, n, n, lda, 'Cholesky factor L')

    ! Clean up
    deallocate(a, b)

end program usageexample
