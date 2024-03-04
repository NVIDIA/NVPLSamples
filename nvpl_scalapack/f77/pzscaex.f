      PROGRAM PZSCAEX
*
*  -- ScaLAPACK example code --
*     University of Tennessee, Knoxville, Oak Ridge National Laboratory,
*     and University of California, Berkeley.
*
*     Written by Antoine Petitet, August 1995 (petitet@cs.utk.edu)
*
*     This program solves a linear system by calling the ScaLAPACK 
*     routine PZGESV. The input matrix and right-and-sides are
*     read from a file. The solution is written to a file.
*
*     .. Parameters ..
      INTEGER            CP16SZ, INTGSZ, MEMSIZ, TOTMEM
      PARAMETER          ( CPLXSZ = 8 )
#if defined(NVPL_ILP64)
      PARAMETER          ( INTGSZ = 8 )
#else
      PARAMETER          ( INTGSZ = 4 )
#endif
      PARAMETER          ( TOTMEM = 2000000, MEMSIZ = TOTMEM / CPLXSZ )
      INTEGER            BLOCK_CYCLIC_2D, CSRC_, CTXT_, DLEN_, DT_,
     $                   LLD_, MB_, M_, NB_, N_, RSRC_
      PARAMETER          ( BLOCK_CYCLIC_2D = 1, DLEN_ = 9, DT_ = 1,
     $                     CTXT_ = 2, M_ = 3, N_ = 4, MB_ = 5, NB_ = 6,
     $                     RSRC_ = 7, CSRC_ = 8, LLD_ = 9 )
      COMPLEX*16         ONE
      PARAMETER          ( ONE = (1.0D+0,0.0D+0) )
*     ..
*     .. Local Scalars ..
      CHARACTER*80       OUTFILE
      INTEGER            IAM, ICTXT, INFO, IPA, IPACPY, IPB, IPPIV, IPX,
     $                   IPW, LIPIV, MYCOL, MYROW, N, NB, NOUT, NPZOL,
     $                   NPROCS, NPROW, NP, NQ, NQRHS, NRHS, WORKSIZ
      DOUBLE PRECISION   ANORM, BNORM, EPS, XNORM, RESID
*     ..
*     .. Local Arrays ..
      INTEGER            DESCA( DLEN_ ), DESCB( DLEN_ ), DESCX( DLEN_ )
      COMPLEX*16, ALLOCATABLE :: MEM(:)
*     ..
*     .. External Subroutines ..
      EXTERNAL           BLACS_EXIT, BLACS_GET, BLACS_GRIDEXIT,
     $                   BLACS_GRIDINFO, BLACS_GRIDINIT, BLACS_PINFO,
     $                   DESCINIT, IGSUM2D, PZGESV,
     $                   PZGEMM, PZLACPY, PZLAPRNT, PZLAREAD, PZLAWRITE
*     ..
*     .. External Functions ..
      INTEGER            ICEIL, NUMROC
      DOUBLE PRECISION   PDLAMCH, PZLANGE
      EXTERNAL           ICEIL, NUMROC, PDLAMCH, PZLANGE
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, MAX
*     ..
*     .. Executable Statements ..
*
*     Get starting information
*
      CALL BLACS_PINFO( IAM, NPROCS )
*
      INFO = 0
      ALLOCATE ( MEM( MEMSIZ ), STAT = INFO )
      IF( INFO.NE.0 ) THEN
         WRITE(*,*) '% Could not allocate MEM. INFO = ', INFO
         GO TO 20
      END IF      
*      
      CALL PDSCAEXINFO( OUTFILE, NOUT, N, NRHS, NB, NPROW, NPZOL, MEM,
     $                  IAM, NPROCS )
*
*     Define process grid
*
      CALL BLACS_GET( -1, 0, ICTXT )
      CALL BLACS_GRIDINIT( ICTXT, 'Row-major', NPROW, NPZOL )
      CALL BLACS_GRIDINFO( ICTXT, NPROW, NPZOL, MYROW, MYCOL )
