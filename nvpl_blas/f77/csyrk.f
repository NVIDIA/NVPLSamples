!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CSYRK
!
!******************************************************************************
      program   CSYRK_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1 uplo, trans
      parameter   (uplo='U', trans='N')
      integer     n, k, i, j
      integer     lda, ldc
      parameter   (n=2, k=2,
     &             lda=2, ldc=2)
      integer     asize, csize
      parameter   (asize=n*k, csize=n*n)
      complex     alpha, beta
      parameter   (alpha=(1.0, 2.0), beta=(1.0, 2.0))
      complex     a(lda,k), c(ldc,n)

*       External Subroutines
      external        print_cmatrix, fill_cmatrix
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, k, lda, ldc, uplo, trans
      print 101, alpha, beta
      call fill_cmatrix(a,n,k,lda,'F','N')
      call fill_cmatrix(c,n,n,ldc,uplo,'N')
      call print_cmatrix(a,n,k,lda,'A')
      call print_cmatrix(c,n,n,ldc,'C')
*
*      Call CSYRK subroutine
      call CSYRK(uplo,trans,n,k,alpha,a,lda,beta,c,ldc)
*
      print*
      print 102, 'After CSYRK operation:'
      call print_cmatrix(c,n,n,ldc,'C')

      stop
 99   format('Example: CSYRK for the symmetric rank-k update')
 100  format('#### args: n=',i1,', k=',i1,', lda=',i1,
     &       ', ldc=',i1,', uplo=',a1,', trans=',a1)
 101  format(11x,'alpha=(',f3.1,', ',f3.1,')',
     &       ', beta=(',f3.1,', ',f3.1,')')
 102  format(A)
      end
