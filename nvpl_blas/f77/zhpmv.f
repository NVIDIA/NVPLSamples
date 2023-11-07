!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZHPMV 
!
!******************************************************************************
      program   ZHPMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo
      parameter       (uplo='U')
      integer         m, n, incx, incy, i
      parameter       (m=2, n=2, incx=1, incy=1)
      double complex  alpha, beta
      parameter       (alpha=(1.0,2.0), beta=(1.0, 2.0))
      integer         xsize, ysize, apsize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy),
     &                 apsize=(n*(n+1))/2)
      double complex  x(xsize), y(ysize), ap(apsize)

*       External Subroutines
      external        print_zvector, fill_zvector
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
      call fill_zvector(x,n,incx)
      call fill_zvector(y,n,incy)
      call fill_zvector(ap,apsize,1)
      call print_zvector(ap,apsize,1,'AP')
      call print_zvector(x,n,incx,'X')
      call print_zvector(y,n,incy,'Y')
*
*      Call ZHPMV subroutine
      call ZHPMV(uplo,n,alpha,ap,x,incx,beta,y,incy)
*
      print*
      print 102
      call print_zvector(y,n,incy,'Y')

      stop
 99   format('Example: ZHPMV for a matrix-vector product using a'
     &       ' Hermitian packed matrix')
 100  format('#### args: n=',i1,', incx=',i1,', incy=',i1,
     &       ', uplo=',a1)
 101  format(11x,'alpha=(',f4.2,', ',f4.2,'),'
     &       ' beta=(',f4.2,', ',f4.2,')')
 102  format('The result of ZHPMV: ')
      end
