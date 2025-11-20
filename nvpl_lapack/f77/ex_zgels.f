program usageexample
    implicit none
    double complex, allocatable :: a(:, :)
    double complex, allocatable :: b(:, :)
    double complex, allocatable :: work(:)
    double complex :: work_query(1)
    external zgels

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
    a = reshape([dcmplx(1.0d0,0.0d0), dcmplx(2.0d0,0.0d0), dcmplx(3.0d0,0.0d0), &
                 dcmplx(4.0d0,0.0d0), dcmplx(5.0d0,0.0d0), dcmplx(1.0d0,0.0d0), &
                 dcmplx(3.0d0,0.0d0), dcmplx(5.0d0,0.0d0), dcmplx(2.0d0,0.0d0), &
                 dcmplx(4.0d0,0.0d0), dcmplx(1.0d0,0.0d0), dcmplx(4.0d0,0.0d0), &
                 dcmplx(2.0d0,0.0d0), dcmplx(5.0d0,0.0d0), dcmplx(3.0d0,0.0d0)], [lda, n])

    ! Initialize Matrix b (5x2): column-major storage
    ! b = -10  -3
    !      12  14
    !      14  12
    !      16  16
    !      18  16
    b = reshape([dcmplx(-10.0d0,0.0d0), dcmplx(12.0d0,0.0d0), &
                 dcmplx(14.0d0,0.0d0), dcmplx(16.0d0,0.0d0), &
                 dcmplx(18.0d0,0.0d0), dcmplx(-3.0d0,0.0d0), &
                 dcmplx(14.0d0,0.0d0), dcmplx(12.0d0,0.0d0), &
                 dcmplx(16.0d0,0.0d0), dcmplx(16.0d0,0.0d0)], [ldb, nrhs])
    
    ! Print input matrices
    call print_zmatrix(a, m, n, lda, 'Entry Matrix A')
    call print_zmatrix(b, m, nrhs, ldb, 'Right Hand Side Matrix b')
    
    print *, "zgels Example Program Results"
    print *
    
    ! Workspace query
    lwork = -1
    call zgels(trans, m, n, nrhs, a, lda, b, ldb, work_query, lwork, info)
    lwork = int(real(work_query(1)))
    
    print *, "Optimal workspace size: ", lwork
    
    ! Allocate workspace
    allocate(work(lwork))
    
    ! Solve least squares problem: min_x || A * x - b ||
    call zgels(trans, m, n, nrhs, a, lda, b, ldb, work, lwork, info)
    
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
    call print_zmatrix(b, n, nrhs, ldb, 'x')
    
    ! Clean up
    deallocate(a, b, work)
    
end program usageexample
