!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SSYR 
!
!******************************************************************************
      program   SSYR_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, i, lda
      parameter       (n=2, incx=1, lda=2)
      real            alpha
      parameter       (alpha=1.0)
      integer         xsize
      parameter       (xsize=1+(n-1)*abs(incx))
      real            x(xsize), a(lda,n)

*       External Subroutines
      external        print_svector, fill_svector, fill_smatrix,
     &                print_smatrix
      character*1     uplo
      parameter       (uplo='U')
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, lda, incx
      print 101, alpha
      call fill_svector(x,n,incx)
      call fill_smatrix(a,n,n,lda,uplo,'N')
      call print_smatrix(a,n,n,lda,'A')
      call print_svector(x,n,incx,'X')
*
*      Call SSYR subroutine
      call SSYR(uplo,n,alpha,x,incx,a,lda)
*
      print*
      print 102
      call print_smatrix(a,n,n,lda,'A')

      stop
 99   format('Example: SSYR for a rank-1 update of a symmetric matrix')
 100  format('#### args: n=',i1,', lda=',i1,', incx=',i1)
 101  format(11x,'alpha=',f4.2)
 102  format('The result of SSYR: ')
      end
