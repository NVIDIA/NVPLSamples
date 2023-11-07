!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DROTMG 
!
!******************************************************************************
      program   DROTMG_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      integer          i
      double precision d1, d2, b1, b2, P(5)
      data             d1 /2.3/, d2 /4.6/, b1 /1.0/, b2 /2.0/,
     &                 (P(i), i=1,5) /5*0.0/
      

*       External Subroutines
      external         print_dvector 
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, d1, d2, b1, b2
*
*      Call DROTMG subroutine
      call DROTMG(d1, d2, b1, b2, P)
*
      print*
      print 101
      print 102, d1, d2, b1, b2
      call print_dvector(P,5,1,'P')

      stop
 99   format('Example: DROTMG for computing the parameters for a Givens'
     & ' rotation')
 100  format('#### args: d1=',f5.3,', d2=',f5.3,', b1=',f5.3,', '
     &       'b2=',f5.3)
 101  format('The result of DROTMG: ')
 102  format('d1=',f5.3,', d2=',f5.3,', b1=',f5.3,', b2=',f5.3)
      end
