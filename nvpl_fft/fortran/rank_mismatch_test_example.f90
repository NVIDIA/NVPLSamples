! Test that 2D/3D arrays can be passed to dimension(*) interfaces.
! With the old named-interface fftw3.f03, this file would fail to compile
! due to Fortran TKR rank checking on assumed-size arguments.
program main
    implicit none

    logical :: success
    logical, external :: run_double_2d_test
    logical, external :: run_double_3d_test
    logical, external :: run_single_2d_test
    logical, external :: run_single_3d_test
    logical, external :: run_double_c2c_rank_cmp_test
    logical, external :: run_single_c2c_rank_cmp_test
    logical, external :: run_double_r2c_rank_cmp_test

    success = .true.

    if (success) then
        success = run_double_2d_test()
        print *, "run_double_2d_test: ", success
    end if
    if (.not. success) call exit(1)

    if (success) then
        success = run_double_3d_test()
        print *, "run_double_3d_test: ", success
    end if
    if (.not. success) call exit(1)

    if (success) then
        success = run_single_2d_test()
        print *, "run_single_2d_test: ", success
    end if
    if (.not. success) call exit(1)

    if (success) then
        success = run_single_3d_test()
        print *, "run_single_3d_test: ", success
    end if
    if (.not. success) call exit(1)

    if (success) then
        success = run_double_c2c_rank_cmp_test()
        print *, "run_double_c2c_rank_cmp_test: ", success
    end if
    if (.not. success) call exit(1)

    if (success) then
        success = run_single_c2c_rank_cmp_test()
        print *, "run_single_c2c_rank_cmp_test: ", success
    end if
    if (.not. success) call exit(1)

    if (success) then
        success = run_double_r2c_rank_cmp_test()
        print *, "run_double_r2c_rank_cmp_test: ", success
    end if
    if (.not. success) call exit(1)

    call exit(0)
end program

! Double-precision: pass a 2D array to plan_many_dft / execute_dft
logical function run_double_2d_test() result(success)
    use, intrinsic :: iso_c_binding
    implicit none
    include 'fftw3.f03'

    integer(C_INT), parameter :: NX = 8, NY = 4
    integer(C_INT), parameter :: NTOT = NX * NY
    integer(C_INT), dimension(1) :: n_arr
    integer(C_INT), dimension(1) :: inembed, onembed

    complex(C_DOUBLE_COMPLEX), pointer :: arr_2d(:,:)
    complex(C_DOUBLE_COMPLEX), pointer :: tmp_2d(:,:)
    type(C_PTR) :: arr_ptr, tmp_ptr
    type(C_PTR) :: plan_fwd, plan_bwd

    integer :: i, j
    real(C_DOUBLE) :: eps

    success = .true.

    n_arr = (/ NTOT /)
    inembed = (/ NTOT /)
    onembed = (/ NTOT /)

    arr_ptr = fftw_alloc_complex(int(NTOT, C_SIZE_T))
    tmp_ptr = fftw_alloc_complex(int(NTOT, C_SIZE_T))
    ! Shape as 2D — the rank mismatch that triggers the TKR bug
    call c_f_pointer(arr_ptr, arr_2d, [NX, NY])
    call c_f_pointer(tmp_ptr, tmp_2d, [NX, NY])

    do j = 1, NY
        do i = 1, NX
            arr_2d(i, j) = cmplx((j - 1) * NX + i, 0.0d0, C_DOUBLE_COMPLEX)
        end do
    end do

    ! rank=1 FFT of length NTOT, howmany=1, passing 2D arrays
    plan_fwd = fftw_plan_many_dft(1, n_arr, 1, &
        arr_2d, inembed, 1, NTOT, &
        tmp_2d, onembed, 1, NTOT, &
        FFTW_FORWARD, FFTW_ESTIMATE)
    plan_bwd = fftw_plan_many_dft(1, n_arr, 1, &
        tmp_2d, onembed, 1, NTOT, &
        arr_2d, inembed, 1, NTOT, &
        FFTW_BACKWARD, FFTW_ESTIMATE)

    ! Also tests execute_dft with rank-mismatched arrays
    call fftw_execute_dft(plan_fwd, arr_2d, tmp_2d)
    call fftw_execute_dft(plan_bwd, tmp_2d, arr_2d)

    ! Verify round-trip: arr should equal original * NTOT
    do j = 1, NY
        do i = 1, NX
            eps = abs(arr_2d(i, j) / NTOT - cmplx((j - 1) * NX + i, 0.0d0, C_DOUBLE_COMPLEX))
            if (eps > 1.0d-10) then
                success = .false.
                call fftw_destroy_plan(plan_fwd)
                call fftw_destroy_plan(plan_bwd)
                call fftw_free(arr_ptr)
                call fftw_free(tmp_ptr)
                return
            end if
        end do
    end do

    call fftw_destroy_plan(plan_fwd)
    call fftw_destroy_plan(plan_bwd)
    call fftw_free(arr_ptr)
    call fftw_free(tmp_ptr)
