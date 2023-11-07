!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CTRMV 
!
!******************************************************************************
      program   CTRMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo, transa, diag
      parameter       (uplo='U', transa='N', diag='N')
      integer         n, lda
      integer         incx, i
      parameter       (n=2, lda=2, incx=1)
      complex         a(lda,n)
      integer         xsize
      parameter       (xsize=1+(n-1)*abs(incx))
      complex         x(xsize)
      data            (x(i), i=1,xsize)/1.0,-3.7/

*       External Subroutines
      external        print_cmatrix, fill_cmatrix, print_cvector
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, lda, incx, uplo, transa, diag
      call fill_cmatrix(a,n,n,lda,uplo,diag)
      call print_cmatrix(a,n,n,lda,'A')
      call print_cvector(x,n,incx,'X')
*
*      Call CTRMV subroutine
      call CTRMV(uplo,transa,diag,n,a,lda,x,incx)
*
      print*
      print 102, 'After ctrmv operation:'
      call print_cvector(x,n,incx,'X')

      stop
 99   format('Example: CTRMV for the triangular matrix-vector '
     &       'multiplication')
 100  format('#### args: n=',i1,', lda=',i1,', incx=',i1,', uplo=',a1,
     &       ', transa=',a1,', diag=',a1)
 102  format(A)
      end
