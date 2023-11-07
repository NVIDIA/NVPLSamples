!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DSWAP 
!
!******************************************************************************
      program   DSWAP_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer          n, incx, incy, i
      parameter        (n=5, incx=1, incy=1)
      integer          xsize, ysize
      parameter        (xsize=1+(n-1)*abs(incx),
     &                  ysize=1+(n-1)*abs(incy))
      double precision x(xsize), y(ysize)
      data             (x(i), i=1,xsize)/1.0,-3.7,4.3,0.0,5.0/,
     &                 (y(i), i=1,ysize)/ysize*2.0/

*       External Subroutines
      external        print_dvector 
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, incx, incy
      call print_dvector(x,n,incx,'X')
      call print_dvector(y,n,incy,'Y')
*
*      Call DSWAP subroutine
      call DSWAP(n,x,incx,y,incy)
*
      print*
      print 101
      call print_dvector(x,n,incx,'X')
      call print_dvector(y,n,incy,'Y')

      stop
 99   format('Example: DSWAP for interchanging the elements '
     &       'of vectors X and Y')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1)
 101  format('The result of interchanging the elements of vector '
     &       'x and y:')
      end
