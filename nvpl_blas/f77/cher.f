!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CHER 
!
!******************************************************************************
      program   CHER_MAIN
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
      complex         x(xsize), a(lda,n)

*       External Subroutines
      external        print_cvector, fill_cvector, fill_cmatrix,
     &                print_cmatrix
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
      call fill_cvector(x,n,incx)
      call fill_cmatrix(a,n,n,lda,uplo,'N')
      call print_cmatrix(a,n,n,lda,'A')
      call print_cvector(x,n,incx,'X')
*
*      Call CHER subroutine
      call CHER(uplo,n,alpha,x,incx,a,lda)
*
      print*
      print 102
      call print_cmatrix(a,n,n,lda,'A')

      stop
 99   format('Example: CHER for the Hermitian rank-1 update')
 100  format('#### args: n=',i1,', lda=',i1,', incx=',i1)
 101  format(11x,'alpha=',f3.1)
 102  format('The result of CHER: ')
      end
