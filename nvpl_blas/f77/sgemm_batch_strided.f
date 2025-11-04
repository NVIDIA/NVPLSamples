!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SGEMM_BATCH_STRIDED
!
!******************************************************************************
      program   SGEMM_BATCH_STRIDED_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1     transa, transb
      parameter       (transa='N', transb='N')
      integer         m, n, k, i, j, batch_size
      integer         lda, ldb, ldc
      parameter       (m=2, n=2, k=2, batch_size=2,
     &                 lda=2, ldb=2, ldc=2)
      integer         stridea, strideb, stridec
      parameter       (stridea=lda*k, strideb=ldb*n, stridec=ldc*n)
      real            alpha, beta
      parameter       (alpha=1.0, beta=1.0)
      real            a(lda,k,batch_size), b(ldb,n,batch_size),
     &                c(ldc,n,batch_size)

*       External Subroutines
      external        print_smatrix, fill_smatrix
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, m, n, k, lda, ldb, ldc, transa, transb
      print 103, stridea, strideb, stridec, batch_size
      print 101, alpha, beta
      do i = 1, batch_size
          call fill_smatrix(a(:,:,i),m,k,lda,'F','N')
          call fill_smatrix(b(:,:,i),k,n,ldb,'F','N')
          call fill_smatrix(c(:,:,i),m,n,ldc,'F','N')
          call print_smatrix(a(:,:,i),m,k,lda,'A')
          call print_smatrix(b(:,:,i),k,n,ldb,'B')
          call print_smatrix(c(:,:,i),m,n,ldc,'C')
      end do
*
*      Call SGEMM_BATCH_STRIDED subroutine
      call SGEMM_BATCH_STRIDED(transa,transb,m,n,k,alpha,a,lda,
     &     stridea,b,ldb,strideb,beta,c,ldc,stridec,batch_size)
*
      print*
      print 102, 'After SGEMM_BATCH_STRIDED operation:'
      do i = 1, batch_size
          call print_smatrix(c(:,:,i),m,n,ldc,'C')
      end do

      stop
 99   format('Example: SGEMM_BATCH_STRIDED for the matrix-matrix',
     &       ' multiplication of a batch of matrices')
 100  format('#### args: m=',i1,', n=',i1,', k=',i1,', lda=',i1,
     &       ', ldb=',i1,', ldc=',i1,', transA=',a1,', transB=',a1)
 101  format(11x,'alpha=',f4.1,'  beta=',f4.1)
 102  format(A)
 103  format(11x,'stridea=',i1,', strideb=',i1,', stridec=',i1,
     &       ', batch_size=',i1)
      end
