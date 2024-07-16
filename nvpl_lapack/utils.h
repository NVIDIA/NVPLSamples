#ifndef NVPL_LAPACK_EXAMPLE_UTILS_H
#define NVPL_LAPACK_EXAMPLE_UTILS_H
// C interface functions
extern "C" {
void print_smatrix_rowmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const float* mat, nvpl_int_t ldn );
void print_dmatrix_rowmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const double* mat, nvpl_int_t ldn );
void print_cmatrix_rowmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const nvpl_scomplex_t* mat, nvpl_int_t ldn );
void print_zmatrix_rowmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const nvpl_dcomplex_t* mat, nvpl_int_t ldn );

void print_smatrix_colmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const float* mat, nvpl_int_t ldm );
void print_dmatrix_colmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const double* mat, nvpl_int_t ldm );
void print_cmatrix_colmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const nvpl_scomplex_t* mat, nvpl_int_t ldm );
void print_zmatrix_colmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const nvpl_dcomplex_t* mat, nvpl_int_t ldm );
}
#endif //NVPL_LAPACK_EXAMPLE_UTILS_H
