program usageexample
    implicit none
    double complex, allocatable :: a(:, :)
    double precision, allocatable :: w(:)
    double complex, allocatable :: work(:)
    double precision, allocatable :: rwork(:)
    external zheev

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
    a = reshape([dcmplx(13.0d0,0.0d0), dcmplx(3.0d0,0.0d0), dcmplx(-6.0d0,0.0d0), dcmplx(-5.0d0,0.0d0), &
                 dcmplx(3.0d0,0.0d0), dcmplx(13.0d0,0.0d0), dcmplx(-6.0d0,0.0d0), dcmplx(-5.0d0,0.0d0), &
                 dcmplx(-6.0d0,0.0d0), dcmplx(-6.0d0,0.0d0), dcmplx(31.0d0,0.0d0), dcmplx(7.0d0,0.0d0), &
                 dcmplx(-5.0d0,0.0d0), dcmplx(-5.0d0,0.0d0), dcmplx(7.0d0,0.0d0), dcmplx(23.0d0,0.0d0)], [lda, n])
    
    ! Print input matrices
    call print_zmatrix(a, n, n, lda, 'Entry Matrix A')

    print *, "zheev Example Program Results"
    print *

    ! Workspace query
    allocate(work(1))
    call zheev('V', 'L', n, a, lda, w, work, lwork, rwork, info)
    lwork = int(dble(work(1)))
    deallocate(work)

    ! Allocate optimal workspace
    allocate(work(lwork))

    ! Compute eigenvalue decomposition (using lower triangular part of A)
    call zheev('V', 'L', n, a, lda, w, work, lwork, rwork, info)

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

    call print_dvector(w, n, 1, 'Eigenvalues')

    ! Print eigenvectors (A is overwritten with eigenvectors)
    call print_zmatrix(a, n, n, lda, 'Eigenvectors')

    ! Clean up
    deallocate(a, w, work, rwork)

end program usageexample