end function run_double_2d_test

! Double-precision: pass a 3D array to execute_dft
logical function run_double_3d_test() result(success)
    use, intrinsic :: iso_c_binding
    implicit none
    include 'fftw3.f03'

    integer(C_INT), parameter :: NX = 4, NY = 4, NZ = 2
    integer(C_INT), parameter :: NTOT = NX * NY * NZ
    integer(C_INT), dimension(1) :: n_arr
    integer(C_INT), dimension(1) :: inembed, onembed

    complex(C_DOUBLE_COMPLEX), pointer :: arr_3d(:,:,:)
    complex(C_DOUBLE_COMPLEX), pointer :: tmp_3d(:,:,:)
    type(C_PTR) :: arr_ptr, tmp_ptr
    type(C_PTR) :: plan_fwd, plan_bwd

    integer :: i, j, k
    real(C_DOUBLE) :: eps, expected

    success = .true.

    n_arr = (/ NTOT /)
    inembed = (/ NTOT /)
    onembed = (/ NTOT /)

    arr_ptr = fftw_alloc_complex(int(NTOT, C_SIZE_T))
    tmp_ptr = fftw_alloc_complex(int(NTOT, C_SIZE_T))
    call c_f_pointer(arr_ptr, arr_3d, [NX, NY, NZ])
    call c_f_pointer(tmp_ptr, tmp_3d, [NX, NY, NZ])

    do k = 1, NZ
        do j = 1, NY
            do i = 1, NX
                arr_3d(i, j, k) = cmplx((k - 1) * NY * NX + (j - 1) * NX + i, &
                                         0.0d0, C_DOUBLE_COMPLEX)
            end do
        end do
    end do

    plan_fwd = fftw_plan_many_dft(1, n_arr, 1, &
        arr_3d, inembed, 1, NTOT, &
        tmp_3d, onembed, 1, NTOT, &
        FFTW_FORWARD, FFTW_ESTIMATE)
    plan_bwd = fftw_plan_many_dft(1, n_arr, 1, &
        tmp_3d, onembed, 1, NTOT, &
        arr_3d, inembed, 1, NTOT, &
        FFTW_BACKWARD, FFTW_ESTIMATE)

    call fftw_execute_dft(plan_fwd, arr_3d, tmp_3d)
    call fftw_execute_dft(plan_bwd, tmp_3d, arr_3d)

    do k = 1, NZ
        do j = 1, NY
            do i = 1, NX
                expected = dble((k - 1) * NY * NX + (j - 1) * NX + i)
                eps = abs(arr_3d(i, j, k) / NTOT - cmplx(expected, 0.0d0, C_DOUBLE_COMPLEX))
                if (eps > 1.0d-10) then
                    success = .false.
                    call fftw_destroy_plan(plan_fwd)
                    call fftw_destroy_plan(plan_bwd)
                    call fftw_free(arr_ptr)
                    call fftw_free(tmp_ptr)
                    return
                end if
            end do
        end do
    end do

    call fftw_destroy_plan(plan_fwd)
    call fftw_destroy_plan(plan_bwd)
    call fftw_free(arr_ptr)
    call fftw_free(tmp_ptr)
