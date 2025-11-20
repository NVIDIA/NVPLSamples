!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     DGEMMT
!
!******************************************************************************
      program   DGEMMT_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1     transa, transb, uplo
      parameter       (transa='T', transb='N', uplo='L')
      integer         n, k, i, j
      integer         lda, ldb, ldc
      parameter       (n=2, k=2,
     &                 lda=2, ldb=2, ldc=2)
      double precision alpha, beta
      parameter       (alpha=0.6d0, beta=0.4d0)
      double precision a(lda,k), b(ldb,n), c(ldc,n)
*       External Subroutines
      external        print_dmatrix, fill_dmatrix
*
*      Executable Statements
*
      print*
      print 99, 'Example: DGEMMT performs of the matrix-matrix' //
     &          ' operations with general matrices and add the' //
     &          ' result to the upper or lower part of a' //
     &          ' scalar-matrix product.'
*
*       Print input data
      print*
      print 100, n, k, lda, ldb, ldc, transa, transb, uplo
      print 101, alpha, beta
      call fill_dmatrix(a,n,k,lda,'F',transa)
      call fill_dmatrix(b,k,n,ldb,'F',transb)
      call fill_dmatrix(c,n,n,ldc,uplo,'N')
      call print_dmatrix(a,n,k,lda,'A')
      call print_dmatrix(b,k,n,ldb,'B')
      call print_dmatrix(c,n,n,ldc,'C')
*
*      Call DGEMMT subroutine
      call DGEMMT(uplo,transa,transb,n,k,alpha,a,lda,b,ldb,beta,c,ldc)
*
      print*
      print 102, 'After DGEMMT operation:'
      call print_dmatrix(c,n,n,ldc,'C')

      stop
 99   format(A)
 100  format('#### args: n=',i1,', k=',i1,', lda=',i1,
     &       ', ldb=',i1,', ldc=',i1,', transa=',a1,', transb=',a1,
     &       ', uplo=',a1)
 101  format(11x,'alpha=',f3.1,', beta=',f3.1)
 102  format(A)
      end
