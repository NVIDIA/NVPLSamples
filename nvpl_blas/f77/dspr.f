!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DSPR 
!
!******************************************************************************
      program   DSPR_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer         n, incx, i
      parameter       (n=2, incx=1)
      double precision alpha
      parameter       (alpha=(1.0, 2.0))
      integer         xsize, apsize
      parameter       (xsize=1+(n-1)*abs(incx),
     &                 apsize=n*(n+1)/2)
      double precision x(xsize), ap(apsize)

*       External Subroutines
      external        print_dvector, fill_dvector
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
      print 100, n, incx
      print 101, alpha
      call fill_dvector(x,n,incx)
      call fill_dvector(ap,apsize,1)
      call print_dvector(x,n,incx,'X')
      call print_dvector(ap,apsize,1,'AP')
*
*      Call DSPR subroutine
      call DSPR(uplo,n,alpha,x,incx,ap)
*
      print*
      print 102
      call print_dvector(ap,apsize,1,'AP')

      stop
 99   format('Example: DSPR for a rank-1 update of a'
     &       'symmetric packed matrix')
 100  format('#### args: n=',i1,', incx=',i1)
 101  format(11x,'alpha=',f4.2)
 102  format('The result of DSPR: ')
      end
