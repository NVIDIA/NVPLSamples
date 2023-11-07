!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SCOPY 
!
!******************************************************************************
      program   SCOPY_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, incy, i
      parameter       (n=5, incx=1, incy=1)
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx), ysize=1+(n-1)*abs(incy))
      real            x(xsize), y(ysize)
      data            (x(i), i=1,xsize)/1.0,-3.7,4.3,0.0,5.0/,
     &                (y(i), i=1,ysize)/ysize*2.0/

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
*
*      Call SCOPY subroutine
      call SCOPY(n,x,incx,y,incy)
*
      print*
      print 101
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')

      stop
 99   format('Example: SCOPY for copying the vector x into the'
     &       ' vector y')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1)
 101  format('The result of copying the vector x into the vector y:')
      end
