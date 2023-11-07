!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DSBMV 
!
!******************************************************************************
      program   DSBMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1      uplo
      parameter        (uplo='U')
      integer          n, k, lda, kl, ku
      integer          incx, incy
      parameter        (n=4, k=2, incx=1, incy=1, lda=k+1)
      integer          asize
      parameter        (asize=n*n)
      double precision alpha, beta
      parameter        (alpha=1.0, beta=1.0)
      double precision a(lda,n)
      integer          xsize, ysize
      parameter        (xsize=1+(n-1)*abs(incx),
     &                  ysize=1+(n-1)*abs(incy))
      double precision x(xsize), y(ysize)

*       External Subroutines
      external        print_dbandmatrix, fill_dbandmatrix,
     &                print_dvector, fill_dvector
*
*      Executable Statements
*
      print*
      print 99, 'Example: DSBMV for the symmetric banded ',
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
      call fill_dbandmatrix(a,kl,ku,n,n,lda)
      call fill_dvector(x,n,incx)
      call fill_dvector(y,n,incy)
      call print_dbandmatrix(a,kl,ku,n,n,lda,'A')
      call print_dvector(x,n,incx,'X')
      call print_dvector(y,n,incy,'Y')
*
*      Call DSBMV subroutine
      call DSBMV(uplo,n,k,alpha,a,lda,x,incx,beta,y,incy)
*
      print*
      print 102, 'After dsbmv operation:'
      call print_dvector(y,n,incy,'Y')

      stop
 99   format(A,A)
 100  format('#### args: n=',i1,', k=',i1,', lda=',i1,
     &       ', incx=',i1,', incy=',i1,', uplo=',a1)
 101  format(11x,'alpha=',f4.1,'  beta=',f4.1)
 102  format(A)
      end
