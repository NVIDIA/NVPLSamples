!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZGERC 
!
!******************************************************************************
      program   ZGERC_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         m, n, incx, incy, i, lda
      parameter       (m=2, n=2, incx=1, incy=1, lda=2)
      double complex  alpha
      parameter       (alpha=(1.0,2.0))
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      double complex  x(xsize), y(ysize), a(lda,n)

*       External Subroutines
      external        print_zvector, fill_zvector, fill_zmatrix,
     &                print_zmatrix
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, m, n, lda, incx, incy
      print 101, alpha
      call fill_zvector(x,n,incx)
      call fill_zvector(y,n,incy)
      call fill_zmatrix(a,m,n,lda,'F','N')
      call print_zmatrix(a,m,n,lda,'A')
      call print_zvector(x,n,incx,'X')
      call print_zvector(y,n,incy,'Y')
*
*      Call ZGERC subroutine
      call ZGERC(m,n,alpha,x,incx,y,incy,a,lda)
*
      print*
      print 102
      call print_zmatrix(a,m,n,lda,'A')

      stop
 99   format('Example: ZGERC for the rank-1 update')
 100  format('#### args: m=',i1,', n=',i1,', lda=',i1,', incx=',i1,
     &       ', incy=',i1)
 101  format(11x,'alpha=(',f4.2,', ',f4.2,')')
 102  format('The result of ZGERC: ')
      end