end function run_double_3d_test

! Single-precision: pass a 2D array to fftwf_plan_many_dft / fftwf_execute_dft
logical function run_single_2d_test() result(success)
    use, intrinsic :: iso_c_binding
    implicit none
    include 'fftw3.f03'

    integer(C_INT), parameter :: NX = 8, NY = 4
    integer(C_INT), parameter :: NTOT = NX * NY
    integer(C_INT), dimension(1) :: n_arr
    integer(C_INT), dimension(1) :: inembed, onembed

    complex(C_FLOAT_COMPLEX), pointer :: arr_2d(:,:)
    complex(C_FLOAT_COMPLEX), pointer :: tmp_2d(:,:)
    type(C_PTR) :: arr_ptr, tmp_ptr
    type(C_PTR) :: plan_fwd, plan_bwd

    integer :: i, j
    real(C_FLOAT) :: eps

    success = .true.

    n_arr = (/ NTOT /)
    inembed = (/ NTOT /)
    onembed = (/ NTOT /)

    arr_ptr = fftwf_alloc_complex(int(NTOT, C_SIZE_T))
    tmp_ptr = fftwf_alloc_complex(int(NTOT, C_SIZE_T))
    call c_f_pointer(arr_ptr, arr_2d, [NX, NY])
    call c_f_pointer(tmp_ptr, tmp_2d, [NX, NY])

    do j = 1, NY
        do i = 1, NX
            arr_2d(i, j) = cmplx(real((j - 1) * NX + i), 0.0, C_FLOAT_COMPLEX)
        end do
    end do

    plan_fwd = fftwf_plan_many_dft(1, n_arr, 1, &
        arr_2d, inembed, 1, NTOT, &
        tmp_2d, onembed, 1, NTOT, &
        FFTW_FORWARD, FFTW_ESTIMATE)
    plan_bwd = fftwf_plan_many_dft(1, n_arr, 1, &
        tmp_2d, onembed, 1, NTOT, &
        arr_2d, inembed, 1, NTOT, &
        FFTW_BACKWARD, FFTW_ESTIMATE)

    call fftwf_execute_dft(plan_fwd, arr_2d, tmp_2d)
    call fftwf_execute_dft(plan_bwd, tmp_2d, arr_2d)

    do j = 1, NY
        do i = 1, NX
            eps = abs(arr_2d(i, j) / NTOT - cmplx(real((j - 1) * NX + i), 0.0, C_FLOAT_COMPLEX))
            if (eps > 1.0e-4) then
                success = .false.
                call fftwf_destroy_plan(plan_fwd)
                call fftwf_destroy_plan(plan_bwd)
                call fftwf_free(arr_ptr)
                call fftwf_free(tmp_ptr)
                return
            end if
        end do
    end do

    call fftwf_destroy_plan(plan_fwd)
    call fftwf_destroy_plan(plan_bwd)
    call fftwf_free(arr_ptr)
    call fftwf_free(tmp_ptr)
end function run_single_2d_test

