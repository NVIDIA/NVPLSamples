#include <stdio.h>
#include <nvpl_lapack.h>
#include <type_traits>
#include "utils.h"

template<class T>
struct is_element_complex : std::false_type {};

template<> struct is_element_complex<nvpl_scomplex_t> : std::true_type {};
template<> struct is_element_complex<nvpl_dcomplex_t> : std::true_type {};

template<typename T>
void print_element(const T* mat, nvpl_int_t index) {
    printf(" %6.2f", mat[index]);
}

template<>
void print_element<nvpl_scomplex_t>(const nvpl_scomplex_t* mat, nvpl_int_t index) {
    printf(" %6.2f + %6.2fj", nvpl_scomplex_t_real(mat[index]), nvpl_scomplex_t_imag(mat[index]));
}

template<>
void print_element<nvpl_dcomplex_t>(const nvpl_dcomplex_t* mat, nvpl_int_t index) {
    printf(" %6.2f + %6.2fj", nvpl_dcomplex_t_real(mat[index]), nvpl_dcomplex_t_imag(mat[index]));
}


/* Auxiliary routine: printing a matrix */
template<typename T>
void print_matrix_rowmajor_impl( const char* desc, nvpl_int_t m, nvpl_int_t n, const T* flat_mat, nvpl_int_t ldn ) {
    nvpl_int_t i, j;
    printf( "\n %s\n", desc );

    for( i = 0; i < m; i++ ) {
        for( j = 0; j < n; j++ ) print_element(flat_mat, i*ldn+j);
        printf( "\n" );
    }
}


/* Auxiliary routine: printing a matrix */
template<typename T>
void print_matrix_colmajor_impl( const char* desc, nvpl_int_t m, nvpl_int_t n, const T* flat_mat, nvpl_int_t ldm ) {
    nvpl_int_t i, j;
    printf( "\n %s\n", desc );

    for( i = 0; i < m; i++ ) {
        for( j = 0; j < n; j++ ) print_element(flat_mat, i+j*ldm);
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

extern "C" {
    void print_smatrix_rowmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const float* mat, nvpl_int_t ldn ) {
        print_matrix_rowmajor_impl(desc, m, n, mat, ldn);
    }

    void print_dmatrix_rowmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const double* mat, nvpl_int_t ldn ) {
        print_matrix_rowmajor_impl(desc, m, n, mat, ldn);
    }

    void print_cmatrix_rowmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const nvpl_scomplex_t* mat, nvpl_int_t ldn ) {
        print_matrix_rowmajor_impl(desc, m, n, mat, ldn);
    }

    void print_zmatrix_rowmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const nvpl_dcomplex_t* mat, nvpl_int_t ldn ) {
        print_matrix_rowmajor_impl(desc, m, n, mat, ldn);
    }

    void print_smatrix_colmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const float* mat, nvpl_int_t ldm ) {
        print_matrix_colmajor_impl(desc, m, n, mat, ldm);
    }

    void print_dmatrix_colmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const double* mat, nvpl_int_t ldm ) {
        print_matrix_colmajor_impl(desc, m, n, mat, ldm);
    }

    void print_cmatrix_colmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const nvpl_scomplex_t* mat, nvpl_int_t ldm ) {
        print_matrix_colmajor_impl(desc, m, n, mat, ldm);
    }

    void print_zmatrix_colmajor( const char* desc, nvpl_int_t m, nvpl_int_t n, const nvpl_dcomplex_t* mat, nvpl_int_t ldm ) {
        print_matrix_colmajor_impl(desc, m, n, mat, ldm);
    }
}
