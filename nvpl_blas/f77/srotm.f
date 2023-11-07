!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SROTM 
!
!******************************************************************************
      program   SROTM_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic       abs

      integer         n, incx, incy, i
      parameter       (n=5, incx=1, incy=1)
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      real            x(xsize), y(ysize), P(5)
      data            (x(i), i=1,xsize)/1.0,-3.7,4.3,0.0,5.0/,
     &                (y(i), i=1,ysize)/ysize*2.0/,
     &                (P(i), i=1,5)/1.0,0.25,-1.0,1.0,0.5/

*       External Subroutines
      external        print_svector 
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, incx, incy
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')
      call print_svector(P,5,1,'P')
*
*      Call SROTM subroutine
      call SROTM(n,x,incx,y,incy,P)
*
      print*
      print 101
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')

      stop
 99   format('Example: SROTM for applying Givens rotation matrix')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1)
 101  format('The result of SROTM: ')
      end
