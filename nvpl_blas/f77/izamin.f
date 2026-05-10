!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     IZAMIN
!
!******************************************************************************
      program   IZAMIN_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, i
      parameter       (n=5, incx=1)
      integer         xsize, index
      parameter       (xsize=1+(n-1)*abs(incx))
      complex*16      x(xsize)
      data            (x(i), i=1,xsize)/(1.0D0,1.0D0),
     &                (-3.7D0,0.0D0),(4.3D0,0.0D0),
     &                (0.25D0,0.5D0),(5.0D0,0.0D0)/

*       External Subroutines
      external        print_zvector
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, incx
      call print_zvector(x,n,incx,'X')
*
*      Call IZAMIN subroutine
      index = IZAMIN(n,x,incx)
*
      print*
      print 101, index

      stop
 99   format('Example: IZAMIN for finding the (smallest) index of ',
     &       'the element of the minimum magnitude')
 100  format('#### args: n=',i1,', incx=',i1)
 101  format('The (smallest) index of the element of the minimum ',
     &       'magnitude:', i1)
      end
