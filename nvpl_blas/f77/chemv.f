!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CHEMV 
!
!******************************************************************************
      program   CHEMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo
      parameter       (uplo='U')
      integer         m, n, incx, incy, i, lda
      parameter       (m=2, n=2, incx=1, incy=1, lda=2)
      complex         alpha, beta
      parameter       (alpha=(1.0, 2.0), beta=(1.0, 2.0))
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      complex         x(xsize), y(ysize), a(lda,n)

*       External Subroutines
      external        print_cvector, fill_cvector, fill_cmatrix,
     &                print_cmatrix
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, lda, incx, incy
      print 101, alpha, beta
      call fill_cvector(x,n,incx)
      call fill_cvector(y,n,incy)
      call fill_cmatrix(a,n,n,lda,uplo,'U')
      call print_cmatrix(a,n,n,lda,'A')
      call print_cvector(x,n,incx,'X')
      call print_cvector(y,n,incy,'Y')
*
*      Call CHEMV subroutine
      call CHEMV(uplo,n,alpha,a,lda,x,incx,beta,y,incy)
*
      print*
      print 102
      call print_cvector(y,n,incy,'Y')

      stop
 99   format('Example: CHEMV for a matrix-vector product using'
     &       ' a Hermitian matrix')
 100  format('#### args: n=',i1,', lda=',i1,', incx=',i1,
     &       ', incy=',i1)
 101  format(11x,'alpha=(',f3.1,', ',f3.1,'),'
     &       ' beta=(',f3.1,', ',f3.1,')')
 102  format('The result of CHEMV: ')
      end
