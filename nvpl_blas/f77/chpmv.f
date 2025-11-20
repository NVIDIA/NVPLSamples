!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CHPMV
!
!******************************************************************************
      program   CHPMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo
      parameter       (uplo='U')
      integer         m, n, incx, incy, i
      parameter       (m=2, n=2, incx=1, incy=1)
      complex         alpha, beta
      parameter       (alpha=(1.0,2.0), beta=(1.0, 2.0))
      integer         xsize, ysize, apsize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy),
     &                 apsize=(n*(n+1))/2)
      complex         x(xsize), y(ysize), ap(apsize)

*       External Subroutines
      external        print_cvector, fill_cvector
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, incx, incy, uplo
      print 101, alpha, beta
      call fill_cvector(x,n,incx)
      call fill_cvector(y,n,incy)
      call fill_cvector(ap,apsize,1)
      call print_cvector(ap,apsize,1,'AP')
      call print_cvector(x,n,incx,'X')
      call print_cvector(y,n,incy,'Y')
*
*      Call CHPMV subroutine
      call CHPMV(uplo,n,alpha,ap,x,incx,beta,y,incy)
*
      print*
      print 102
      call print_cvector(y,n,incy,'Y')

      stop
 99   format('Example: CHPMV for a matrix-vector product using a',
     &       ' Hermitian packed matrix')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1,
     &       ', uplo=',a1)
 101  format(11x,'alpha=(',f3.1,', ',f3.1,'),',
     &       ' beta=(',f3.1,', ',f3.1,')')
 102  format('The result of CHPMV: ')
      end
