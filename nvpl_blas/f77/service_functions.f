      program SERVICE_FUNCTIONS

      implicit none

      include 'nvpl_blas.fi'

      integer M
      parameter (M = 128)
      real A(M, M), B(M, M), C(M, M)

      integer*4 ver, ver_major, ver_minor, ver_patch
      integer*4 nt, old_nt

      ver = nvpl_blas_get_version()
      ver_major = ver / 10000
      ver_minor = mod(ver / 100, 100)
      ver_patch = mod(ver, 100)
      print '(a,i2,a,i2,a,i2)', "This example runs NVPL BLAS version ", &
     &      ver_major, ".", ver_minor, ".", ver_patch

      nt = nvpl_blas_get_max_threads()
      print '(a,i2,a)', "NVPL BLAS targets using ", nt, " threads"
      call sgemm('N', 'N', M, M, M, 1., A, M, B, M, 0., C, M)

      nt = 2
      print '(a,i2)', "Set the number of threads for NVPL BLAS to ", nt
      call nvpl_blas_set_num_threads(nt)
      nt = nvpl_blas_get_max_threads()
      print '(a,i2,a)', "Now NVPL BLAS targets using ", nt, " threads"
      call sgemm('N', 'N', M, M, M, 1., A, M, B, M, 0., C, M)

      nt = 4
      print '(a,i2)', "Set local number of threads for NVPL BLAS to ",  &
     &      nt
      old_nt = nvpl_blas_set_num_threads_local(nt)
      call sgemm('N', 'N', M, M, M, 1., A, M, B, M, 0., C, M)
      print '(a,i2)', "Restore the local number of threads to ", old_nt
      old_nt = nvpl_blas_set_num_threads_local(old_nt)

      end
