program usageexample
    implicit none
    real, allocatable :: a(:, :)
    real, allocatable :: w(:)
    real, allocatable :: work(:)
    external ssyev

    integer n, lda
    integer lwork, info
    integer i

    ! Problem dimensions
    n = 4      ! Order of matrix A (square matrix)
    lda = n    ! Leading dimension of A
    lwork = -1 ! Workspace query
    info = 0

    ! Allocate matrices
    allocate(a(lda, n))
    allocate(w(n))

    ! Initialize Matrix A (4x4): column-major storage, symmetric
    ! A = 13   3  -6  -5
    !      3  13  -6  -5  
    !     -6  -6  31   7
    !     -5  -5   7  23
    a = reshape([13.0, 3.0, -6.0, -5.0, &
                 3.0, 13.0, -6.0, -5.0, &
                 -6.0, -6.0, 31.0, 7.0, &
                 -5.0, -5.0, 7.0, 23.0], [lda, n])
    
    ! Print input matrices
    call print_smatrix(a, n, n, lda, 'Entry Matrix A')

    print *, "ssyev Example Program Results"
    print *

    ! Workspace query
    allocate(work(1))
    call ssyev('V', 'L', n, a, lda, w, work, lwork, info)
    lwork = int(work(1))
    deallocate(work)

    ! Allocate optimal workspace
    allocate(work(lwork))

    ! Compute eigenvalue decomposition (using lower triangular part of A)
    call ssyev('V', 'L', n, a, lda, w, work, lwork, info)

    ! Any errors?
    if (info < 0) then
        print *, "Illegal input argument."
    else if (info > 0) then
        print *, "The algorithm failed to converge."
    end if

    if (info .ne. 0) then
        deallocate(a, w, work)
        stop info
    end if

    call print_svector(w, n, 1, 'Eigenvalues')

    ! Print eigenvectors (A is overwritten with eigenvectors)
    call print_smatrix(a, n, n, lda, 'Eigenvectors')

    ! Clean up
    deallocate(a, w, work)

end program usageexample
