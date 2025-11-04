!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     CGEMM3M
!
!******************************************************************************
      program   CGEMM3M_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1     transa, transb
      parameter       (transa='N', transb='N')
      integer         m, n, k, i, j
      integer         lda, ldb, ldc
      parameter       (m=2, n=2, k=2,
     &                 lda=2, ldb=2, ldc=2)
      integer         asize, bsize, csize
      parameter       (asize=m*k, bsize=k*n, csize=m*n)
      complex         alpha, beta
      parameter       (alpha=(1.0,2.0), beta=(1.0,2.0))
      complex         a(lda,k), b(ldb,n), c(ldc,n)

*       External Subroutines
      external        print_cmatrix, fill_cmatrix
*
*      Executable Statements
*
      print*
      print 99, 'Example: CGEMM3M for matrix-matrix multiplication ' //
     &          'using Gauss complexity reduction algorithm'
*
*       Print input data
      print*
      print 100, m, n, k, lda, ldb, ldc, transa, transb
      print 101, alpha, beta
      call fill_cmatrix(a,m,k,lda,'F','N')
      call fill_cmatrix(b,k,n,ldb,'F','N')
      call fill_cmatrix(c,m,n,ldc,'F','N')
      call print_cmatrix(a,m,k,lda,'A')
      call print_cmatrix(b,k,n,ldb,'B')
      call print_cmatrix(c,m,n,ldc,'C')
*
*      Call CGEMM3M subroutine
      call CGEMM3M(transa,transb,m,n,k,alpha,a,lda,b,ldb,beta,c,ldc)
*
      print*
      print 102, 'After gemm3m operation:'
      call print_cmatrix(c,m,n,ldc,'C')

      stop
 99   format(A)
 100  format('#### args: m=',i1,', n=',i1,', k=',i1,', lda=',i1,
     &       ', ldb=',i1,', ldc=',i1,', transA=',a1,', transB=',a1)
 101  format(11x,'alpha=(',f3.1,', ',f3.1,')',
     &       ', beta=(',f3.1,', ',f3.1,')')
 102  format(A)
      end
