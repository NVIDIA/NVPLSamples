program usageexample
    implicit none
    complex, allocatable :: a(:, :)
    real, allocatable :: w(:)
    complex, allocatable :: work(:)
    real, allocatable :: rwork(:)
    external cheev

    integer n, lda
    integer lwork, lrwork, info
    integer i

    ! Problem dimensions
    n = 4        ! Order of matrix A (square matrix)
    lda = n      ! Leading dimension of A
    lwork = -1   ! Workspace query
    lrwork = 3*n-2 ! Real workspace size
    info = 0

    ! Allocate matrices
    allocate(a(lda, n))
    allocate(w(n))
    allocate(rwork(lrwork))

    ! Initialize Matrix A (4x4): column-major storage, Hermitian
    ! A = 13   3  -6  -5
    !      3  13  -6  -5  
    !     -6  -6  31   7
    !     -5  -5   7  23
    a = reshape([cmplx(13.0,0.0), cmplx(3.0,0.0), cmplx(-6.0,0.0), cmplx(-5.0,0.0), &
                 cmplx(3.0,0.0), cmplx(13.0,0.0), cmplx(-6.0,0.0), cmplx(-5.0,0.0), &
                 cmplx(-6.0,0.0), cmplx(-6.0,0.0), cmplx(31.0,0.0), cmplx(7.0,0.0), &
                 cmplx(-5.0,0.0), cmplx(-5.0,0.0), cmplx(7.0,0.0), cmplx(23.0,0.0)], [lda, n])
    
    ! Print input matrices
    call print_cmatrix(a, n, n, lda, 'Entry Matrix A')

    print *, "cheev Example Program Results"
    print *

    ! Workspace query
    allocate(work(1))
    call cheev('V', 'L', n, a, lda, w, work, lwork, rwork, info)
    lwork = int(real(work(1)))
    deallocate(work)

    ! Allocate optimal workspace
    allocate(work(lwork))

    ! Compute eigenvalue decomposition (using lower triangular part of A)
    call cheev('V', 'L', n, a, lda, w, work, lwork, rwork, info)

    ! Any errors?
    if (info < 0) then
        print *, "Illegal input argument."
    else if (info > 0) then
        print *, "The algorithm failed to converge."
    end if

    if (info .ne. 0) then
        deallocate(a, w, work, rwork)
        stop info
    end if

    call print_svector(w, n, 1, 'Eigenvalues')

    ! Print eigenvectors (A is overwritten with eigenvectors)
    call print_cmatrix(a, n, n, lda, 'Eigenvectors')

    ! Clean up
    deallocate(a, w, work, rwork)

end program usageexample