! Single-precision: pass a 3D array to fftwf_execute_dft
logical function run_single_3d_test() result(success)
    use, intrinsic :: iso_c_binding
    implicit none
    include 'fftw3.f03'

    integer(C_INT), parameter :: NX = 4, NY = 4, NZ = 2
    integer(C_INT), parameter :: NTOT = NX * NY * NZ
    integer(C_INT), dimension(1) :: n_arr
    integer(C_INT), dimension(1) :: inembed, onembed

    complex(C_FLOAT_COMPLEX), pointer :: arr_3d(:,:,:)
    complex(C_FLOAT_COMPLEX), pointer :: tmp_3d(:,:,:)
    type(C_PTR) :: arr_ptr, tmp_ptr
    type(C_PTR) :: plan_fwd, plan_bwd

    integer :: i, j, k
    real(C_FLOAT) :: eps, expected

    success = .true.

    n_arr = (/ NTOT /)
    inembed = (/ NTOT /)
    onembed = (/ NTOT /)

    arr_ptr = fftwf_alloc_complex(int(NTOT, C_SIZE_T))
    tmp_ptr = fftwf_alloc_complex(int(NTOT, C_SIZE_T))
    call c_f_pointer(arr_ptr, arr_3d, [NX, NY, NZ])
    call c_f_pointer(tmp_ptr, tmp_3d, [NX, NY, NZ])

    do k = 1, NZ
        do j = 1, NY
            do i = 1, NX
                arr_3d(i, j, k) = cmplx(real((k - 1) * NY * NX + (j - 1) * NX + i), &
                                         0.0, C_FLOAT_COMPLEX)
            end do
        end do
    end do

    plan_fwd = fftwf_plan_many_dft(1, n_arr, 1, &
        arr_3d, inembed, 1, NTOT, &
        tmp_3d, onembed, 1, NTOT, &
        FFTW_FORWARD, FFTW_ESTIMATE)
    plan_bwd = fftwf_plan_many_dft(1, n_arr, 1, &
        tmp_3d, onembed, 1, NTOT, &
        arr_3d, inembed, 1, NTOT, &
        FFTW_BACKWARD, FFTW_ESTIMATE)

    call fftwf_execute_dft(plan_fwd, arr_3d, tmp_3d)
    call fftwf_execute_dft(plan_bwd, tmp_3d, arr_3d)

    do k = 1, NZ
        do j = 1, NY
            do i = 1, NX
                expected = real((k - 1) * NY * NX + (j - 1) * NX + i)
                eps = abs(arr_3d(i, j, k) / NTOT - cmplx(expected, 0.0, C_FLOAT_COMPLEX))
                if (eps > 1.0e-4) then
                    success = .false.
                    call fftwf_destroy_plan(plan_fwd)
                    call fftwf_destroy_plan(plan_bwd)
                    call fftwf_free(arr_ptr)
                    call fftwf_free(tmp_ptr)
                    return
                end if
            end do
        end do
    end do

    call fftwf_destroy_plan(plan_fwd)
    call fftwf_destroy_plan(plan_bwd)
    call fftwf_free(arr_ptr)
    call fftwf_free(tmp_ptr)
end function run_single_3d_test

! =========================================================================
! Rank-comparison tests: run the same 2D FFT on a 1D flat array and a 2D
! shaped array, verify the results are bitwise identical.
! =========================================================================

