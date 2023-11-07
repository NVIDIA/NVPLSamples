!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SGEMV 
!
!******************************************************************************
      program   SGEMV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     transa
      parameter       (transa='N')
      integer         m, n, lda
      integer         incx, incy, i, nx, ny, xymax
      parameter       (m=3, n=2, lda=3, incx=1, incy=1, xymax=3)
      integer         asize
      parameter       (asize=m*n)
      real            alpha, beta
      parameter       (alpha=1.0, beta=1.0)
      real            a(lda,n)
      integer         xsize, ysize
      parameter       (xsize=1+(xymax-1)*abs(incx),
     &                 ysize=1+(xymax-1)*abs(incy))
      real            x(xsize), y(ysize)

*       External Subroutines
      external        print_smatrix, fill_smatrix, print_svector,
     &                fill_svector
*
*      Executable Statements
*
      print*
      print 99, 'Example: SGEMV for matrix-vector multiplication'
*
*       Print input data
      print*
      print 100, m, n, lda, incx, incy, transa
      print 101, alpha, beta
      if (transa .eq. 'N') then
          nx=n
          ny=m
      else
          nx=m
          ny=n
      endif
      call fill_smatrix(a,m,n,lda,'F','N')
      call fill_svector(x,nx,incx)
      call fill_svector(y,ny,incy)
      call print_smatrix(a,m,n,lda,'A')
      call print_svector(x,nx,incx,'X')
      call print_svector(y,ny,incy,'Y')
*
*      Call SGEMV subroutine
      call SGEMV(transa,m,n,alpha,a,lda,x,incx,beta,y,incy)
*
      print*
      print 102, 'After sgemv operation:'
      call print_svector(y,ny,incy,'Y')

      stop
 99   format(A)
 100  format('#### args: m=',i1,', n=',i1,', lda=',i1,', incx=',i1,', '
     &       'incy=',i1,', transA=',a1)
 101  format(11x,'alpha=',f4.1,'  beta=',f4.1)
 102  format(A)
      end
