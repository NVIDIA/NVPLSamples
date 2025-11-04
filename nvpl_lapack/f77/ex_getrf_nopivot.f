program usageexample
    implicit none
    real, allocatable :: a(:, :)
    integer, pointer :: ipiv => null()
    external sgetrf
    integer m, n
    integer lda
    integer info
    integer i, j
    
    m = 10
    n = 10
    lda = m
    info = 0
    
    allocate(a(lda, n))
    
    call random_number(a)
    do i = 1, m
        a(i, i) = a(i, i) + real(m) * 2.0
        do j = 1, n
            print *, i, j, a(i, j)
        end do
    end do

    print *, "lda = ", lda
    ! Pass null pointer to ipiv to use no-pivoting algorithm
    call sgetrf(m, n, a, lda, ipiv, info)
    print *, "info = ", info
    
    deallocate(a)
end program usageexample
