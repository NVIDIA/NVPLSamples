!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CHER2 
!
!******************************************************************************
      program   CHER2_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, incy, i, lda
      parameter       (n=2, incx=1, incy=1, lda=2)
      complex         alpha
      parameter       (alpha=(1.0, 2.0))
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      complex         x(xsize), y(ysize), a(lda,n)

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
      print 100, n, lda, incx, incy
      print 101, alpha
      call fill_cvector(x,n,incx)
      call fill_cvector(y,n,incy)
      call fill_cmatrix(a,n,n,lda,uplo,'N')
      call print_cmatrix(a,n,n,lda,'A')
      call print_cvector(x,n,incx,'X')
      call print_cvector(y,n,incy,'Y')
*
*      Call CHER2 subroutine
      call CHER2(uplo,n,alpha,x,incx,y,incy,a,lda)
*
      print*
      print 102
      call print_cmatrix(a,n,n,lda,'A')

      stop
 99   format('Example: CHER2 for the Hermitian rank-2 update')
 100  format('#### args: n=',i1,', lda=',i1,', incx=',i1,
     &       ', incy=',i1)
 101  format(11x,'alpha=',f3.1)
 102  format('The result of CHER2: ')
      end
