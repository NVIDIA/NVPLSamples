!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     STPSV 
!
!******************************************************************************
      program   STPSV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1     uplo, transa, diag
      parameter       (uplo='U', transa='N', diag='N')
      integer         n, incx, i, j
      parameter       (n=2, incx=1)
      integer         xsize, apsize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 apsize=(n*(n+1))/2)
      real            x(xsize), ap(apsize)

*       External Subroutines
      external        print_svector, fill_svector
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, incx, uplo, transa, diag
      call fill_svector(x,n,incx)
      call fill_svector(ap,apsize,1)
      call print_svector(ap,apsize,1,'AP')
      call print_svector(x,n,incx,'X')
*
*      Call STPSV subroutine
      call STPSV(uplo,transa,diag,n,ap,x,incx)
*
      print*
      print 102, 'After STPSV operation:'
      call print_svector(x,n,incx,'X')

      stop
 99   format('Example: STPSV for solving a system of linear equationse'
     &       ' whose coefficients are in a triangular packed matrix.')
 100  format('#### args: n=',i1,', incx=',i1
     &       ', uplo=',a1,', transa=',a1,', diag=',a1)
 102  format(A)
      end
