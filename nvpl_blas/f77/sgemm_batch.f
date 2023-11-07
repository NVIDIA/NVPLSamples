!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     SGEMM_BATCH 
!
!******************************************************************************
      program   SGEMM_BATCH_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

      integer         grp_cnt, i, j, maxmat, l
      parameter       (grp_cnt=2, maxmat=20)
      integer         ig, is, matidx
      integer         rmaxa, cmaxa, rmaxb, cmaxb, rmaxc, cmaxc
      parameter       (rmaxa=5, cmaxa=5, rmaxb=5, cmaxb=5,
     $                 rmaxc=5, cmaxc=5)
      character*1     transa(grp_cnt), transb(grp_cnt)
      data            (transa(i), i=1,grp_cnt)/grp_cnt*'N'/
     &                (transb(i), i=1,grp_cnt)/grp_cnt*'N'/
      integer         m(grp_cnt), n(grp_cnt), k(grp_cnt)
      data            (m(i), i=1,grp_cnt)/grp_cnt*2/
     &                (n(i), i=1,grp_cnt)/grp_cnt*3/
     &                (k(i), i=1,grp_cnt)/grp_cnt*2/
      integer         lda(grp_cnt), ldb(grp_cnt), ldc(grp_cnt)
      data            (lda(i), i=1,grp_cnt)/grp_cnt*2/
     &                (ldb(i), i=1,grp_cnt)/grp_cnt*2/
     &                (ldc(i), i=1,grp_cnt)/grp_cnt*2/
      real            alpha(grp_cnt), beta(grp_cnt)
      data            (alpha(i), i=1,grp_cnt)/grp_cnt*2.0/
     &                (beta(i), i=1,grp_cnt)/grp_cnt*2.0/
      integer         grp_size(grp_cnt)
      data            (grp_size(i), i=1,grp_cnt)/grp_cnt*2/ 
      real            a(rmaxa,cmaxa,maxmat), b(rmaxb,cmaxb,maxmat),
     &                c(rmaxc,cmaxc,maxmat)
      integer*8       a_array(maxmat), b_array(maxmat),
     $                       c_array(maxmat)
*       External Subroutines
      external        print_smatrix, fill_smatrix
*
*      Executable Statements
*
      print*
      print 99
      print*
      matidx = 1
      do ig = 1, grp_cnt
          print 103, ig, grp_size(ig)
          print 100, m(ig), n(ig), k(ig), lda(ig), ldb(ig), ldc(ig),
     &        transa(ig), transb(ig)
          print 101, alpha(ig), beta(ig)
          do is = 1, grp_size(ig)
              a_array(matidx) = LOC(a(1,1,matidx))
              b_array(matidx) = LOC(b(1,1,matidx))
              c_array(matidx) = LOC(c(1,1,matidx))
              call fill_smatrix(a(:,:,matidx),m(ig),k(ig),
     &                          lda(ig),'F','N')
              call fill_smatrix(b(:,:,matidx),k(ig),n(ig),
     &                          ldb(ig),'F','N')
              call fill_smatrix(c(:,:,matidx),m(ig),n(ig),
     &                          ldc(ig),'F','N')
              call print_smatrix(a(:,:,matidx),m(ig),k(ig),lda(ig),'A')
              call print_smatrix(b(:,:,matidx),k(ig),n(ig),ldb(ig),'B')
              call print_smatrix(c(:,:,matidx),m(ig),n(ig),ldc(ig),'C')
              matidx = matidx + 1
          end do
      end do
*
*      Call SGEMM_BATCH subroutine
      call SGEMM_BATCH(transa,transb,m,n,k,alpha,a_array,lda,b_array,
     &                 ldb,beta,c_array,ldc,grp_cnt,grp_size)
*
      print*
      print 102, 'After SGEMM_BATCH operation:'
      print*
      matidx = 1
      do ig = 1, grp_cnt
          print 104, ig
          do is = 1, grp_size(ig)
              print 105, matidx
              call print_smatrix(c(:,:,matidx),m(ig),n(ig),ldc(ig),'C')
              matidx = matidx + 1
          end do
      end do

      stop
 99   format('Example: SGEMM_BATCH for the matrix-matrix multiplication'
     &       ' of a batch of matrices')
 100  format('#### args: m=',i1,', n=',i1,', k=',i1,', lda=',i1,
     &       ', ldb=',i1,', ldc=',i1,', transA=',a1,', transB=',a1)
 101  format(11x,'alpha=',f4.1,'  beta=',f4.1)
 102  format(A)
 103  format('Group=',i1,', Number of matrices in the Group=',i1)
 104  format('Group=',i1)
 105  format('Matrix idx=',i1)
      end
