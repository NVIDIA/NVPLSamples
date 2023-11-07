!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZTRSM 
!
!******************************************************************************
      program   ZTRSM_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1     side, uplo, transa, diag
      parameter       (side='L', uplo='U', transa='N', diag='N')
      integer         m, n, i, j
      integer         lda, ldb
      parameter       (m=2, n=2,
     &                 lda=2, ldb=2)
      double complex  alpha
      parameter       (alpha=(1.0,2.0))
      double complex  a(lda,m), b(ldb,n)

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
      print 100, m, n, lda, ldb, side, uplo, transa, diag
      print 101, alpha
      call fill_zmatrix(a,m,m,lda,uplo,diag)
      call fill_zmatrix(b,m,n,ldb,'F','N')
      call print_zmatrix(a,m,m,lda,'A')
      call print_zmatrix(b,m,n,ldb,'B')
*
*      Call ZTRSM subroutine
      call ZTRSM(side,uplo,transa,diag,m,n,alpha,a,lda,b,ldb)
*
      print*
      print 102, 'After ZTRSM operation:'
      call print_zmatrix(b,m,n,ldb,'B')

      stop
 99   format('Example: ZTRSM for solving the triangular linear system'
     &       ' with multiple right-hand-sides')
 100  format('#### args: m=',i1,', n=',i1,', lda=',i1,', ldb=',i1,
     &       ', side=',a1,', uplo=',a1,', transa=',a1,', diag=',a1)
 101  format(11x,'alpha=(',f4.2,', ',f4.2,')')
 102  format(A)
      end
