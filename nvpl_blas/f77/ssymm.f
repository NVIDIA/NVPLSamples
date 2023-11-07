!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SSYMM 
!
!******************************************************************************
      program   SSYMM_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1     side, uplo
      parameter       (side='L', uplo='U')
      integer         m, n, i, j
      integer         lda, ldb, ldc
      parameter       (m=2, n=2,
     &                 lda=2, ldb=2, ldc=2)
      integer         asize, bsize, csize
      parameter       (asize=m*m, bsize=m*n, csize=m*n)
      real            alpha, beta
      parameter       (alpha=1.0, beta=1.0)
      real            a(lda,m), b(ldb,n), c(ldc,n)

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
      print 100, m, n, lda, ldb, ldc, side, uplo
      print 101, alpha, beta
      call fill_smatrix(a,m,m,lda,uplo,'N')
      call fill_smatrix(b,m,n,ldb,'F','N')
      call fill_smatrix(c,m,n,ldc,'F','N')
      call print_smatrix(a,m,m,lda,'A')
      call print_smatrix(b,m,n,ldb,'B')
      call print_smatrix(c,m,n,ldc,'C')
*
*      Call SSYMM subroutine
      call SSYMM(side,uplo,m,n,alpha,a,lda,b,ldb,beta,c,ldc)
*
      print*
      print 102, 'After SSYMM operation:'
      call print_smatrix(c,m,n,ldc,'C')

      stop
 99   format('Example: SSYMM for symmetric matrix-matrix'
     &       ' multiplication')
 100  format('#### args: m=',i1,', n=',i1,', lda=',i1,
     &       ', ldb=',i1,', ldc=',i1,', side=',a1,', uplo=',a1)
 101  format(11x,'alpha=',f4.1,'  beta=',f4.1)
 102  format(A)     
      end
