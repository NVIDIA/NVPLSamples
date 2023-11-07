!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SGER 
!
!******************************************************************************
      program   SGER_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         m, n, incx, incy, i, lda
      parameter       (m=2, n=2, incx=1, incy=1, lda=2)
      real            alpha
      parameter       (alpha=1.0)
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      real            x(xsize), y(ysize), a(lda,n)

*       External Subroutines
      external        print_svector, fill_svector, fill_smatrix,
     &                print_smatrix
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, m, n, lda, incx, incy, alpha
      call fill_svector(x,n,incx)
      call fill_svector(y,n,incy)
      call fill_smatrix(a,m,n,lda,'F','N')
      call print_smatrix(a,m,n,lda,'A')
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')
*
*      Call SGER subroutine
      call SGER(m,n,alpha,x,incx,y,incy,a,lda)
*
      print*
      print 101
      call print_smatrix(a,m,n,lda,'A')

      stop
 99   format('Example: SGER for the rank-1 update')
 100  format('#### args: m=',i1,', n=',i1,', lda=',i1,', incx=',i1,
     &       ', incy=',i1,', alpha=',f3.1)
 101  format('The result of SGER: ')
      end
