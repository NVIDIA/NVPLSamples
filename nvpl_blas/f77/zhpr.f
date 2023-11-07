!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZHPR 
!
!******************************************************************************
      program   ZHPR_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, i
      parameter       (n=2, incx=1)
      double precision alpha
      parameter       (alpha=1.0)
      integer         xsize
      parameter       (xsize=1+(n-1)*abs(incx))
      double complex  x(xsize), a(n,n)

*       External Subroutines
      external        print_zvector, fill_zvector, fill_zmatrix,
     &                print_zmatrix
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
      print 100, n, incx
      print 101, alpha
      call fill_zvector(x,n,incx)
      call fill_zmatrix(a,n,n,n,uplo,'U')
      call print_zmatrix(a,n,n,n,'A')
      call print_zvector(x,n,incx,'X')
*
*      Call ZHPR subroutine
      call ZHPR(uplo,n,alpha,x,incx,a)
*
      print*
      print 102
      call print_zmatrix(a,n,n,n,'A')

      stop
 99   format('Example: ZHPR for the packed Hermitian rank-1 update')
 100  format('#### args: n=',i1,', incx=',i1)
 101  format(11x,'alpha=',f4.2)
 102  format('The result of ZHPR: ')
      end
