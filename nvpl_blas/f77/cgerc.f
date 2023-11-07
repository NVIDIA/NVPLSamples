!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CGERC 
!
!******************************************************************************
      program   CGERC_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         m, n, incx, incy, i, lda
      parameter       (m=2, n=2, incx=1, incy=1, lda=2)
      complex         alpha
      parameter       (alpha=(1.0,2.0))
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      complex         x(xsize), y(ysize), a(lda,n)

*       External Subroutines
      external        print_cvector, fill_cvector, fill_cmatrix,
     &                print_cmatrix
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
      call fill_cvector(x,n,incx)
      call fill_cvector(y,n,incy)
      call fill_cmatrix(a,m,n,lda,'F','N')
      call print_cmatrix(a,m,n,lda,'A')
      call print_cvector(x,n,incx,'X')
      call print_cvector(y,n,incy,'Y')
*
*      Call CGERC subroutine
      call CGERC(m,n,alpha,x,incx,y,incy,a,lda)
*
      print*
      print 102
      call print_cmatrix(a,m,n,lda,'A')

      stop
 99   format('Example: CGERC for the rank-1 update')
 100  format('#### args: m=',i1,', n=',i1,', lda=',i1,', incx=',i1,
     &       ', incy=',i1)
 101  format(11x,'alpha=(',f3.1,', ',f3.1,')')
 102  format('The result of CGERC: ')
      end
