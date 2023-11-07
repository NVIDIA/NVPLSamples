program usageexample
    real, allocatable :: a(:, :)
    external spotrf
    external nvpl_lapack_set_num_threads
    integer*4, external :: nvpl_lapack_get_max_threads
    integer*4, external :: nvpl_lapack_set_num_threads_local

    character(1) uplo
    integer*4 nt
    integer n
    integer lda
    integer info
    uplo = 'u'
    n = 10
    lda = n
    info = 0
    allocate (a(lda, n))
    call random_number(a)
    do 10 i = 1, n
        a(i, i) = a(i, i) + real(n) * 2.0
        do 20 j = 1, n
            print *, i, j, a(i, j)
20      continue
10  continue

!   optionally set the number of threads
    nt = 4
    call nvpl_lapack_set_num_threads(nt)
    print *, "lda = ", lda
    call spotrf(uplo, n, a, lda, info)
    print *, "info = ", info
    deallocate(a)
end program usageexample
