!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZSYRK
!
!******************************************************************************
      program   ZSYRK_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1    uplo, trans
      parameter      (uplo='U', trans='N')
      integer        n, k, i, j
      integer        lda, ldc
      parameter      (n=2, k=2,
     &                lda=2, ldc=2)
      integer        asize, ZSize
      parameter      (asize=n*k, ZSize=n*n)
      double complex alpha, beta
      parameter      (alpha=(1.0, 2.0), beta=(1.0, 2.0))
      double complex a(lda,k), c(ldc,n)

*       External Subroutines
      external        print_zmatrix, fill_zmatrix
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
      call fill_zmatrix(a,n,k,lda,'F','N')
      call fill_zmatrix(c,n,n,ldc,uplo,'N')
      call print_zmatrix(a,n,k,lda,'A')
      call print_zmatrix(c,n,n,ldc,'C')
*
*      Call ZSYRK subroutine
      call ZSYRK(uplo,trans,n,k,alpha,a,lda,beta,c,ldc)
*
      print*
      print 102, 'After ZSYRK operation:'
      call print_zmatrix(c,n,n,ldc,'C')

      stop
 99   format('Example: ZSYRK for the symmetric rank-k update')
 100  format('#### args: n=',i1,', k=',i1,', lda=',i1,
     &       ', ldc=',i1,', uplo=',a1,', trans=',a1)
 101  format(11x,'alpha=(',f4.2,', ',f4.2,')',
     &       ', beta=(',f4.2,', ',f4.2,')')
 102  format(A)
      end
