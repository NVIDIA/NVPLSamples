!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DNRM2 
!
!******************************************************************************
      program   DNRM2_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      integer          n, incx, i
      parameter        (n=5, incx=1)
      double precision result
      integer          xsize
      parameter        (xsize=1+(n-1)*abs(incx))
      double precision x(xsize)
      data             (x(i), i=1,xsize)/1.0,-3.7,4.3,0.0,5.0/

*       External Subroutines
      external         print_svector 
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, incx
      call print_svector(x,n,incx,'X')
*
*      Call DNRM2 subroutine
      result = DNRM2(n,x,incx)
*
      print*
      print 101, result 

      stop
 99   format('Example: DNRM2 for computing the Euclidean norm '
     &       'of the vector')
 100  format('#### args: n=',i1,', incx=',i1)
 101  format('The Euclidean norm of the vector: ', f4.2)
      end
