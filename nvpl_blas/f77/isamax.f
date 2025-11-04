!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ISAMAX
!
!******************************************************************************
      program   ISAMAX_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, i
      parameter       (n=5, incx=1)
      integer         xsize, index
      parameter       (xsize=1+(n-1)*abs(incx))
      real            x(xsize)
      data            (x(i), i=1,xsize)/1.0,-3.7,4.3,0.0,5.0/


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
      print 100, n, incx
      call print_svector(x,n,incx,'X')
*
*      Call ISAMAX subroutine
      index = isamax(n,x,incx)
*
      print*
      print 101, index

      stop
 99   format('Example: ISAMAX for finding the (smallest) index of ',
     &       'the element of the maximum magnitude')
 100  format('#### args: n=',i1,', incx=',i1)
 101  format('The (smallest) index of the element of the maximum ',
     &       'magnitude:', i1)
      end
