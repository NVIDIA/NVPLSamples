!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SSPR2 
!
!******************************************************************************
      program   SSPR2_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo
      parameter       (uplo='U')
      integer         m, n, incx, incy, i
      parameter       (m=2, n=2, incx=1, incy=1)
      real            alpha
      parameter       (alpha=1.0)
      integer         xsize, ysize, apsize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy),
     &                 apsize=(n*(n+1))/2)
      real            x(xsize), y(ysize), ap(apsize)

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
      print 100, n, incx, incy, uplo
      print 101, alpha
      call fill_svector(x,n,incx)
      call fill_svector(y,n,incy)
      call fill_svector(ap,apsize,1)
      call print_svector(ap,apsize,1,'AP')
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')
*
*      Call SSPR2 subroutine
      call SSPR2(uplo,n,alpha,x,incx,y,incy,ap)
*
      print*
      print 102
      call print_svector(ap,apsize,1,'AP')

      stop
 99   format('Example: SSPR2 for a rank-2 update of a'
     &       ' symmetric packed matrix')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1,
     &       ', uplo=',a1)
 101  format(11x,'alpha=',f4.2)
 102  format('The result of SSPR2: ')
      end
