program usageexample
    implicit none
    double precision, allocatable :: a(:, :)
    integer, allocatable :: ipiv(:)
    external dgetrf

    integer n
    integer lda
    integer info

    ! Problem dimensions
    n = 4      ! Order of matrix A (square matrix)
    lda = n    ! Leading dimension of A
    info = 0

    ! Allocate matrices
    allocate(a(lda, n))
    allocate(ipiv(n))

    ! Initialize Matrix A (4x4): column-major storage
    ! A = 2   1   1   0
    !     4   3   3   1  
    !     8   7   9   5
    !     6   7   9   8
    a = reshape([2.0d0, 4.0d0, 8.0d0, 6.0d0, &
                 1.0d0, 3.0d0, 7.0d0, 7.0d0, &
                 1.0d0, 3.0d0, 9.0d0, 9.0d0, &
                 0.0d0, 1.0d0, 5.0d0, 8.0d0], [lda, n])

    ! Print input matrix
    call print_dmatrix(a, n, n, lda, 'Entry Matrix A')

    print *, "dgetrf Example Program Results"

    ! Compute LU factorization: P * A = L * U
    call dgetrf(n, n, a, lda, ipiv, info)

    ! Any errors?
    if (info < 0) then
        print *, "Illegal input argument."
    else if (info > 0) then
        print *, "Matrix A doesn't have a full rank."
    end if

    if (info .ne. 0) then
        deallocate(a, ipiv)
        stop info
    end if

    ! Print LU factors (A is overwritten with LU factorization)
    call print_dmatrix(a, n, n, lda, 'LU factors')

    ! Print pivot indices
    call print_ivector(ipiv, n, 'Pivot indices')

    ! Clean up
    deallocate(a, ipiv)

end program usageexample