! Double C2C: plan_dft_2d + execute_dft on 1D vs 2D arrays
logical function run_double_c2c_rank_cmp_test() result(success)
    use, intrinsic :: iso_c_binding
    implicit none
    include 'fftw3.f03'

    integer(C_INT), parameter :: NX = 8, NY = 4
    integer(C_INT), parameter :: NTOT = NX * NY

    complex(C_DOUBLE_COMPLEX), pointer :: in_1d(:), out_1d(:)
    complex(C_DOUBLE_COMPLEX), pointer :: in_2d(:,:), out_2d(:,:)
    type(C_PTR) :: in1_ptr, out1_ptr, in2_ptr, out2_ptr
    type(C_PTR) :: plan_1d, plan_2d

    integer :: i, j, idx
    real(C_DOUBLE) :: eps

    success = .true.

    in1_ptr  = fftw_alloc_complex(int(NTOT, C_SIZE_T))
    out1_ptr = fftw_alloc_complex(int(NTOT, C_SIZE_T))
    in2_ptr  = fftw_alloc_complex(int(NTOT, C_SIZE_T))
    out2_ptr = fftw_alloc_complex(int(NTOT, C_SIZE_T))

    call c_f_pointer(in1_ptr,  in_1d,  [NTOT])
    call c_f_pointer(out1_ptr, out_1d, [NTOT])
    call c_f_pointer(in2_ptr,  in_2d,  [NX, NY])
    call c_f_pointer(out2_ptr, out_2d, [NX, NY])

    idx = 0
    do j = 1, NY
        do i = 1, NX
            idx = idx + 1
            in_1d(idx) = cmplx(dble(idx), dble(NTOT - idx), C_DOUBLE_COMPLEX)
            in_2d(i, j) = in_1d(idx)
        end do
    end do

    plan_1d = fftw_plan_dft_2d(NX, NY, in_1d, out_1d, FFTW_FORWARD, FFTW_ESTIMATE)
    plan_2d = fftw_plan_dft_2d(NX, NY, in_2d, out_2d, FFTW_FORWARD, FFTW_ESTIMATE)

    call fftw_execute_dft(plan_1d, in_1d, out_1d)
    call fftw_execute_dft(plan_2d, in_2d, out_2d)

    idx = 0
    do j = 1, NY
        do i = 1, NX
            idx = idx + 1
            eps = abs(out_1d(idx) - out_2d(i, j))
            if (eps > 1.0d-10) then
                print *, "  double c2c rank cmp mismatch at", idx, "eps=", eps
                success = .false.
                go to 100
            end if
        end do
    end do

100 call fftw_destroy_plan(plan_1d)
    call fftw_destroy_plan(plan_2d)
    call fftw_free(in1_ptr)
    call fftw_free(out1_ptr)
    call fftw_free(in2_ptr)
    call fftw_free(out2_ptr)
end function run_double_c2c_rank_cmp_test

! Single C2C: plan_dft_2d + execute_dft on 1D vs 2D arrays
logical function run_single_c2c_rank_cmp_test() result(success)
    use, intrinsic :: iso_c_binding
    implicit none
    include 'fftw3.f03'

    integer(C_INT), parameter :: NX = 8, NY = 4
    integer(C_INT), parameter :: NTOT = NX * NY

    complex(C_FLOAT_COMPLEX), pointer :: in_1d(:), out_1d(:)
    complex(C_FLOAT_COMPLEX), pointer :: in_2d(:,:), out_2d(:,:)
    type(C_PTR) :: in1_ptr, out1_ptr, in2_ptr, out2_ptr
    type(C_PTR) :: plan_1d, plan_2d

    integer :: i, j, idx
    real(C_FLOAT) :: eps

    success = .true.

    in1_ptr  = fftwf_alloc_complex(int(NTOT, C_SIZE_T))
    out1_ptr = fftwf_alloc_complex(int(NTOT, C_SIZE_T))
    in2_ptr  = fftwf_alloc_complex(int(NTOT, C_SIZE_T))
    out2_ptr = fftwf_alloc_complex(int(NTOT, C_SIZE_T))

    call c_f_pointer(in1_ptr,  in_1d,  [NTOT])
    call c_f_pointer(out1_ptr, out_1d, [NTOT])
    call c_f_pointer(in2_ptr,  in_2d,  [NX, NY])
    call c_f_pointer(out2_ptr, out_2d, [NX, NY])

    idx = 0
    do j = 1, NY
        do i = 1, NX
            idx = idx + 1
            in_1d(idx) = cmplx(real(idx), real(NTOT - idx), C_FLOAT_COMPLEX)
            in_2d(i, j) = in_1d(idx)
        end do
    end do

    plan_1d = fftwf_plan_dft_2d(NX, NY, in_1d, out_1d, FFTW_FORWARD, FFTW_ESTIMATE)
    plan_2d = fftwf_plan_dft_2d(NX, NY, in_2d, out_2d, FFTW_FORWARD, FFTW_ESTIMATE)

    call fftwf_execute_dft(plan_1d, in_1d, out_1d)
    call fftwf_execute_dft(plan_2d, in_2d, out_2d)

    idx = 0
    do j = 1, NY
        do i = 1, NX
            idx = idx + 1
            eps = abs(out_1d(idx) - out_2d(i, j))
            if (eps > 1.0e-4) then
                print *, "  single c2c rank cmp mismatch at", idx, "eps=", eps
                success = .false.
                go to 200
            end if
        end do
    end do

