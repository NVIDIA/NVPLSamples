#include <stdio.h>
#include <nvpl_lapack.h>

/* Auxiliary routine: printing a matrix */
void print_matrix_rowmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const double* mat, nvpl_int_t ldm ) {
        nvpl_int_t i, j;
        printf( "\n %s\n", desc );
 
        for( i = 0; i < m; i++ ) {
                for( j = 0; j < n; j++ ) printf( " %6.2f", mat[i*ldm+j] );
                printf( "\n" );
        }
}
 
 
/* Auxiliary routine: printing a matrix */
void print_matrix_colmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const double* mat, nvpl_int_t ldm ) {
        nvpl_int_t i, j;
        printf( "\n %s\n", desc );
 
        for( i = 0; i < m; i++ ) {
                for( j = 0; j < n; j++ ) printf( " %6.2f", mat[i+j*ldm] );
                printf( "\n" );
        }
}
 
/* Auxiliary routine: printing a vector of integers */
void print_vector( const char* desc, nvpl_int_t n, const nvpl_int_t* vec ) {
        nvpl_int_t j;
        printf( "\n %s\n", desc );
        for( j = 0; j < n; j++ ) printf( " %6" NVPL_LAPACK_IFMT, vec[j] );
        printf( "\n" );
}

/* Main program */
int main()
{
   double A[5][3] = {{1,2,3},{4,5,1},{3,5,2},{4,1,4},{2,5,3}};
   double b[5][2] = {{-10,12},{14,16},{18,-3},{14,12},{16,16}};

   nvpl_int_t info,m,n,lda,ldb,nrhs;

   /* Initialization */
   m = 5;
   n = 3;
   nrhs = 2;
   lda = 5;
   ldb = 5;

   printf("NVPL LAPACK version: %d\n", nvpl_lapack_get_version());

   print_matrix_colmajor( "Entry Matrix *A", m, n, *A, lda );
   print_matrix_colmajor( "Right Hand Side *b", n, nrhs, *b, ldb );
   printf("sizeof(nvpl_int_t) = %d\n", (int) sizeof(nvpl_int_t));
   printf( "\n" );
 
   printf( "dgels (col-major, high-level) Example Program Results\n" );
   /* Solve least squares problem*/
   char t = 'N';
   double* work;
   double w;
   nvpl_int_t lwork = -1;
   dgels_(&t,&m,&n,&nrhs,*A,&lda,*b,&ldb, &w, &lwork, &info);
   printf("lwork = %f", w) ;
   work = (double*) malloc(sizeof(double) * w);
   lwork = static_cast<nvpl_int_t>(w);
   dgels_(&t,&m,&n,&nrhs,*A,&lda,*b,&ldb, work, &lwork, &info);


   /* Print Solution */
   print_matrix_colmajor( "Solution", n, nrhs, *b, ldb );
   printf( "\n" );

   free(work);

   exit( info );
}
