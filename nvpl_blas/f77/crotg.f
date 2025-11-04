!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CROTG
!
!******************************************************************************
      program   CROTG_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      real            c
      complex         a, b, s
      data            a /2.3/, b /4.6/, c /0.0/, s /0.0/


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
*      Call CROTG subroutine
      call CROTG(a, b, c, s)
*
      print*
      print 101
      print 102, a, b, c, s

      stop
 99   format('Example: CROTG for computing the parameters for a given',
     &       ' rotation')
 100  format('#### args: a=(',f5.3,', ',f5.3,'), b=(',f5.3,', ',f5.3,
     &       '), c=',f5.3,', s=(',f5.3,', ',f5.3,')')
 101  format('The result of CROTG: ')
 102  format('a=(',f5.3,', ',f5.3,'), b=(',f5.3,', ',f5.3,'), c=',f5.3,
     &       ', s=(',f5.3,', ',f5.3,')')
      end
