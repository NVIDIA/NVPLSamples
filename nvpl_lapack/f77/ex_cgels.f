program usageexample
    implicit none
    complex, allocatable :: a(:, :)
    complex, allocatable :: b(:, :)
    complex, allocatable :: work(:)
    complex :: work_query(1)
    external cgels

    character(1) trans
    integer m, n, nrhs
    integer lda, ldb, lwork
    integer info
    
    ! Problem dimensions
    m = 5      ! Number of rows of A and b matrices
    n = 3      ! Number of columns of A
    nrhs = 2   ! Number of columns of b
    lda = m    ! Leading dimension of A
    ldb = m    ! Leading dimension of b
    trans = 'N'
    info = 0
    
    ! Allocate matrices
    allocate(a(lda, n))
    allocate(b(ldb, nrhs))
    
    ! Initialize Matrix A (5x3): column-major storage
    ! A = 1   1   1
    !     2   3   4  
    !     3   5   2
    !     4   2   5
    !     5   4   3
    a = reshape([cmplx(1.0,0.0), cmplx(2.0,0.0), cmplx(3.0,0.0), &
                 cmplx(4.0,0.0), cmplx(5.0,0.0), cmplx(1.0,0.0), &
                 cmplx(3.0,0.0), cmplx(5.0,0.0), cmplx(2.0,0.0), &
                 cmplx(4.0,0.0), cmplx(1.0,0.0), cmplx(4.0,0.0), &
                 cmplx(2.0,0.0), cmplx(5.0,0.0), cmplx(3.0,0.0)], [lda, n])

    ! Initialize Matrix b (5x2): column-major storage
    ! b = -10  -3
    !      12  14
    !      14  12
    !      16  16
    !      18  16
    b = reshape([cmplx(-10.0,0.0), cmplx(12.0,0.0), &
                 cmplx(14.0,0.0), cmplx(16.0,0.0), &
                 cmplx(18.0,0.0), cmplx(-3.0,0.0), &
                 cmplx(14.0,0.0), cmplx(12.0,0.0), &
                 cmplx(16.0,0.0), cmplx(16.0,0.0)], [ldb, nrhs])
    
    ! Print input matrices
    call print_cmatrix(a, m, n, lda, 'Entry Matrix A')
    call print_cmatrix(b, m, nrhs, ldb, 'Right Hand Side Matrix b')
    
    print *, "cgels Example Program Results"
    print *
    
    ! Workspace query
    lwork = -1
    call cgels(trans, m, n, nrhs, a, lda, b, ldb, work_query, lwork, info)
    lwork = int(real(work_query(1)))
    
    print *, "Optimal workspace size: ", lwork
    
    ! Allocate workspace
    allocate(work(lwork))
    
    ! Solve least squares problem: min_x || A * x - b ||
    call cgels(trans, m, n, nrhs, a, lda, b, ldb, work, lwork, info)
    
    ! Any errors?
    if (info < 0) then
        print *, "Illegal input argument."
    else if (info > 0) then
        print *, "Matrix A doesn't have a full rank."
    end if
    
    if (info .ne. 0) then
        deallocate(a, b, work)
        stop info
    end if
    
    print *, "Solution (first n rows of b):"
    call print_cmatrix(b, n, nrhs, ldb, 'x')
    
    ! Clean up
    deallocate(a, b, work)
    
end program usageexample
