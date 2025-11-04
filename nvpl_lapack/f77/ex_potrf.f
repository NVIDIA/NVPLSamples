program usageexample
    implicit none
    real, allocatable :: a(:, :)
    external spotrf
    external nvpl_lapack_set_num_threads

    character(1) uplo
    integer*4 nt
    integer n
    integer lda
    integer info
    integer i, j
    
    uplo = 'U'
    n = 10
    lda = n
    info = 0
    
    allocate(a(lda, n))
    
    call random_number(a)
    do i = 1, n
        a(i, i) = a(i, i) + real(n) * 2.0
        do j = 1, n
            print *, i, j, a(i, j)
        end do
    end do

    ! optionally set the number of threads
    nt = 4
    call nvpl_lapack_set_num_threads(nt)
    print *, "lda = ", lda
    ! Perform Cholesky factorization: A = U^T * U (using upper triangular storage)
    call spotrf(uplo, n, a, lda, info)
    print *, "info = ", info
    
    ! Note: Only the upper triangular part of A now contains the Cholesky factor U
    
    deallocate(a)
end program usageexample

