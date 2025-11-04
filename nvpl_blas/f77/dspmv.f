!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DSPMV
!
!******************************************************************************
      program   DSPMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo
      parameter       (uplo='U')
      integer         m, n, incx, incy, i
      parameter       (m=2, n=2, incx=1, incy=1)
      double precision alpha, beta
      parameter       (alpha=1.0, beta =1.0)
      integer         xsize, ysize, apsize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy),
     &                 apsize=(n*(n+1))/2)
      double precision x(xsize), y(ysize), ap(apsize)

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
      print 100, n, incx, incy, uplo
      print 101, alpha, beta
      call fill_dvector(x,n,incx)
      call fill_dvector(y,n,incy)
      call fill_dvector(ap,apsize,1)
      call print_dvector(ap,apsize,1,'AP')
      call print_dvector(x,n,incx,'X')
      call print_dvector(y,n,incy,'Y')
*
*      Call DSPMV subroutine
      call DSPMV(uplo,n,alpha,ap,x,incx,beta,y,incy)
*
      print*
      print 102
      call print_dvector(y,n,incy,'Y')

      stop
 99   format('Example: DSPMV for a matrix-vector product',
     &       ' with a symmetric packed matrix')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1,
     &       ', uplo=',a1)
 101  format(11x,'alpha=',f4.2,", beta=",f4.2)
 102  format('The result of DSPMV: ')
      end
