program usageexample
    implicit none
    double complex, allocatable :: a(:, :)
    double complex, allocatable :: b(:, :)
    external zposv

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
    a = reshape([dcmplx(4.0d0,0.0d0), dcmplx(5.0d0,0.0d0), dcmplx(-3.0d0,0.0d0), dcmplx(7.0d0,0.0d0), &
                 dcmplx(5.0d0,0.0d0), dcmplx(16.0d0,0.0d0), dcmplx(0.0d0,0.0d0), dcmplx(17.0d0,0.0d0), &
                 dcmplx(-3.0d0,0.0d0), dcmplx(0.0d0,0.0d0), dcmplx(15.0d0,0.0d0), dcmplx(-8.0d0,0.0d0), &
                 dcmplx(7.0d0,0.0d0), dcmplx(17.0d0,0.0d0), dcmplx(-8.0d0,0.0d0), dcmplx(23.0d0,0.0d0)], [lda, n])

    ! Initialize Matrix b (4x2): column-major storage
    ! b = -3  17
    !      9  55
    !     47  11
    !    -16  54
    b = reshape([dcmplx(-3.0d0,0.0d0), dcmplx(9.0d0,0.0d0), dcmplx(47.0d0,0.0d0), dcmplx(-16.0d0,0.0d0), &
                 dcmplx(17.0d0,0.0d0), dcmplx(55.0d0,0.0d0), dcmplx(11.0d0,0.0d0), dcmplx(54.0d0,0.0d0)], [ldb, nrhs])
    
    ! Print input matrices
    call print_zmatrix(a, n, n, lda, 'Entry Matrix A')
    call print_zmatrix(b, n, nrhs, ldb, 'Right Hand Side Matrix b')

    print *, "zposv Example Program Results"
    print *

    ! Solve linear system: A * x = b using Cholesky decomposition
    call zposv('L', n, nrhs, a, lda, b, ldb, info)

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
    call print_zmatrix(b, n, nrhs, ldb, 'x')

    ! Print Cholesky factor (lower triangular part of A is overwritten with L factor)
    call print_zmatrix(a, n, n, lda, 'Cholesky factor L')

    ! Clean up
    deallocate(a, b)

end program usageexample
