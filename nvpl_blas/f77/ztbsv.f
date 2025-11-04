!******************************************************************************
! Content:
!     This example demonstrates use of API as below:
!     ZTBSV
!
!******************************************************************************
      program   ZTBSV_MAIN
*
      implicit none

      include "nvpl_blas_f77_blas.fi"

*       Intrinsic Functions
      intrinsic  abs

      character*1     uplo, transa, diag
      parameter       (uplo='U', transa='N', diag='N')
      integer         n, k, lda, kl, ku
      integer         incx, i
      parameter       (n=4, k=2, lda=k+1, incx=1)
      double complex  a(lda,n)
      integer         xsize
      parameter       (xsize=1+(n-1)*abs(incx))
      double complex  x(xsize)

*       External Subroutines
      external        print_zbandmatrix, fill_zbandmatrix,
     &                print_zvector, fill_zvector
*
*      Executable Statements
*
      print*
      print 99
*
*       Print input data
      print*
      print 100, n, k, lda, incx, uplo, transa, diag

      if (uplo .eq. 'U') then
          kl=0
          ku=k
      else
          kl=k
          ku=0
      end if

      call fill_zbandmatrix(a,kl,ku,n,n,lda)
      call fill_zvector(x,n,incx)
      call print_zbandmatrix(a,kl,ku,n,n,lda,'A')
      call print_zvector(x,n,incx,'X')
*
*      Call ZTBSV subroutine
      call ZTBSV(uplo,transa,diag,n,k,a,lda,x,incx)
*
      print*
      print 102, 'After ZTBSV operation:'
      call print_zvector(x,n,incx,'X')

      stop
 99   format('Example: ZTBSV for solving the triangular banded linear ',
     &       'system with a single right-hand-side')
 100  format('#### args: n=',i1,', k=',i1,', lda=',i1,', incx=',i1,
     &       ', uplo=',a1,', transa=',a1,', diag=',a1)
 102  format(A)
      end
