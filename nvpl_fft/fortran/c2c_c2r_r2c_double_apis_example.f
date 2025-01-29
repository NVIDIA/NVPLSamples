        program main

        implicit none

        logical :: success
        logical, external :: run_version_api_test
        logical, external :: run_1d_api_test
        logical, external :: run_2d_api_test
        logical, external :: run_3d_api_test
        logical, external :: run_many_api_test
        logical, external :: run_thread_api_test

        success = .true.

        success = run_version_api_test()
        print *, "run_version_api_test success: "
        print *, success
        if (success .eqv. .false.) then
            call exit(1)
        end if

        success = run_1d_api_test()
        print *, "run_1d_api_test success: "
        print *, success
        if (success .eqv. .false.) then
            call exit(1)
        end if

        success = run_2d_api_test()
        print *, "run_2d_api_test success: "
        print *, success
        if (success .eqv. .false.) then
            call exit(1)
        end if

        success = run_3d_api_test()
        print *, "run_3d_api_test success: "
        print *, success
        if (success .eqv. .false.) then
            call exit(1)
        end if

        success = run_many_api_test()
        print *, "run_many_api_test success: "
        print *, success
        if (success .eqv. .false.) then
            call exit(1)
        end if

        success = run_thread_api_test()
        print *, "run_thread_api_test success: "
        print *, success
        if (success .eqv. .false.) then
            call exit(1)
        end if

        call exit(0)
        end program

        logical function run_version_api_test() result(success)
        use, intrinsic :: iso_c_binding

        implicit none

        include 'fftw3.f'

        integer(C_INT) :: version
    
        success = .true.
    
        call nvpl_fft_get_version(version)
    
        ! print *, "nvpl fftw version: "
        ! print *, version
        end function run_version_api_test

        logical function run_1d_api_test() result(success)
        use, intrinsic :: iso_c_binding

        implicit none

        include 'fftw3.f'

        logical, external :: validate_complex
        logical, external :: validate_real

        integer(C_INT) :: nx

        integer(C_INT), dimension(1) :: c2c_count
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c(:)
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_tmp(:)
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_c2c_solution(:)
        type(C_PTR) :: plan_c2c_forward, plan_c2c_backward

        integer(C_INT), dimension(1) :: r2c_count
        real(C_DOUBLE), allocatable :: r2c(:)
        integer(C_INT), dimension(1) :: c2r_count
        complex(C_DOUBLE_COMPLEX), allocatable :: c2r(:)
        real(C_DOUBLE), allocatable :: r2c_c2r_solution(:)
        type(C_PTR) :: plan_r2c, plan_c2r

        success = .true.

        nx = 8

        c2c_count = (/ (nx) /)
        r2c_count = (/ (nx) /)
        c2r_count = (/ (nx/2 + 1) /)

        allocate(c2c(product(c2c_count)))
        allocate(c2c_tmp(product(c2c_count)))
        allocate(c2c_c2c_solution(product(c2c_count)))

        allocate(r2c(product(r2c_count)))
        allocate(c2r(product(c2r_count)))
        allocate(r2c_c2r_solution(product(r2c_count)))

        call initialize_complex(c2c, product(c2c_count))
        call initialize_complex(c2c_c2c_solution, product(c2c_count))

        call initialize_real(r2c, product(r2c_count))
        call initialize_real(r2c_c2r_solution, product(r2c_count))
        
        call dfftw_plan_dft_1d(plan_c2c_forward,
     &                         c2c_count(1),
     &                         c2c, c2c_tmp,
     &                         FFTW_FORWARD, FFTW_MEASURE)
        call dfftw_plan_dft_1d(plan_c2c_backward,
     &                         c2c_count(1),
     &                         c2c_tmp, c2c,
     &                         FFTW_BACKWARD, FFTW_MEASURE)

        call dfftw_execute_dft(plan_c2c_forward, c2c, c2c_tmp)
        call dfftw_execute_dft(plan_c2c_backward, c2c_tmp, c2c)

        call normalize_complex(c2c, product(c2c_count))

        call dfftw_destroy_plan(plan_c2c_forward)
        call dfftw_destroy_plan(plan_c2c_backward)

        call dfftw_plan_dft_r2c_1d(plan_r2c,
     &                             r2c_count(1),
     &                             r2c, c2r,
     &                             FFTW_MEASURE)
        call dfftw_plan_dft_c2r_1d(plan_c2r,
     &                            r2c_count(1),
     &                            c2r, r2c,
     &                            FFTW_MEASURE)

        call dfftw_execute_dft_r2c(plan_r2c, r2c, c2r)
        call dfftw_execute_dft_c2r(plan_c2r, c2r, r2c)

        call dfftw_destroy_plan(plan_r2c)
        call dfftw_destroy_plan(plan_c2r)

        call dfftw_cleanup()

        call normalize_real(r2c, product(r2c_count))
        
        ! print *, "c2c: "
        ! call print_complex(c2c, product(c2c_count));
        ! call print_complex(c2c_c2c_solution, product(c2c_count));

        ! print *, "r2c: "
        ! call print_real(r2c, product(r2c_count));
        ! call print_real(r2c_c2r_solution, product(r2c_count));

        if (success .eqv. .true.) then
            success = validate_complex(c2c, c2c_c2c_solution,
     &                                 product(c2c_count),
     &                                 1.0d-5)
        end if

        if (success .eqv. .true.) then
            success = validate_real(r2c, r2c_c2r_solution,
     &                              product(r2c_count),
     &                              1.0d-5)
        end if

        deallocate(r2c_c2r_solution)
        deallocate(c2r)
        deallocate(r2c)

        deallocate(c2c_c2c_solution)
        deallocate(c2c_tmp)
        deallocate(c2c)
        end function run_1d_api_test

        logical function run_2d_api_test() result(success)
        use, intrinsic :: iso_c_binding

        implicit none

        include 'fftw3.f'

        logical, external :: validate_complex
        logical, external :: validate_real

        integer(C_INT) :: nx, ny

        integer(C_INT), dimension(2) :: c2c_count
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c(:)
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_tmp(:)
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_c2c_solution(:)
        type(C_PTR) :: plan_c2c_forward, plan_c2c_backward

        integer(C_INT), dimension(2) :: r2c_count
        real(C_DOUBLE), allocatable :: r2c(:)
        integer(C_INT), dimension(2) :: c2r_count
        complex(C_DOUBLE_COMPLEX), allocatable :: c2r(:)
        real(C_DOUBLE), allocatable :: r2c_c2r_solution(:)
        type(C_PTR) :: plan_r2c, plan_c2r

        success = .true.

        nx = 8
        ny = 4

        c2c_count = (/ ny, nx /)
        r2c_count = (/ ny, nx /)
        c2r_count = (/ ny/2 + 1, nx/)

        allocate(c2c(product(c2c_count)))
        allocate(c2c_tmp(product(c2c_count)))
        allocate(c2c_c2c_solution(product(c2c_count)))

        allocate(r2c(product(r2c_count)))
        allocate(c2r(product(c2r_count)))
        allocate(r2c_c2r_solution(product(r2c_count)))

        call initialize_complex(c2c, product(c2c_count))
        call initialize_complex(c2c_c2c_solution, product(c2c_count))

        call initialize_real(r2c, product(r2c_count))
        call initialize_real(r2c_c2r_solution, product(r2c_count))

        call dfftw_plan_dft_2d(plan_c2c_forward,
     &                         c2c_count(1), c2c_count(2),
     &                         c2c, c2c_tmp,
     &                         FFTW_FORWARD, FFTW_MEASURE)
        call dfftw_plan_dft_2d(plan_c2c_backward,
     &                         c2c_count(1), c2c_count(2),
     &                         c2c_tmp, c2c,
     &                         FFTW_BACKWARD, FFTW_MEASURE)

        call dfftw_execute_dft(plan_c2c_forward, c2c, c2c_tmp)
        call dfftw_execute_dft(plan_c2c_backward, c2c_tmp, c2c)

        call dfftw_destroy_plan(plan_c2c_forward)
        call dfftw_destroy_plan(plan_c2c_backward)

        call normalize_complex(c2c, product(c2c_count))

        call dfftw_plan_dft_r2c_2d(plan_r2c,
     &                             r2c_count(1), r2c_count(2),
     &                             r2c, c2r,
     &                             FFTW_MEASURE)
        call dfftw_plan_dft_c2r_2d(plan_c2r,
     &                             r2c_count(1), r2c_count(2),
     &                             c2r, r2c,
     &                             FFTW_MEASURE)

        call dfftw_execute_dft_r2c(plan_r2c, r2c, c2r)
        call dfftw_execute_dft_c2r(plan_c2r, c2r, r2c)

        call dfftw_destroy_plan(plan_r2c)
        call dfftw_destroy_plan(plan_c2r)

        call dfftw_cleanup()

        call normalize_real(r2c, product(r2c_count))

        ! print *, "c2c: "
        ! call print_complex(c2c, product(c2c_count));
        ! call print_complex(c2c_c2c_solution, product(c2c_count));

        ! print *, "r2c: "
        ! call print_real(r2c, product(r2c_count));
        ! call print_real(r2c_c2r_solution, product(r2c_count));

        if (success .eqv. .true.) then
            success = validate_complex(c2c, c2c_c2c_solution,
     &                                 product(c2c_count),
     &                                 1.0d-5)
        end if

        if (success .eqv. .true.) then
            success = validate_real(r2c, r2c_c2r_solution,
     &                              product(r2c_count),
     &                              1.0d-5)
        end if

        deallocate(r2c_c2r_solution)
        deallocate(c2r)
        deallocate(r2c)

        deallocate(c2c_c2c_solution)
        deallocate(c2c_tmp)
        deallocate(c2c)
        end function run_2d_api_test

        logical function run_3d_api_test() result(success)
        use, intrinsic :: iso_c_binding

        implicit none

        include 'fftw3.f'

        logical, external :: validate_complex
        logical, external :: validate_real
        external :: initialize_complex
        external :: initialize_real

        integer(C_INT) :: nx, ny, nz

        integer(C_INT), dimension(3) :: c2c_count
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c(:)
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_tmp(:)
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_c2c_solution(:)
        type(C_PTR) :: plan_c2c_forward, plan_c2c_backward

        integer(C_INT), dimension(3) :: r2c_count
        real(C_DOUBLE), allocatable :: r2c(:)
        integer(C_INT), dimension(3) :: c2r_count
        complex(C_DOUBLE_COMPLEX), allocatable :: c2r(:)
        real(C_DOUBLE), allocatable :: r2c_c2r_solution(:)
        type(C_PTR) :: plan_r2c, plan_c2r

        success = .true.

        nx = 8
        ny = 4
        nz = 2

        c2c_count = (/ nz, ny, nx /)
        r2c_count = (/ nz, ny, nx /)
        c2r_count = (/ nz/2 + 1, ny, nx /)

        allocate(c2c(product(c2c_count)))
        allocate(c2c_tmp(product(c2c_count)))
        allocate(c2c_c2c_solution(product(c2c_count)))

        allocate(r2c(product(r2c_count)))
        allocate(c2r(product(c2r_count)))
        allocate(r2c_c2r_solution(product(r2c_count)))

        call initialize_complex(c2c, product(c2c_count))
        call initialize_complex(c2c_c2c_solution, product(c2c_count))

        call initialize_real(r2c, product(r2c_count))
        call initialize_real(r2c_c2r_solution, product(r2c_count))

        call dfftw_plan_dft_3d(plan_c2c_forward,
     &                         c2c_count(1), c2c_count(2), c2c_count(3),
     &                         c2c, c2c_tmp,
     &                         FFTW_FORWARD, FFTW_MEASURE)
        call dfftw_plan_dft_3d(plan_c2c_backward,
     &                        c2c_count(1), c2c_count(2), c2c_count(3),
     &                        c2c_tmp, c2c,
     &                        FFTW_BACKWARD, FFTW_MEASURE)

        call dfftw_execute_dft(plan_c2c_forward, c2c, c2c_tmp)
        call dfftw_execute_dft(plan_c2c_backward, c2c_tmp, c2c)

        call dfftw_destroy_plan(plan_c2c_forward)
        call dfftw_destroy_plan(plan_c2c_backward)

        call normalize_complex(c2c, product(c2c_count))

        call dfftw_plan_dft_r2c_3d(plan_r2c,
     &                         r2c_count(1), r2c_count(2), r2c_count(3),
     &                         r2c, c2r,
     &                         FFTW_MEASURE)
        call dfftw_plan_dft_c2r_3d(plan_c2r,
     &                         r2c_count(1), r2c_count(2), r2c_count(3),
     &                         c2r, r2c,
     &                         FFTW_MEASURE)

        call dfftw_execute_dft_r2c(plan_r2c, r2c, c2r)
        call dfftw_execute_dft_c2r(plan_c2r, c2r, r2c)

        call dfftw_destroy_plan(plan_r2c)
        call dfftw_destroy_plan(plan_c2r)

        call dfftw_cleanup()

        call normalize_real(r2c, product(r2c_count))

        ! print *, "c2c: "
        ! call print_complex(c2c, product(c2c_count));
        ! call print_complex(c2c_c2c_solution, product(c2c_count));

        ! print *, "r2c: "
        ! call print_real(r2c, product(r2c_count));
        ! call print_real(r2c_c2r_solution, product(r2c_count));

        if (success .eqv. .true.) then
            success = validate_complex(c2c, c2c_c2c_solution,
     &                                 product(c2c_count),
     &                                 1.0d-5)
        end if

        if (success .eqv. .true.) then
            success = validate_real(r2c, r2c_c2r_solution,
     &                              product(r2c_count),
     &                              1.0d-5)
        end if
        
        deallocate(r2c_c2r_solution)
        deallocate(c2r)
        deallocate(r2c)

        deallocate(c2c_c2c_solution)
        deallocate(c2c_tmp)
        deallocate(c2c)
        end function run_3d_api_test

        logical function run_many_api_test() result(success)
        use, intrinsic :: iso_c_binding

        implicit none

        include 'fftw3.f'

        logical, external :: validate_complex
        logical, external :: validate_real
        external :: initialize_complex
        external :: initialize_real

        integer(C_INT) :: nx, ny, nz

        integer(C_INT) :: howmany

        integer(C_INT), dimension(3) :: c2c_count
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c(:)
        integer(C_INT), dimension(3) :: c2c_embed
        integer(C_INT) :: c2c_stride
        integer(C_INT) :: c2c_dist
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_tmp(:)
        integer(C_INT), dimension(3) :: c2c_tmp_embed
        integer(C_INT) :: c2c_tmp_stride
        integer(C_INT) :: c2c_tmp_dist
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_c2c_solution(:)
        type(C_PTR) :: plan_c2c_forward, plan_c2c_backward

        integer(C_INT), dimension(3) :: r2c_count
        real(C_DOUBLE), allocatable :: r2c(:)
        integer(C_INT), dimension(3) :: r2c_embed
        integer(C_INT) :: r2c_stride
        integer(C_INT) :: r2c_dist
        integer(C_INT), dimension(3) :: c2r_count
        complex(C_DOUBLE_COMPLEX), allocatable :: c2r(:)
        integer(C_INT), dimension(3) :: c2r_embed
        integer(C_INT) :: c2r_stride
        integer(C_INT) :: c2r_dist
        real(C_DOUBLE), allocatable :: r2c_c2r_solution(:)
        type(C_PTR) :: plan_r2c, plan_c2r

        success = .true.

        nx = 8
        ny = 4
        nz = 2

        c2c_count = (/ nz, ny, nx /)
        r2c_count = (/ nz, ny, nx /)
        c2r_count = (/ nz/2 + 1, ny, nx /)

        howmany = 1

        c2c_embed = c2c_count
        c2c_stride = 1
        c2c_dist = product(c2c_count)

        c2c_tmp_embed = c2c_count
        c2c_tmp_stride = 1
        c2c_tmp_dist = product(c2c_count)

        r2c_embed = r2c_count
        r2c_stride = 1
        r2c_dist = product(r2c_count)

        c2r_embed = c2r_count
        c2r_stride = 1
        c2r_dist = product(c2r_count)

        allocate(c2c(howmany * product(c2c_count)))
        allocate(c2c_tmp(howmany * product(c2c_count)))
        allocate(c2c_c2c_solution(howmany * product(c2c_count)))

        allocate(r2c(howmany * product(r2c_count)))
        allocate(c2r(howmany * product(c2r_count)))
        allocate(r2c_c2r_solution(howmany * product(r2c_count)))

        call initialize_complex(c2c, howmany * product(c2c_count))
        call initialize_complex(c2c_c2c_solution,
     &                          howmany * product(c2c_count))

        call initialize_real(r2c, howmany * product(r2c_count))
        call initialize_real(r2c_c2r_solution,
     &                       howmany * product(r2c_count))

        call dfftw_plan_many_dft(plan_c2c_forward,
     &                     size(c2c_count), c2c_count, howmany,
     &                     c2c,
     &                     c2c_embed,     c2c_stride,     c2c_dist,
     &                     c2c_tmp,
     &                     c2c_tmp_embed, c2c_tmp_stride, c2c_tmp_dist,
     &                     FFTW_FORWARD, FFTW_MEASURE)
        call dfftw_plan_many_dft(plan_c2c_backward,
     &                     size(c2c_count), c2c_count, howmany,
     &                     c2c_tmp,
     &                     c2c_tmp_embed, c2c_tmp_stride, c2c_tmp_dist,
     &                     c2c,
     &                     c2c_embed,     c2c_stride,     c2c_dist,
     &                     FFTW_BACKWARD, FFTW_MEASURE)

        call dfftw_execute_dft(plan_c2c_forward, c2c, c2c_tmp)
        call dfftw_execute_dft(plan_c2c_backward, c2c_tmp, c2c)

        call dfftw_destroy_plan(plan_c2c_forward)
        call dfftw_destroy_plan(plan_c2c_backward)

        call normalize_complex(c2c, product(c2c_count))

        call dfftw_plan_many_dft_r2c(plan_r2c,
     &                             size(r2c_count), r2c_count, howmany,
     &                             r2c, r2c_embed, r2c_stride, r2c_dist,
     &                             c2r, c2r_embed, c2r_stride, c2r_dist,
     &                             FFTW_MEASURE)
        call dfftw_plan_many_dft_c2r(plan_c2r,
     &                             size(r2c_count), r2c_count, howmany,
     &                             c2r, c2r_embed, c2r_stride, c2r_dist,
     &                             r2c, r2c_embed, r2c_stride, r2c_dist,
     &                             FFTW_MEASURE)

        call dfftw_execute_dft_r2c(plan_r2c, r2c, c2r)
        call dfftw_execute_dft_c2r(plan_c2r, c2r, r2c)

        call dfftw_destroy_plan(plan_r2c)
        call dfftw_destroy_plan(plan_c2r)

        call dfftw_cleanup()

        call normalize_real(r2c, product(r2c_count))

        ! print *, "c2c: "
        ! call print_complex(c2c, product(c2c_count));
        ! call print_complex(c2c_c2c_solution, product(c2c_count));

        ! print *, "r2c: "
        ! call print_real(r2c, product(r2c_count));
        ! call print_real(r2c_c2r_solution, product(r2c_count));

        if (success .eqv. .true.) then
            success = validate_complex(c2c, c2c_c2c_solution,
     &                                 product(c2c_count),
     &                                 1.0d-5)
        end if

        if (success .eqv. .true.) then
            success = validate_real(r2c, r2c_c2r_solution,
     &                              product(r2c_count),
     &                              1.0d-5)
        end if
        
        deallocate(r2c_c2r_solution)
        deallocate(c2r)
        deallocate(r2c)

        deallocate(c2c_c2c_solution)
        deallocate(c2c_tmp)
        deallocate(c2c)
        end function run_many_api_test

        logical function run_thread_api_test() result(success)
        use, intrinsic :: iso_c_binding

        implicit none
    
        include 'fftw3.f'
    
        logical, external :: validate_complex
        logical, external :: validate_real
        external :: initialize_complex
        external :: initialize_real
    
        integer(C_INT) :: nthread
        integer(C_INT) :: nx
    
        integer(C_INT), dimension(1) :: c2c_count
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c(:)
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_tmp(:)
        complex(C_DOUBLE_COMPLEX), allocatable :: c2c_c2c_solution(:)
        type(C_PTR) :: plan_c2c_forward, plan_c2c_backward
    
        integer(C_INT) :: initialized
    
        success = .true.

        nthread = 4

        nx = 128
    
        c2c_count = (/ nx /)
        
        allocate(c2c(product(c2c_count)))
        allocate(c2c_tmp(product(c2c_count)))
        allocate(c2c_c2c_solution(product(c2c_count)))
    
        call initialize_complex(c2c, product(c2c_count))
        call initialize_complex(c2c_c2c_solution, product(c2c_count))
    
        call dfftw_init_threads(initialized);
        call dfftw_plan_with_nthreads(nthread);

        call dfftw_plan_dft_1d(plan_c2c_forward,
     &                         c2c_count(1),
     &                         c2c, c2c_tmp,
     &                         FFTW_FORWARD, FFTW_MEASURE)
    
        call dfftw_plan_dft_1d(plan_c2c_backward,
     &                        c2c_count(1),
     &                        c2c_tmp, c2c,
     &                        FFTW_BACKWARD, FFTW_MEASURE)
    
        call dfftw_execute_dft(plan_c2c_forward, c2c, c2c_tmp)
        call dfftw_execute_dft(plan_c2c_backward, c2c_tmp, c2c)
    
        call dfftw_destroy_plan(plan_c2c_forward)
        call dfftw_destroy_plan(plan_c2c_backward)
    
        call dfftw_cleanup_threads()
    
        call dfftw_cleanup()
    
        call normalize_complex(c2c, product(c2c_count))
    
        ! print *, "c2c: "
        ! call print_complex(c2c, product(c2c_count));
        ! call print_complex(c2c_c2c_solution, product(c2c_count));

        if (success .eqv. .true.) then
            success = validate_complex(c2c, c2c_c2c_solution,
     &                                 product(c2c_count),
     &                                 1.0d-5)
        end if
    
        deallocate(c2c_c2c_solution)
        deallocate(c2c_tmp)
        deallocate(c2c)
        end function run_thread_api_test

        subroutine initialize_complex(ary, count)
        use, intrinsic :: iso_c_binding

        implicit none

        complex(C_DOUBLE_COMPLEX), dimension(*), intent(out) :: ary
        integer(C_INT) :: count

        integer(C_INT) :: n

        do n = 1, count
         ary(n) = cmplx(n,0)
        end do
        end subroutine initialize_complex

        subroutine initialize_real(ary, count)
        use, intrinsic :: iso_c_binding

        implicit none

        real(C_DOUBLE), dimension(*), intent(out) :: ary
        integer(C_INT) :: count

        integer(C_INT) :: n

        do n = 1, count
         ary(n) = n
        end do
        end subroutine initialize_real

        subroutine normalize_complex(ary, count)
        use, intrinsic :: iso_c_binding

        implicit none

        complex(C_DOUBLE_COMPLEX), dimension(*), intent(out) :: ary
        integer(C_INT) :: count

        integer(C_INT) :: n

        do n = 1, count
         ary(n) = ary(n) / count
        end do
        end subroutine normalize_complex

        subroutine normalize_real(ary, count)
        use, intrinsic :: iso_c_binding

        implicit none

        real(C_DOUBLE), dimension(*), intent(out) :: ary
        integer(C_INT) :: count

        integer(C_INT) :: n

        do n = 1, count
         ary(n) = ary(n) / count
        end do
        end subroutine normalize_real

        logical function validate_complex(ary,solution,count,threshold)
     &  result(success)
        use, intrinsic :: iso_c_binding

        implicit none

        complex(C_DOUBLE_COMPLEX), dimension(*), intent(in) :: ary
        complex(C_DOUBLE_COMPLEX), dimension(*), intent(in) :: solution
        integer(C_INT) :: count
        real(C_DOUBLE) :: threshold

        integer(C_INT) :: n
        real(C_DOUBLE) :: eps

        success = .true.
        do n = 1, count
            eps = abs(ary(n) - solution(n))
            if (abs(eps) > threshold) then
                success = .false.
                return
            end if
        end do
        end function validate_complex

        logical function validate_real(ary, solution, count, threshold) 
     &  result(success)
        use, intrinsic :: iso_c_binding

        implicit none

        real(C_DOUBLE), dimension(*), intent(in) :: ary
        real(C_DOUBLE), dimension(*), intent(in) :: solution
        integer(C_INT) :: count
        real(C_DOUBLE) :: threshold

        integer(C_INT) :: n
        real(C_DOUBLE) :: eps

        success = .true.
        do n = 1, count
            eps = (ary(n) - solution(n))
            if (abs(eps) > threshold) then
                success = .false.
                return
            end if
        end do
        end function validate_real

        subroutine print_complex(ary, count)
        use, intrinsic :: iso_c_binding

        implicit none

        complex(C_DOUBLE_COMPLEX), dimension(*), intent(in) :: ary
        integer(C_INT) :: count

        integer(C_INT) :: n

        write(*, *) (ary(n), n = 1, count)
        end subroutine print_complex
        
        subroutine print_real(ary, count)
        use, intrinsic :: iso_c_binding
    
        implicit none
    
        real(C_DOUBLE), dimension(*), intent(in) :: ary
        integer(C_INT) :: count
    
        integer(C_INT) :: n
    
        write(*, *) (ary(n), n = 1, count)
        end subroutine print_real
        