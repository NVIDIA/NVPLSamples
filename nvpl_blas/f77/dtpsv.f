!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DTPSV 
!
!******************************************************************************
      program   DTPSV_MAIN
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
      double precision x(xsize), ap(apsize)

*       External Subroutines
      external        print_dvector, fill_dvector
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, incx, uplo, transa, diag
      call fill_dvector(x,n,incx)
      call fill_dvector(ap,apsize,1)
      call print_dvector(ap,apsize,1,'AP')
      call print_dvector(x,n,incx,'X')
*
*      Call DTPSV subroutine
      call DTPSV(uplo,transa,diag,n,ap,x,incx)
*
      print*
      print 102, 'After DTPSV operation:'
      call print_dvector(x,n,incx,'X')

      stop
 99   format('Example: DTPSV for solving a system of linear equations'
     &       ' whose coefficients are in a triangular packed matrix.')
 100  format('#### args: n=',i1,', incx=',i1,
     &       ', uplo=',a1,', transa=',a1,', diag=',a1)
 102  format(A)
      end
