!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DROTG
!
!******************************************************************************
      program   DROTG_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      double precision a, b, c, s
      data             a /2.3/, b /4.6/, c /0.0/, s /0.0/


*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, a, b, c, s
*
*      Call DROTG subroutine
      call DROTG(a, b, c, s)
*
      print*
      print 101
      print 102, a, b, c, s

      stop
 99   format('Example: DROTG for computing the parameters for a given',
     & ' rotation')
 100  format('#### args: a=',f5.3,', b=',f5.3,', c=',f5.3,
     &       ', s=',f5.3)
 101  format('The result of DROTG: ')
 102  format('a=',f5.3,', b=',f5.3,', c=',f5.3,', s=',f5.3)
      end
