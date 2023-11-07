!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DSYR2 
!
!******************************************************************************
      program   DSYR2_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo
      parameter       (uplo='U')
      integer         m, n, incx, incy, i, lda
      parameter       (m=2, n=2, incx=1, incy=1, lda=2)
      double precision alpha
      parameter       (alpha=1.0)
      integer         xsize, ysize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 ysize=1+(n-1)*abs(incy))
      double precision x(xsize), y(ysize), a(lda,n)

*       External Subroutines
      external        print_dvector, fill_dvector, fill_dmatrix,
     &                print_dmatrix
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
      call fill_dvector(x,n,incx)
      call fill_dvector(y,n,incy)
      call fill_dmatrix(a,n,n,lda,uplo,'N')
      call print_dmatrix(a,n,n,lda,'A')
      call print_dvector(x,n,incx,'X')
      call print_dvector(y,n,incy,'Y')
*
*      Call DSYR2 subroutine
      call DSYR2(uplo,n,alpha,x,incx,y,incy,a,lda)
*
      print*
      print 102
      call print_dmatrix(a,n,n,lda,'A')

      stop
 99   format('Example: DSYR2 for the packed Hermitian rank-2 update')
 100  format('#### args: n=',i1,', lda=',i1,', incx=',i1,
     &       ', incy=',i1)
 101  format(11x,'alpha=',f4.2)
 102  format('The result of DSYR2: ')
      end
