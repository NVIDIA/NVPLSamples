
      double precision function get_dnan()
      implicit none

      include "nvpl_blas_f77_blas.fi"

      integer(8) nan_bits_data, nan_bits
      data nan_bits_data/Z'7FFFFFFFFFFFFFFF'/
      nan_bits = ibset(int(nan_bits_data,8),bit_size(1_8)-1)
      get_dnan = transfer(nan_bits,1._8)
      end function get_dnan

      real function get_snan()
      implicit none

      include "nvpl_blas_f77_blas.fi"

      integer(4) nan_bits_data, nan_bits
      data nan_bits_data/Z'7FFFFFFF'/
      nan_bits = ibset(int(nan_bits_data,4),bit_size(1_4)-1)
      get_snan = transfer(nan_bits,1._4)
      end function get_snan

      subroutine print_svector(x, n, incx, name)
      integer       n, incx
      character*2   name
      real          x(*)

*       Intrinsic Functions
      intrinsic        abs

      print 100, name
      print 101, (x(i),i=1,1+(n-1)*abs(incx))

 100  format('Vector ',a2,':')
 101  format(9x,10(f10.6,2x))
      return
      end

      subroutine print_dvector(x, n, incx, name)
      integer           n, incx
      character*2       name
      double precision  x(*)

*       Intrinsic Functions
      intrinsic         abs

      print 100, name
      print 101, (x(i),i=1,1+(n-1)*abs(incx))

 100  format('Vector ',a2,':')
 101  format(9x,10(f10.6,2x))
      return
      end

      subroutine print_cvector(x, n, incx, name)
      integer       n, incx
      character*2   name
      complex       x(*)

*       Intrinsic Functions
      intrinsic        abs

      print 100, name
      print 101, (x(i),i=1,1+(n-1)*abs(incx))

 100  format('Vector ',a2,':')
 101  format(9x,10(f10.6,',',f10.6,3x))
      return
      end

      subroutine print_zvector(x, n, incx, name)
      integer       n, incx
      character*2   name
      complex*16    x(*)

*       Intrinsic Functions
      intrinsic        abs

      print 100, name
      print 101, (x(i),i=1,1+(n-1)*abs(incx))

 100  format('Vector ',a2,':')
 101  format(7x,10(f10.6,',',f10.6,3x))
      return
      end

      subroutine fill_svector(x, n, incx)
      integer       n, incx, i
      real          x(*)

*       Intrinsic Functions
      intrinsic        abs

      do i=1,n
          x(1+(i-1)*abs(incx))=1.0/1024.0*mod(91+i,1023)-0.5
      end do
      return
      end

      subroutine fill_dvector(x, n, incx)
      integer           n, incx, i
      double precision  x(*)

*       Intrinsic Functions
      intrinsic         abs

      do i=1,n
          x(1+(i-1)*abs(incx))=1.0/1024.0*mod(91+i,1023)-0.5
      end do
      return
      end

      subroutine fill_cvector(x, n, incx)
      integer       n, incx, i
      complex       x(*)

*       Intrinsic Functions
      intrinsic        abs

      do i=1,n
          x(1+(i-1)*abs(incx))=cmplx(1.0/1024.0*mod(91+i,1023)-0.5,
     &                               1.0/1024.0*mod(479+i,1023)-0.5)
      end do
      return
      end

      subroutine fill_zvector(x, n, incx)
      integer       n, incx, i
      complex*16    x(*)

