!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SSCAL
!
!******************************************************************************
      program   SSCAL_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, i
      parameter       (n=5, incx=1)
      real            alpha
      parameter       (alpha=1.0)
      integer         xsize
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
      print 100, n, incx, alpha
      call print_svector(x,n,incx,'X')
*
*      Call SSCAL subroutine
      call SSCAL(n,alpha,x,incx)
*
      print*
      print 101
      call print_svector(x,n,incx,'X')

      stop
 99   format('Example: SSCAL for multiplying the vector x by the',
     &       ' scalar')
 100  format('#### args: n=',i1,', incx=',i1,', alpha=',f3.1)
 101  format('The result of SSCAL: ')
      end
