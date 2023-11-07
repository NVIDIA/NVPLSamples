!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZHBMV 
!
!******************************************************************************
      program   ZHBMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo
      parameter       (uplo='U')
      integer         n, k, lda, kl, ku
      integer         incx, incy
      parameter       (n=4, k=2, incx=1, incy=1, lda=k+1)
      integer         asize
      parameter       (asize=n*n)
      double complex  alpha, beta
      parameter       (alpha=(1.0,2.0), beta=(1.0,2.0))
      double complex  a(lda,n)
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      double complex  x(xsize), y(ysize)

*       External Subroutines
      external        print_zbandmatrix, fill_zbandmatrix,
     &                print_zvector, fill_zvector
*
*      Executable Statements
*
      print*
      print 99, 'Example: ZHBMV for the Hermitian banded ',
     &          'matrix-vector multiplication'
*
*       Print input data
      if (uplo .eq. 'U') then
          kl=0
          ku=k
      else
          kl=k
          ku=0
      endif
      print*
      print 100, n, k, lda, incx, incy, uplo
      print 101, alpha, beta
      call fill_zbandmatrix(a,kl,ku,n,n,lda)
      call fill_zvector(x,n,incx)
      call fill_zvector(y,n,incy)
      call print_zbandmatrix(a,kl,ku,n,n,lda,'A')
      call print_zvector(x,n,incx,'X')
      call print_zvector(y,n,incy,'Y')
*
*      Call ZHBMV subroutine
      call ZHBMV(uplo,n,k,alpha,a,lda,x,incx,beta,y,incy)
*
      print*
      print 102, 'After zhbmv operation:'
      call print_zvector(y,n,incy,'Y')

      stop
 99   format(A,A)
 100  format('#### args: n=',i1,', k=',i1,', lda=',i1,
     &       ', incx=',i1,', incy=',i1,', uplo=',a1)
 101  format(11x,'alpha=(',f4.2,', ',f4.2,')',
     &       ', beta=(',f4.2,', ',f4.2,')')
 102  format(A)
      end
