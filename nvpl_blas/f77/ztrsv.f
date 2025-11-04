!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZTRSV
!
!******************************************************************************
      program   ZTRSV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, i, lda
      parameter       (n=2, incx=1, lda=2)
      integer         xsize, apsize
      parameter       (xsize=2+(n-1)*abs(incx))
      double complex  x(xsize), a(lda,n)

*       External Subroutines
      external        print_zvector, fill_zvector,
     &                print_zmatrix, fill_zmatrix
      character*1     uplo, trans, diag
      parameter       (uplo='U', trans='N', diag='N')
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, lda, uplo, trans, diag, incx
      call fill_zmatrix(a,n,n,lda,uplo,diag)
      call fill_zvector(x,n,incx)
      call print_zvector(x,n,incx,'X')
      call print_zmatrix(a,n,n,lda,'A')
*
*      Call ZTRSV subroutine
      call ZTRSV(uplo,trans,diag,n,a,lda,x,incx)
*
      print*
      print 102
      call print_zvector(x,n,incx,'X')

      stop
 99   format('Example: ZTRSV for solving a system of linear equations ',
     &       'whose coefficients are in a triangular matrix')
 100  format('#### args: n=',i1,', lda=',i1,', uplo=',a1,', trans=',a1,
     &       ', diag=',a1,', incx=',i1)
 102  format('The result of ZTRSV: ')
      end
