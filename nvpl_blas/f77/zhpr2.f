!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZHPR2 
!
!******************************************************************************
      program   ZHPR2_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, incy, i, lda
      parameter       (n=2, incx=1, incy=1, lda=2)
      double complex  alpha
      parameter       (alpha=(1.0, 2.0))
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      double complex  x(xsize), y(ysize), a(lda,n)

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
      print 100, n, lda, incx, incy
      print 101, alpha
      call fill_zvector(x,n,incx)
      call fill_zvector(y,n,incy)
      call fill_zmatrix(a,n,n,lda,uplo,'U')
      call print_zmatrix(a,n,n,lda,'A')
      call print_zvector(x,n,incx,'X')
      call print_zvector(y,n,incy,'Y')
*
*      Call ZHPR2 subroutine
      call ZHPR2(uplo,n,alpha,x,incx,y,incy,a)
*
      print*
      print 102
      call print_zmatrix(a,n,n,lda,'A')

      stop
 99   format('Example: ZHPR2 for the packed Hermitian rank-2 update')
 100  format('#### args: n=',i1,', lda=',i1,', incx=',i1,
     &       ', incy=',i1)
 101  format(11x,'alpha=',f4.2)
 102  format('The result of ZHPR2: ')
      end
