!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DAXPY
!
!******************************************************************************
      program   DAXPY_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic        abs

      integer          n, incx, incy, i
      parameter        (n=5, incx=1, incy=1)
      double precision alpha
      parameter        (alpha=1.0)
      integer          xsize, ysize
      parameter        (xsize=1+(n-1)*abs(incx),
     &                  ysize=1+(n-1)*abs(incy))
      double precision x(xsize), y(ysize)
      data             (x(i), i=1,xsize)/1.0,-3.7,4.3,0.0,5.0/,
     &                 (y(i), i=1,ysize)/ysize*2.0/

*       External Subroutines
      external         print_dvector
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, incx, incy, alpha
      call print_dvector(x,n,incx,'X')
      call print_dvector(y,n,incy,'Y')
*
*      Call DAXPY subroutine
      call DAXPY(n,alpha,x,incx,y,incy)
*
      print*
      print 101
      call print_dvector(y,n,incy,'Y')

      stop
 99   format('Example: DAXPY for multiplying the vector x by the',
     &       ' scalar and adds it to the vector y')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1,
     &       ', alpha=',f4.2)
 101  format('The result of DAXPY: ')
      end