*
*     Go to bottom of process grid loop if this case doesn't use my
*     process
*
      IF( MYROW.GE.NPROW .OR. MYCOL.GE.NPZOL )
     $   GO TO 20
*
      NP    = NUMROC( N, NB, MYROW, 0, NPROW )
      NQ    = NUMROC( N, NB, MYCOL, 0, NPZOL )
      NQRHS = NUMROC( NRHS, NB, MYCOL, 0, NPZOL )
*
*     Initialize the array descriptor for the matrix A and B
*
      CALL DESCINIT( DESCA, N, N, NB, NB, 0, 0, ICTXT, MAX( 1, NP ),
     $               INFO )
      CALL DESCINIT( DESCB, N, NRHS, NB, NB, 0, 0, ICTXT, MAX( 1, NP ),
     $               INFO )
      CALL DESCINIT( DESCX, N, NRHS, NB, NB, 0, 0, ICTXT, MAX( 1, NP ),
     $               INFO )
*
*     Assign pointers into MEM for SCALAPACK arrays, A is
*     allocated starting at position MEM( 1 )
*
      IPA = 1
      IPACPY = IPA + DESCA( LLD_ )*NQ
      IPB = IPACPY + DESCA( LLD_ )*NQ
      IPX = IPB + DESCB( LLD_ )*NQRHS
      IPPIV = IPX + DESCB( LLD_ )*NQRHS
      LIPIV = ICEIL( INTGSZ*( NP+NB ), CP16SZ )
      IPW = IPPIV + MAX( NP, LIPIV )
*
      WORKSIZ = NB
*
*     Check for adequate memory for problem size
*
      INFO = 0
      IF( IPW+WORKSIZ.GT.MEMSIZ ) THEN
         IF( IAM.EQ.0 )
     $      WRITE( NOUT, FMT = 9998 ) 'test', ( IPW+WORKSIZ )*CP16SZ
         INFO = 1
      END IF
*
*     Check all processes for an error
*
      CALL IGSUM2D( ICTXT, 'All', ' ', 1, 1, INFO, 1, -1, 0 )
      IF( INFO.GT.0 ) THEN
         IF( IAM.EQ.0 )
     $      WRITE( NOUT, FMT = 9999 ) 'MEMORY'
         GO TO 10
      END IF
*
*     Read from file and distribute matrices A and B
*
      CALL PZLAREAD( 'ZSCAEXMAT.dat', MEM( IPA ), DESCA, 0, 0,
     $               MEM( IPW ) )
      CALL PZLAREAD( 'ZSCAEXRHS.dat', MEM( IPB ), DESCB, 0, 0,
     $               MEM( IPW ) )
*
*     Make a copy of A and the rhs for checking purposes
*
      CALL PZLACPY( 'All', N, N, MEM( IPA ), 1, 1, DESCA,
     $              MEM( IPACPY ), 1, 1, DESCA )
      CALL PZLACPY( 'All', N, NRHS, MEM( IPB ), 1, 1, DESCB,
     $              MEM( IPX ), 1, 1, DESCX )
*
**********************************************************************
*     Call ScaLAPACK PZGESV routine
**********************************************************************
*
      IF( IAM.EQ.0 ) THEN
         WRITE( NOUT, FMT = * )
         WRITE( NOUT, FMT = * )
     $         '***********************************************'
         WRITE( NOUT, FMT = * )
     $         'Example of ScaLAPACK routine call: (PZGESV)'
         WRITE( NOUT, FMT = * )
     $         '***********************************************'
         WRITE( NOUT, FMT = * )
         WRITE( NOUT, FMT = * ) 'A * X = B, Matrix A:'
         WRITE( NOUT, FMT = * )
      END IF
      CALL PZLAPRNT( N, N, MEM( IPA ), 1, 1, DESCA, 0, 0,
     $               'A', NOUT, MEM( IPW ) )
      IF( IAM.EQ.0 ) THEN
         WRITE( NOUT, FMT = * )
         WRITE( NOUT, FMT = * ) 'Matrix B:'
         WRITE( NOUT, FMT = * )
      END IF
      CALL PZLAPRNT( N, NRHS, MEM( IPB ), 1, 1, DESCB, 0, 0,
     $               'B', NOUT, MEM( IPW ) )
