      subroutine print_smatrix(a, m, n, lda, name)
      integer       m, n, lda
      character*(*)   name
      real          a(lda,*)
      integer       i, j

      print 100, name
      do i=1,m
          print 110, (a(i,j),j=1,n)
      end do
      print *
 100  format(' ',a,':')
 110  format(10x,10(f10.6,2x))
      return
      end

      subroutine print_dmatrix(a, m, n, lda, name)
      integer           m, n, lda
      character*(*)       name
      double precision  a(lda,*)
      integer           i, j

      print 100, name
      do i=1,m
          print 110, (a(i,j),j=1,n)
      end do
      print *
 100  format(' ',a,':')
 110  format(10x,10(f10.6,2x))
      return
      end

      subroutine print_cmatrix(a, m, n, lda, name)
      integer           m, n, lda
      character*(*)       name
      complex           a(lda,*)
      integer           i, j

      print 100, name
      do i=1,m
          print 110, (a(i,j),j=1,n)
      end do
      print *
 100  format(' ',a,':')
 110  format(10x,10(f10.6,',',f10.6,3x))
      return
      end

      subroutine print_zmatrix(a, m, n, lda, name)
      integer           m, n, lda
      character*(*)       name
      double complex    a(lda,*)
      integer           i, j

      print 100, name
      do i=1,m
          print 110, (a(i,j),j=1,n)
      end do
      print *
 100  format(' ',a,':')
 110  format(10x,10(f10.6,',',f10.6,3x))
      return
      end

      subroutine print_svector(x, n, incx, name)
      integer       n, incx
      character*(*) name
      real          x(*)
      integer       i

      intrinsic        abs

      print 200, name
      print 201, (x(1 + (i-1)*abs(incx)), i = 1, n)
      print *

 200  format(' Vector ',a,':')
 201  format(10x,10(f10.6,2x))
      return
      end

      subroutine print_dvector(x, n, incx, name)
      integer           n, incx
      character*(*)     name
      double precision  x(*)
      integer           i

      intrinsic         abs

      print 200, name
      print 201, (x(1 + (i-1)*abs(incx)), i = 1, n)
      print *

 200  format(' Vector ',a,':')
 201  format(10x,10(f10.6,2x))
      return
      end

      subroutine print_cvector(x, n, incx, name)
      integer       n, incx
      character*(*) name
      complex       x(*)
      integer       i

      intrinsic        abs

      print 200, name
      print 201, (x(1 + (i-1)*abs(incx)), i = 1, n)
      print *

 200  format(' Vector ',a,':')
 201  format(10x,10(f10.6,',',f10.6,3x))
      return
      end

      subroutine print_zvector(x, n, incx, name)
      integer           n, incx
      character*(*)     name
      double complex    x(*)
      integer           i

      intrinsic         abs

      print 200, name
      print 201, (x(1 + (i-1)*abs(incx)), i = 1, n)
      print *

 200  format(' Vector ',a,':')
 201  format(10x,10(f10.6,',',f10.6,3x))
      return
      end

      subroutine print_ivector(v, n, name)
      integer       n
      character*(*) name
      integer       v(*)
      integer       i

      print 200, name
      print 201, (v(i),i=1,n)
      print *

 200  format(' Vector ',a,':')
 201  format(10x,10(i6,2x))
      return
      end

