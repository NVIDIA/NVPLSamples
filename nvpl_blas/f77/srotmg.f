!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SROTMG 
!
!******************************************************************************
      program   SROTMG_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      integer         i
      real            d1, d2, b1, b2, P(5)
      data            d1 /2.3/, d2 /4.6/, b1 /1.0/, b2 /2.0/,
     &                (P(i), i=1,5) /5*0.0/
      

*       External Subroutines
      external        print_svector 
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
*      Call SROTMG subroutine
      call SROTMG(d1, d2, b1, b2, P)
*
      print*
      print 101
      print 102, d1, d2, b1, b2
      call print_svector(P,5,1,'P')

      stop
 99   format('Example: SROTMG for computing the parameters for a Givens'
     & ' rotation')
 100  format('#### args: d1=',f5.3,', d2=',f5.3,', b1=',f5.3,', '
     &       'b2=',f5.3)
 101  format('The result of SROTMG: ')
 102  format('d1=',f5.3,', d2=',f5.3,', b1=',f5.3,', b2=',f5.3)
      end