*
      CALL PZGESV( N, NRHS, MEM( IPA ), 1, 1, DESCA, MEM( IPPIV ),
     $             MEM( IPB ), 1, 1, DESCB, INFO )
*
      IF( MYROW.EQ.0 .AND. MYCOL.EQ.0 ) THEN
         WRITE( NOUT, FMT = * )
         WRITE( NOUT, FMT = * ) 'INFO code returned by PZGESV = ', INFO
         WRITE( NOUT, FMT = * )
         WRITE( NOUT, FMT = * ) 'Matrix X = A^{-1} * B'
         WRITE( NOUT, FMT = * )
      END IF
      CALL PZLAPRNT( N, NRHS, MEM( IPB ), 1, 1, DESCB, 0, 0, 'X', NOUT,
     $               MEM( IPW ) )
      CALL PZLAWRITE( 'ZSCAEXSOL.dat', N, NRHS, MEM( IPB ), 1, 1, DESCB,
     $                0, 0, MEM( IPW ) )
*
*     Compute residual ||A * X  - B|| / ( ||X|| * ||A|| * eps * N )
*
      EPS = PDLAMCH( ICTXT, 'Epsilon' )
      ANORM = PZLANGE( 'I', N, N, MEM( IPA ), 1, 1, DESCA, MEM( IPW ) )
      BNORM = PZLANGE( 'I', N, NRHS, MEM( IPB ), 1, 1, DESCB,
     $                 MEM( IPW ) )
      CALL PZGEMM( 'No transpose', 'No transpose', N, NRHS, N, ONE,
     $             MEM( IPACPY ), 1, 1, DESCA, MEM( IPB ), 1, 1, DESCB,
     $             -ONE, MEM( IPX ), 1, 1, DESCX )
      XNORM = PZLANGE( 'I', N, NRHS, MEM( IPX ), 1, 1, DESCX,
     $                 MEM( IPW ) )
      RESID = XNORM / ( ANORM * BNORM * EPS * DBLE( N ) )
*
      IF( MYROW.EQ.0 .AND. MYCOL.EQ.0 ) THEN
         WRITE( NOUT, FMT = * )
         WRITE( NOUT, FMT = * )
     $     '||A * X  - B|| / ( ||X|| * ||A|| * eps * N ) = ', RESID
         WRITE( NOUT, FMT = * )
         IF( RESID.LT.10.0D+0 ) THEN
            WRITE( NOUT, FMT = * ) 'The answer is correct.'
         ELSE
            WRITE( NOUT, FMT = * ) 'The answer is suspicious.'
         END IF
      END IF
*
   10 CONTINUE
*
      CALL BLACS_GRIDEXIT( ICTXT )
*
      DEALLOCATE( MEM )      
   20 CONTINUE
*
*     Print ending messages and close output file
*
      IF( IAM.EQ.0 ) THEN
         WRITE( NOUT, FMT = * )
         WRITE( NOUT, FMT = * )
         WRITE( NOUT, FMT = 9997 )
         WRITE( NOUT, FMT = * )
         IF( NOUT.NE.6 .AND. NOUT.NE.0 )
     $      CLOSE ( NOUT )
      END IF
*
      CALL BLACS_EXIT( 0 )
*
 9999 FORMAT( 'Bad ', A6, ' parameters: going on to next test case.' )
 9998 FORMAT( 'Unable to perform ', A, ': need TOTMEM of at least',
     $        I11 )
 9997 FORMAT( 'END OF TESTS.' )
*
      STOP
*
*     End of PZSCAEX
*
      END
