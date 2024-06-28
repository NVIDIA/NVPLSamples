#include <stdio.h>
#include <nvpl_lapack.h>
#include "utils.h"

/* Main program */
int main()
{
   nvpl_int_t info;
   /* Initialization */
   const nvpl_int_t m = 5;
   const nvpl_int_t n = 3;
   const nvpl_int_t nrhs = 2;
   const nvpl_int_t lda = 5;
   const nvpl_int_t ldb = 5;

   double A[n][m] = {{1,2,3,4,5},{1,3,5,2,4},{1,4,2,5,3}};
   double b[nrhs][m] = {{-10,12,14,16,18},{-3,14,12,16,16}};

   printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

   print_dmatrix_colmajor( "Entry Matrix *A", m, n, *A, lda );
   print_dmatrix_colmajor( "Right Hand Side *b", m, nrhs, *b, ldb );
   printf( "\n" );
 
   printf( "dgels (col-major, high-level) Example Program Results\n" );
   /* Solve least squares problem*/
   char t = 'N';
   double* work = NULL;
   double w;
   nvpl_int_t lwork = -1;
   NVPL_LAPACK_dgels(&t,&m,&n,&nrhs,*A,&lda,*b,&ldb, &w, &lwork, &info);
   dgels_(&t,&m,&n,&nrhs,*A,&lda,*b,&ldb, &w, &lwork, &info);
   lwork = (nvpl_int_t)w;
   printf("lwork = %ld", (long int)lwork) ;
   work = (double*) malloc(sizeof(double) * lwork);
   if(!work) {
           printf("insufficient memory");
           exit(-1);
   }
   NVPL_LAPACK_dgels(&t,&m,&n,&nrhs,*A,&lda,*b,&ldb, work, &lwork, &info);

   /* Print Solution */
   print_dmatrix_colmajor( "Solution", n, nrhs, *b, ldb );
   printf( "\n" );

   free(work);

   exit( info );
}
