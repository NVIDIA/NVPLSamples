!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SSYR2K 
!
!******************************************************************************
      program   SSYR2K_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      character*1     trans, uplo
      parameter       (trans='N', uplo='U')
      integer         n, k, i, j
      integer         lda, ldb, ldc
      parameter       (n=2, k=2,
     &                 lda=2, ldb=2, ldc=2)
      integer         asize, bsize, csize
      parameter       (asize=n*k, bsize=n*k, csize=n*n)
      real            alpha, beta
      parameter       (alpha=1.0, beta=1.0)
      real            a(lda,k), b(ldb,n), c(ldc,n)

*       External Subroutines
      external        print_smatrix, fill_smatrix
*
*      Executable Statements
*
      print*
      print 99, 'Example: SSYR2K for the symmetric rank-2k update'
*
*       Print input data
      print*
      print 100, n, k, lda, ldb, ldc, trans, uplo
      print 101, alpha, beta
      call fill_smatrix(a,n,k,lda,'F','N')
      call fill_smatrix(b,n,k,ldb,'F','N')
      call fill_smatrix(c,n,n,ldc,uplo,'N')
      call print_smatrix(a,n,k,lda,'A')
      call print_smatrix(b,n,k,ldb,'B')
      call print_smatrix(c,n,n,ldc,'C')
*
*      Call SSYR2K subroutine
      call SSYR2K(uplo,trans,n,k,alpha,a,lda,b,ldb,beta,c,ldc)
*
      print*
      print 102, 'After SSYR2K operation:'
      call print_smatrix(c,n,n,ldc,'C')

      stop
 99   format(A)
 100  format('#### args: n=',i1,', k=',i1,', lda=',i1,
     &       ', ldb=',i1,', ldc=',i1,', trans=',a1,', uplo=',a1)
 101  format(11x,'alpha=',f4.1,'  beta=',f4.1)
 102  format(A)
      end
