!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SAXPBY 
!
!******************************************************************************
      program   SAXPBY_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, incy, i
      parameter       (n=5, incx=1, incy=1)
      real            alpha, beta
      parameter       (alpha=1.0, beta=2.0)
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
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
      print 100, n, incx, incy, alpha, beta
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')
*
*      Call SAXPBY subroutine
      call SAXPBY(n,alpha,x,incx,beta,y,incy)
*
      print*
      print 101
      call print_svector(y,n,incy,'Y')

      stop

 99   format('Example: SAXPBY scales vector X and vector Y by two'
     &       ' scalars, adds them to one another, and stores in the'
     &       ' vector Y')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1,', '
     &       'alpha=',f3.1,', beta=',f3.1)
 101  format('The result of SAXPBY: ')
      end
