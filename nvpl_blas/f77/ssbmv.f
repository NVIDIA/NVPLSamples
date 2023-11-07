!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SSBMV 
!
!******************************************************************************
      program   SSBMV_MAIN
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
      real            alpha, beta
      parameter       (alpha=1.0, beta=1.0)
      real            a(lda,n)
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      real            x(xsize), y(ysize)

*       External Subroutines
      external        print_sbandmatrix, fill_sbandmatrix,
     &                print_svector, fill_svector
*
*      Executable Statements
*
      print*
      print 99, 'Example: SSBMV for the symmetric banded ',
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
      call fill_sbandmatrix(a,kl,ku,n,n,lda)
      call fill_svector(x,n,incx)
      call fill_svector(y,n,incy)
      call print_sbandmatrix(a,kl,ku,n,n,lda,'A')
      call print_svector(x,n,incx,'X')
      call print_svector(y,n,incy,'Y')
*
*      Call SSBMV subroutine
      call SSBMV(uplo,n,k,alpha,a,lda,x,incx,beta,y,incy)
*
      print*
      print 102, 'After ssbmv operation:'
      call print_svector(y,n,incy,'Y')

      stop
 99   format(A,A)
 100  format('#### args: n=',i1,', k=',i1,', lda=',i1,
     &       ', incx=',i1,', incy=',i1,', uplo=',a1)
 101  format(11x,'alpha=',f4.1,'  beta=',f4.1)
 102  format(A)
      end
