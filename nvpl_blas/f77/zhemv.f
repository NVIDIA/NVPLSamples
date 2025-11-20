!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZHEMV
!
!******************************************************************************
      program   ZHEMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo
      parameter       (uplo='U')
      integer         m, n, incx, incy, i, lda
      parameter       (m=2, n=2, incx=1, incy=1, lda=2)
      double complex  alpha, beta
      parameter       (alpha=(1.0, 2.0), beta=(1.0, 2.0))
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      double complex  x(xsize), y(ysize), a(lda,n)

*       External Subroutines
      external        print_zvector, fill_zvector, fill_zmatrix,
     &                print_zmatrix
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
      call fill_zvector(x,n,incx)
      call fill_zvector(y,n,incy)
      call fill_zmatrix(a,n,n,lda,uplo,'U')
      call print_zmatrix(a,n,n,lda,'A')
      call print_zvector(x,n,incx,'X')
      call print_zvector(y,n,incy,'Y')
*
*      Call ZHEMV subroutine
      call ZHEMV(uplo,n,alpha,a,lda,x,incx,beta,y,incy)
*
      print*
      print 102
      call print_zvector(y,n,incy,'Y')

      stop
 99   format('Example: ZHEMV for a matrix-vector product using',
     &       ' a Hermitian matrix')
 100  format('#### args: n=',i1,', lda=',i1,', incx=',i1,
     &       ', incy=',i1)
 101  format(11x,'alpha=(',f4.2,', ',f4.2,')',
     &       ', beta=(',f4.2,', ',f4.2,')')
 102  format('The result of ZHEMV: ')
      end
