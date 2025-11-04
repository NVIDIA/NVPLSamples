!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZHER2K
!
!******************************************************************************
      program   ZHER2K_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1      trans, uplo
      parameter        (trans='N', uplo='U')
      integer          n, k, i, j
      integer          lda, ldb, ldc
      parameter        (n=2, k=2,
     &                  lda=2, ldb=2, ldc=2)
      double complex   alpha
      double precision beta
      parameter        (alpha=(1.0,2.0), beta=1.0)
      double complex   a(lda,k), b(ldb,n), c(ldc,n)

*       External Subroutines
      external         print_zmatrix, fill_zmatrix
*
*      Executable Statements
*
      print*
      print 99, 'Example: ZHER2K for the Hermitian rank-2k update'
*
*       Print input data
      print*
      print 100, n, k, lda, ldb, ldc, trans, uplo
      print 101, alpha, beta
      call fill_zmatrix(a,n,k,lda,'F','N')
      call fill_zmatrix(b,n,k,ldb,'F','N')
      call fill_zmatrix(c,n,n,ldc,uplo,'N')

      call print_zmatrix(a,n,k,lda,'A')
      call print_zmatrix(b,n,k,ldb,'B')
      call print_zmatrix(c,n,n,ldc,'C')
*
*      Call ZHER2K subroutine
      call ZHER2K(uplo,trans,n,k,alpha,a,lda,b,ldb,beta,c,ldc)
*
      print*
      print 102, 'After ZHER2K operation:'
      call print_zmatrix(c,n,n,ldc,'C')

      stop
 99   format(A)
 100  format('#### args: n=',i1,', k=',i1,', lda=',i1,
     &       ', ldb=',i1,', ldc=',i1,', trans=',a1,', uplo=',a1)
 101  format(11x,'alpha=(',f4.2,', ',f4.2,')',
     &       ', beta=',f4.2)
 102  format(A)
      end
