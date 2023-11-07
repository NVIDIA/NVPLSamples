!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZTPMV 
!
!******************************************************************************
      program   ZTPMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, i
      parameter       (n=2, incx=1)
      integer         xsize, apsize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 apsize=n*(n+1)/2)
      double complex  x(xsize), ap(apsize)

*       External Subroutines
      external        print_zvector, fill_zvector
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
      print 100, n, uplo, trans, diag, incx
      call fill_zvector(x,n,incx)
      call fill_zvector(ap,apsize,1)
      call print_zvector(x,n,incx,'X')
      call print_zvector(ap,apsize,1,'AP')
*
*      Call ZTPMV subroutine
      call ZTPMV(uplo,trans,diag,n,ap,x,incx)
*
      print*
      print 102
      call print_zvector(x,n,incx,'X')

      stop
 99   format('Example: ZTPMV for a matrix-vector product using a'
     &       ' triangular packed matrix')
 100  format('#### args: n=',i1,'uplo=',a1,', trans=',a1,
     &       ', diag=',a1,', incx=',i1)
 102  format('The result of ZTPMV: ')
      end