*       Intrinsic Functions
      intrinsic        abs

      do i=1,n
          x(1+(i-1)*abs(incx))=dcmplx(1.0/1024.0*mod(91+i,1023)-0.5,
     &                                1.0/1024.0*mod(479+i,1023)-0.5)
      end do
      return
      end

      subroutine print_smatrix(a, m, n, lda, name)
      integer       m, n, lda
      character*1   name
      real          a(lda,*)
      integer       i, j

      print 100, name
      do i=1,m
          print 110, (a(i,j),j=1,n)
      end do
 100  format('Matrix ',a1,':')
 110  format(9x,10(f10.6,2x))
      return
      end

      subroutine print_dmatrix(a, m, n, lda, name)
      integer           m, n, lda
      character*1       name
      double precision  a(lda,*)
      integer           i, j

      print 100, name
      do i=1,m
          print 110, (a(i,j),j=1,n)
      end do
 100  format('Matrix ',a1,':')
 110  format(9x,10(f10.6,2x))
      return
      end

      subroutine print_cmatrix(a, m, n, lda, name)
      integer           m, n, lda
      character*1       name
      complex           a(lda,*)
      integer           i, j

      print 100, name
      do i=1,m
          print 110, (a(i,j),j=1,n)
      end do
 100  format('Matrix ',a1,':')
 110  format(9x,10(f10.6,',',f10.6,3x))
      return
      end

      subroutine print_zmatrix(a, m, n, lda, name)
      integer           m, n, lda
      character*1       name
      double complex    a(lda,*)
      integer           i, j

      print 100, name
      do i=1,m
          print 110, (a(i,j),j=1,n)
      end do
 100  format('Matrix ',a1,':')
 110  format(9x,10(f10.6,',',f10.6,3x))
      return
      end

      subroutine fill_smatrix(a, m, n, ld, uplo, diag)
      integer       m, n, ld
      character*1   uplo, diag
      real          a(ld,*)
      integer       i, j
      real          get_snan

      if (uplo .eq. 'F') then
          do i=1,m
              do j=1,n
                  a(i,j)=1.0/1024.0*mod(91+j,1023)-0.5
              end do
          end do
      else
          do i=1,m
              do j=1,n
                  if (((uplo .eq. 'L') .and. (j .lt. i)) .or.
     &               ((uplo .eq. 'U') .and. (i .lt. j)) .or.
     &               ((diag .eq. 'N') .and. (i .eq. j))) then
                      a(i,j)=1.0/1024.0*mod(91+j,1023)-0.5
                  else if (i .eq. j) then
                      a(i,j)=1.0
                  else
                      a(i,j)=get_snan()
                  endif
              end do
          end do
      endif
      return
      end

      subroutine fill_dmatrix(a, m, n, ld, uplo, diag)
      integer          m, n, ld
      character*1      uplo, diag
      double precision a(ld,*)
      integer          i, j
      double precision get_dnan

      if (uplo .eq. 'F') then
          do i=1,m
              do j=1,n
                  a(i,j)=1.0/1024.0*mod(91+j,1023)-0.5
              end do
          end do
      else
          do i=1,m
              do j=1,n
                  if (((uplo .eq. 'L') .and. (j .lt. i)) .or.
     &               ((uplo .eq. 'U') .and. (i .lt. j)) .or.
     &               ((diag .eq. 'N') .and. (i .eq. j))) then
                      a(i,j)=1.0/1024.0*mod(91+j,1023)-0.5
                  else if (i .eq. j) then
                      a(i,j)=1.0
                  else
                      a(i,j)=get_dnan()
                  endif
              end do
          end do
      endif
      return
      end

      subroutine fill_cmatrix(a, m, n, ld, uplo, diag)
      integer       m, n, ld
      character*1   uplo, diag
      complex       a(ld,*)
      integer       i, j
      real          get_snan

      if (uplo .eq. 'F') then
          do i=1,m
              do j=1,n
                  a(i,j)=cmplx(1.0/1024.0*mod(91+j,1023)-0.5,
     &                         1.0/1024.0*mod(479+j,1023)-0.5)
              end do
          end do
      else
          do i=1,m
              do j=1,n
                  if (((uplo .eq. 'L') .and. (j .lt. i)) .or.
     &               ((uplo .eq. 'U') .and. (i .lt. j)) .or.
     &               ((diag .eq. 'N') .and. (i .eq. j))) then
                      a(i,j)=cmplx(1.0/1024.0*mod(91+j,1023)-0.5,
     &                             1.0/1024.0*mod(479+j,1023)-0.5)
                  else if (i .eq. j) then
                      a(i,j)=cmplx(1.0,0.0)
                  else
                      a(i,j)=cmplx(get_snan(),get_snan())
                  endif
              end do
          end do
      endif
      return
      end

      subroutine fill_zmatrix(a, m, n, ld, uplo, diag)
      integer          m, n, ld
      character*1      uplo, diag
      double complex   a(ld,*)
      integer          i, j
      double precision get_dnan

      if (uplo .eq. 'F') then
          do i=1,m
              do j=1,n
                  a(i,j)=dcmplx(1.0/1024.0*mod(91+j,1023)-0.5,
     &                          1.0/1024.0*mod(479+j,1023)-0.5)
              end do
          end do
      else
          do i=1,m
              do j=1,n
                  if (((uplo .eq. 'L') .and. (j .lt. i)) .or.
     &               ((uplo .eq. 'U') .and. (i .lt. j)) .or.
     &               ((diag .eq. 'N') .and. (i .eq. j))) then
                      a(i,j)=dcmplx(1.0/1024.0*mod(91+j,1023)-0.5,
     &                              1.0/1024.0*mod(479+j,1023)-0.5)
                  else if (i .eq. j) then
                      a(i,j)=dcmplx(1.0,0.0)
                  else
                      a(i,j)=dcmplx(get_dnan(),get_dnan())
                  endif
              end do
          end do
      endif
      return
      end

      subroutine fill_sbandmatrix(a, kl, ku, m, n, lda)
      integer       kl, ku, m, n, lda
      character*1   name
      real          a(lda,*)
      integer       i, j, k, k1, ku1, kl1

      if (ku.ge.n) then
         ku1 = n-1
      else
         ku1 = ku
      end if
      k = ku1+1
      do i=1, ku1+1
         if ((ku1-i+m+1).ge.n) then
            k1 = n
         else
            k1 = ku1-i+m+1
         end if
         do j=ku1+2-i,k1
             a(ku-ku1+i,j) = ku-ku1+i
         end do
         k = k-1
      end do
      if (kl.ge.m) then
         kl1 = m-1
      else
         kl1 = kl
      end if
      do i=ku1+2, ku1+kl1+1
         if ((m+ku1-i+1).ge.n) then
            k1 = n
         else
            k1 = m+ku1-i+1
         end if
         do j=1,k1
            a(ku-ku1+i,j) = ku-ku1+i
         end do
      end do
      return
      end

      subroutine fill_dbandmatrix(a, kl, ku, m, n, lda)
      integer           kl, ku, m, n, lda
      character*1       name
      double precision  a(lda,*)
      integer           i, j, k, k1, ku1, kl1

      if (ku.ge.n) then
         ku1 = n-1
      else
         ku1 = ku
      end if
      k = ku1+1
      do i=1, ku1+1
         if ((ku1-i+m+1).ge.n) then
            k1 = n
         else
            k1 = ku1-i+m+1
         end if
         do j=ku1+2-i,k1
             a(ku-ku1+i,j) = ku-ku1+i
         end do
         k = k-1
      end do
      if (kl.ge.m) then
         kl1 = m-1
      else
         kl1 = kl
      end if
      do i=ku1+2, ku1+kl1+1
         if ((m+ku1-i+1).ge.n) then
            k1 = n
         else
            k1 = m+ku1-i+1
         end if
         do j=1,k1
            a(ku-ku1+i,j) = ku-ku1+i
         end do
      end do
      return
      end

      subroutine fill_cbandmatrix(a, kl, ku, m, n, lda)
      integer       kl, ku, m, n, lda
      character*1   name
      complex       a(lda,*)
      integer       i, j, k, k1, ku1, kl1

      if (ku.ge.n) then
         ku1 = n-1
      else
         ku1 = ku
      end if
      k = ku1+1
      do i=1, ku1+1
         if ((ku1-i+m+1).ge.n) then
            k1 = n
         else
            k1 = ku1-i+m+1
         end if
         do j=ku1+2-i,k1
             a(ku-ku1+i,j) = cmplx(ku-ku1+i, ku-ku1+i)
         end do
         k = k-1
      end do
      if (kl.ge.m) then
         kl1 = m-1
      else
         kl1 = kl
      end if
      do i=ku1+2, ku1+kl1+1
         if ((m+ku1-i+1).ge.n) then
            k1 = n
         else
            k1 = m+ku1-i+1
         end if
         do j=1,k1
            a(ku-ku1+i,j) = cmplx(ku-ku1+i, ku-ku1+i)
         end do
      end do
      return
      end

      subroutine fill_zbandmatrix(a, kl, ku, m, n, lda)
      integer         kl, ku, m, n, lda
      character*1     name
      double complex  a(lda,*)
      integer         i, j, k, k1, ku1, kl1

      if (ku.ge.n) then
         ku1 = n-1
      else
         ku1 = ku
      end if
      k = ku1+1
      do i=1, ku1+1
         if ((ku1-i+m+1).ge.n) then
            k1 = n
         else
            k1 = ku1-i+m+1
         end if
         do j=ku1+2-i,k1
             a(ku-ku1+i,j) = dcmplx(ku-ku1+i, ku-ku1+i)
         end do
         k = k-1
      end do
      if (kl.ge.m) then
         kl1 = m-1
      else
         kl1 = kl
      end if
      do i=ku1+2, ku1+kl1+1
         if ((m+ku1-i+1).ge.n) then
            k1 = n
         else
            k1 = m+ku1-i+1
         end if
         do j=1,k1
            a(ku-ku1+i,j) = dcmplx(ku-ku1+i, ku-ku1+i)
         end do
      end do
      return
      end

      subroutine print_sbandmatrix(a, kl, ku, m, n, lda, name)
      integer       kl, ku, m, n, lda
      character*1   name
      real          a(lda,*)
      integer       i, j, k, k1, ku1, kl1

      if (ku.ge.n) then
         ku1 = n-1
      else
         ku1 = ku
      end if
      print*, name, '='
      k = ku1+1
      do i=1, ku1+1
         if ((ku1-i+m+1).ge.n) then
            k1 = n
         else
            k1 = ku1-i+m+1
         end if
         goto (10,20,30,40,50) k
  10     print 110, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  20     print 120, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  30     print 130, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  40     print 140, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  50     print 150, (a(ku-ku1+i,j),j=ku1+2-i,k1)
  60     continue
         k = k-1
      end do
      if (kl.ge.m) then
         kl1 = m-1
      else
         kl1 = kl
      end if
      do i=ku1+2, ku1+kl1+1
         if ((m+ku1-i+1).ge.n) then
            k1 = n
         else
            k1 = m+ku1-i+1
         end if
         print 110, (a(ku-ku1+i,j),j=1,k1)
      end do

 110  format(7x,2x,10(f6.2,2x))
 120  format(7x,10x,10(f6.2,2x))
 130  format(7x,18x,10(f6.2,2x))
 140  format(7x,26x,10(f6.2,2x))
 150  format(7x,34x,10(f6.2,2x))
      return
      end

      subroutine print_dbandmatrix(a, kl, ku, m, n, lda, name)
      integer          kl, ku, m, n, lda
      character*1      name
      double precision a(lda,*)
      integer          i, j, k, k1, ku1, kl1

      if (ku.ge.n) then
          ku1 = n-1
      else
          ku1 = ku
      end if
      print*, name, '='
      k = ku1+1
      do i=1, ku1+1
         if ((ku1-i+m+1).ge.n) then
            k1 = n
         else
            k1 = ku1-i+m+1
         end if
         goto (10,20,30,40,50) k
  10     print 110, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  20     print 120, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  30     print 130, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  40     print 140, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  50     print 150, (a(ku-ku1+i,j),j=ku1+2-i,k1)
  60     continue
         k = k-1
      end do
      if (kl.ge.m) then
         kl1 = m-1
      else
         kl1 = kl
      end if
      do i=ku1+2, ku1+kl1+1
         if ((m+ku1-i+1).ge.n) then
            k1 = n
         else
            k1 = m+ku1-i+1
         end if
         print 110, (a(ku-ku1+i,j),j=1,k1)
      end do

 110  format(7x,2x,10(f8.3,2x))
 120  format(7x,12x,10(f8.3,2x))
 130  format(7x,22x,10(f8.3,2x))
 140  format(7x,32x,10(f8.3,2x))
 150  format(7x,42x,10(f8.3,2x))
      return
      end

      subroutine print_cbandmatrix(a, kl, ku, m, n, lda, name)
      integer       kl, ku, m, n, lda
      character*1   name
      complex       a(lda,*)
      integer       i, j, k, k1, ku1, kl1

      if (ku.ge.n) then
         ku1 = n-1
      else
         ku1 = ku
      end if
      print*, name, '='
      k = ku1+1
      do i=1, ku1+1
         if ((ku1-i+m+1).ge.n) then
            k1 = n
         else
            k1 = ku1-i+m+1
         end if
         goto (10,20,30,40,50) k
  10     print 110, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  20     print 120, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  30     print 130, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  40     print 140, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  50     print 150, (a(ku-ku1+i,j),j=ku1+2-i,k1)
  60     continue
         k = k-1
      end do
      if (kl.ge.m) then
          kl1 = m-1
      else
          kl1 = kl
      end if
      do i=ku1+2, ku1+kl1+1
         if ((m+ku1-i+1).ge.n) then
            k1 = n
         else
            k1 = m+ku1-i+1
         end if
         print 110, (a(ku-ku1+i,j),j=1,k1)
      end do

 110  format(7x,2x,10(f6.2,',',f6.2,3x))
 120  format(7x,18x,10(f6.2,',',f6.2,3x))
 130  format(7x,34x,10(f6.2,',',f6.2,3x))
 140  format(7x,50x,10(f6.2,',',f6.2,3x))
 150  format(7x,66x,10(f6.2,',',f6.2,3x))
      return
      end

      subroutine print_zbandmatrix(a, kl, ku, m, n, lda, name)
      integer         kl, ku, m, n, lda
      character*1     name
      double complex  a(lda,*)
      integer         i, j, k, k1, ku1, kl1

      if (ku.ge.n) then
          ku1 = n-1
      else
          ku1 = ku
      end if
      print*, name, '='
      k = ku1+1
      do i=1, ku1+1
         if ((ku1-i+m+1).ge.n) then
            k1 = n
         else
            k1 = ku1-i+m+1
         end if
         goto (10,20,30,40,50) k
  10     print 110, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  20     print 120, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  30     print 130, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  40     print 140, (a(ku-ku1+i,j),j=ku1+2-i,k1)
         goto 60
  50     print 150, (a(ku-ku1+i,j),j=ku1+2-i,k1)
  60     continue
         k = k-1
      end do
      if (kl.ge.m) then
          kl1 = m-1
      else
          kl1 = kl
      end if
      do i=ku1+2, ku1+kl1+1
         if ((m+ku1-i+1).ge.n) then
            k1 = n
         else
            k1 = m+ku1-i+1
         end if
         print 110, (a(ku-ku1+i,j),j=1,k1)
      end do

 110  format(7x,2x,10(f6.2,',',f6.2,3x))
 120  format(7x,18x,10(f6.2,',',f6.2,3x))
 130  format(7x,34x,10(f6.2,',',f6.2,3x))
 140  format(7x,50x,10(f6.2,',',f6.2,3x))
 150  format(7x,66x,10(f6.2,',',f6.2,3x))
      return
      end
