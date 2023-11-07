!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SSYMV 
!
!******************************************************************************
      program   SSYMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo
      parameter       (uplo='U')
      integer         n, lda, incx, incy, i
      parameter       (n=2, lda=2, incx=1, incy=1)
      real            alpha, beta
      parameter       (alpha=1.0, beta=1.0)
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      real            x(xsize), y(ysize), a(lda,n)

*       External Subroutines
      external        print_svector, fill_svector,
     &                print_smatrix, fill_smatrix
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, lda, incx, incy, uplo
      print 101, alpha, beta
      call fill_svector(x,n,incx)
      call fill_svector(y,n,incy)
      call fill_smatrix(a,n,n,lda,uplo,'N')
      call print_smatrix(a,n,n,lda,'A')
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')
*
*      Call SSYMV subroutine
      call SSYMV(uplo,n,alpha,a,lda,x,incx,beta,y,incy)
*
      print*
      print 102
      call print_svector(y,n,incy,'Y')

      stop
 99   format('Example: SSYMV for computing a matrix-vector product'
     &       ' for a symmetric matrix.')
 100  format('#### args: n=',i1,', lda=',i1,', incx=',i1,', incy=',i1,
     &       ', uplo=',a1)
 101  format(11x,'alpha=',f3.1,' beta=',f3.1)
 102  format('The result of SSYMV: ')
      end
