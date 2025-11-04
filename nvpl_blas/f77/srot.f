!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SROT
!
!******************************************************************************
      program   SROT_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic       sqrt, abs

      integer         n, incx, incy, i
      parameter       (n=5, incx=1, incy=1)
      real            s, c
      parameter       (s=0.5, c=sqrt(1-s*s))
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
      print 100, n, incx, incy, c, s
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')
*
*      Call SROT subroutine
      call SROT(n,x,incx,y,incy,c,s)
*
      print*
      print 101
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')

      stop
 99   format('Example: SROT for applying Givens rotation matrix')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1,
     &       ', c=',f5.3,', s=',f5.3)
 101  format('The result of SROT: ')
      end