200 call fftwf_destroy_plan(plan_1d)
    call fftwf_destroy_plan(plan_2d)
    call fftwf_free(in1_ptr)
    call fftwf_free(out1_ptr)
    call fftwf_free(in2_ptr)
    call fftwf_free(out2_ptr)
end function run_single_c2c_rank_cmp_test

! Double R2C: plan_dft_r2c_2d + execute_dft_r2c on 1D vs 2D arrays
logical function run_double_r2c_rank_cmp_test() result(success)
    use, intrinsic :: iso_c_binding
    implicit none
    include 'fftw3.f03'

    integer(C_INT), parameter :: NX = 8, NY = 4
    integer(C_INT), parameter :: NY_C = NY / 2 + 1
    integer(C_INT), parameter :: NTOT_R = NX * NY
    integer(C_INT), parameter :: NTOT_C = NX * NY_C

    real(C_DOUBLE), pointer :: rin_1d(:)
    real(C_DOUBLE), pointer :: rin_2d(:,:)
    complex(C_DOUBLE_COMPLEX), pointer :: cout_1d(:)
    complex(C_DOUBLE_COMPLEX), pointer :: cout_2d(:,:)
    type(C_PTR) :: rin1_ptr, cout1_ptr, rin2_ptr, cout2_ptr
    type(C_PTR) :: plan_1d, plan_2d

    integer :: i, j, idx
    real(C_DOUBLE) :: eps

    success = .true.

    rin1_ptr  = fftw_alloc_real(int(NTOT_R, C_SIZE_T))
    cout1_ptr = fftw_alloc_complex(int(NTOT_C, C_SIZE_T))
    rin2_ptr  = fftw_alloc_real(int(NTOT_R, C_SIZE_T))
    cout2_ptr = fftw_alloc_complex(int(NTOT_C, C_SIZE_T))

    call c_f_pointer(rin1_ptr,  rin_1d,  [NTOT_R])
    call c_f_pointer(cout1_ptr, cout_1d, [NTOT_C])
    call c_f_pointer(rin2_ptr,  rin_2d,  [NX, NY])
    call c_f_pointer(cout2_ptr, cout_2d, [NX, NY_C])

    idx = 0
    do j = 1, NY
        do i = 1, NX
            idx = idx + 1
            rin_1d(idx) = dble(idx)
            rin_2d(i, j) = rin_1d(idx)
        end do
    end do

    plan_1d = fftw_plan_dft_r2c_2d(NX, NY, rin_1d, cout_1d, FFTW_ESTIMATE)
    plan_2d = fftw_plan_dft_r2c_2d(NX, NY, rin_2d, cout_2d, FFTW_ESTIMATE)

    call fftw_execute_dft_r2c(plan_1d, rin_1d, cout_1d)
    call fftw_execute_dft_r2c(plan_2d, rin_2d, cout_2d)

    idx = 0
    do j = 1, NY_C
        do i = 1, NX
            idx = idx + 1
            eps = abs(cout_1d(idx) - cout_2d(i, j))
            if (eps > 1.0d-10) then
                print *, "  double r2c rank cmp mismatch at", idx, "eps=", eps
                success = .false.
                go to 300
            end if
        end do
    end do

300 call fftw_destroy_plan(plan_1d)
    call fftw_destroy_plan(plan_2d)
    call fftw_free(rin1_ptr)
    call fftw_free(cout1_ptr)
    call fftw_free(rin2_ptr)
    call fftw_free(cout2_ptr)
end function run_double_r2c_rank_cmp_test
