!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CHPR 
!
!******************************************************************************
      program   CHPR_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, i
      parameter       (n=2, incx=1)
      real            alpha
      parameter       (alpha=1.0)
      integer         xsize
      parameter       (xsize=1+(n-1)*abs(incx))
      complex         x(xsize), a(n,n)

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
      print 100, n, incx
      print 101, alpha
      call fill_cvector(x,n,incx)
      call fill_cmatrix(a,n,n,n,uplo,'U')
      call print_cmatrix(a,n,n,n,'A')
      call print_cvector(x,n,incx,'X')
*
*      Call CHPR subroutine
      call CHPR(uplo,n,alpha,x,incx,a)
*
      print*
      print 102
      call print_cmatrix(a,n,n,n,'A')

      stop
 99   format('Example: CHPR for the packed Hermitian rank-1 update')
 100  format('#### args: n=',i1,', incx=',i1)
 101  format(11x,'alpha=',f3.1)
 102  format('The result of CHPR: ')
      end
