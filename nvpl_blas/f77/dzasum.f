!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DZASUM
!
!******************************************************************************
      program   DZASUM_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic        abs

      integer          n, incx, i
      parameter        (n=5, incx=1)
      double precision result
      integer          xsize
      parameter        (xsize=1+(n-1)*abs(incx))
      double complex   x(xsize)
      data             (x(i), i=1,xsize)/1.0,-3.7,4.3,0.0,5.0/

*       External Subroutines
      external         print_zvector
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
*      Call DZASUM subroutine
      result = DZASUM(n,x,incx)
*
      print*
      print 101, result

      stop
 99   format('Example: DZASUM for computing the sum of the absolute',
     &       ' values of the elements of vector')
 100  format('#### args: n=',i1,', incx=',i1)
 101  format('The sum of the absolute values of the elements of',
     &       ' vector: ', f6.2)
      end
