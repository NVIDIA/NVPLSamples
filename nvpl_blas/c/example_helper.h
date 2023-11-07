#ifndef EXAMPLE_HELPER_H
#define EXAMPLE_HELPER_H
#include <stdio.h> // printf
#include <stdlib.h> // labs
#include <inttypes.h> // PRId64
#include <math.h> // NAN
#include "nvpl_blas_cblas.h"

#define MIN(a,b)  (((a) > (b)) ? (b) : (a))
#define MAX(a,b)  (((a) > (b)) ? (a) : (b))

enum FILL_MODE {Full=120, Upper=121, Lower=122};
#define UPLO_TO_FILL_MODE(uplo) ((CblasUpper == uplo) ? Upper : Lower)

/**
 * \brief      Fill data for single precision matrix
 *
 * \param      A        The pointer of the matrix
 * \param[in]  rows     The rows of the matrix
 * \param[in]  cols     The columns of the matrix
 * \param[in]  ld       The leading demension of the matrix
 * \param[in]  order    row-major or column-major matrix
 * \param[in]  fillMode Matrix lower or upper or all the part is stored
 * \param[in]  diag     Indicates if the elements on the main diagonal of matrix are unity and should not be accessed
 */
static void fill_smatrix(float * A, nvpl_int_t rows, nvpl_int_t cols, nvpl_int_t ld, enum CBLAS_ORDER order,
        enum FILL_MODE fillMode, enum CBLAS_DIAG diag) {
    if (CblasColMajor == order) {
        if (Full == fillMode) {
            for (nvpl_int_t j = 0; j < cols; ++j) {
                for (nvpl_int_t i = 0; i < rows; ++i) {
                    A[i + ld * j] = (float)((float)1/1024.f * ((91u + j) & 1023u) - 0.5f);
                }
            }
        } else {
            for (nvpl_int_t j = 0; j < cols; ++j) {
                for (nvpl_int_t i = 0; i < rows; ++i) {
                    if (((Lower == fillMode) ? (j < i) : (j > i)) || ((CblasNonUnit == diag) && (i == j))) {
                        A[i + ld * j] = (float)((float)1/1024.f * ((91u + j) & 1023u) - 0.5f);
                    } else if (i == j) {
                        A[i + ld * j] = 1.0f;
                    } else {
                        A[i + ld * j] = NAN;
                    }
                }
            }
         }
    } else {
        if (Full == fillMode) {
            for (nvpl_int_t i = 0; i < rows; ++i) {
                for (nvpl_int_t j = 0; j < cols; ++j) {
                    A[j + ld * i] = (float)((float)1/1024.f * ((91u + i) & 1023u) - 0.5f);
                }
            }
        } else {
            for (nvpl_int_t i = 0; i < rows; ++i) {
                for (nvpl_int_t j = 0; j < cols; ++j) {
                    if (((Lower == fillMode) ? (j < i) : (j > i)) || ((CblasNonUnit == diag) && (i == j))) {
                        A[j + ld * i] = (float)((float)1/1024.f * ((91u + i) & 1023u) - 0.5f);
                    } else if (i == j) {
                        A[j + ld * i] = 1.0f;
                    } else {
                        A[j + ld * i] = NAN;
                    }
                }
            }
        }
    }
}

/**
 * \brief      Fill data for double precision matrix
 *
 * \param      A        The pointer of the matrix
 * \param[in]  rows     The rows of the matrix
 * \param[in]  cols     The columns of the matrix
 * \param[in]  ld       The leading demension of the matrix
 * \param[in]  order    row-major or column-major matrix
 * \param[in]  fillMode Matrix lower or upper or all the part is stored
 * \param[in]  diag     Indicates if the elements on the main diagonal of matrix are unity and should not be accessed
 */
static void fill_dmatrix(double * A, nvpl_int_t rows, nvpl_int_t cols, nvpl_int_t ld, enum CBLAS_ORDER order,
        enum FILL_MODE fillMode, enum CBLAS_DIAG diag) {
    if (CblasColMajor == order) {
        if (Full == fillMode) {
            for (nvpl_int_t j = 0; j < cols; ++j) {
                for (nvpl_int_t i = 0; i < rows; ++i) {
                    A[i + ld * j] = (double)((double)1/1024.f * ((91u + j) & 1023u) - 0.5f);
                }
            }
        } else {
            for (nvpl_int_t j = 0; j < cols; ++j) {
                for (nvpl_int_t i = 0; i < rows; ++i) {
                    if (((Lower == fillMode) ? (j < i) : (j > i)) || ((CblasNonUnit == diag) && (i == j))) {
                        A[i + ld * j] = (double)((double)1/1024.f * ((91u + j) & 1023u) - 0.5f);
                    } else if (i == j) {
                        A[i + ld * j] = 1.0f;
                    } else {
                        A[i + ld * j] = NAN;
                    }
                }
            }
         }
    } else {
        if (Full == fillMode) {
            for (nvpl_int_t i = 0; i < rows; ++i) {
                for (nvpl_int_t j = 0; j < cols; ++j) {
                    A[j + ld * i] = (double)((double)1/1024.f * ((91u + i) & 1023u) - 0.5f);
                }
            }
        } else {
            for (nvpl_int_t i = 0; i < rows; ++i) {
                for (nvpl_int_t j = 0; j < cols; ++j) {
                    if (((Lower == fillMode) ? (j < i) : (j > i)) || ((CblasNonUnit == diag) && (i == j))) {
                        A[j + ld * i] = (double)((double)1/1024.f * ((91u + i) & 1023u) - 0.5f);
                    } else if (i == j) {
                        A[j + ld * i] = 1.0f;
                    } else {
                        A[j + ld * i] = NAN;
                    }
                }
            }
        }
    }
}

/**
 * \brief      Fill data for nvpl_dcomplex_t precision matrix
 *
 * \param      A        The pointer of the matrix
 * \param[in]  rows     The rows of the matrix
 * \param[in]  cols     The columns of the matrix
 * \param[in]  ld       The leading demension of the matrix
 * \param[in]  order    row-major or column-major matrix
 * \param[in]  fillMode Matrix lower or upper or all the part is stored
 * \param[in]  diag     Indicates if the elements on the main diagonal of matrix are unity and should not be accessed
 */
static void fill_zmatrix(nvpl_dcomplex_t * A, nvpl_int_t rows, nvpl_int_t cols, nvpl_int_t ld, enum CBLAS_ORDER order,
        enum FILL_MODE fillMode, enum CBLAS_DIAG diag) {
    if (CblasColMajor == order) {
        if (Full == fillMode) {
            for (nvpl_int_t j = 0; j < cols; ++j) {
                for (nvpl_int_t i = 0; i < rows; ++i) {
                    A[i + ld * j] = (nvpl_dcomplex_t){1/1024.f * ((91u + j) & 1023u) - 0.5f,
                        1/1024.f *((479u + j) & 1023u) - 0.5f};
                }
            }
        } else {
            for (nvpl_int_t j = 0; j < cols; ++j) {
                for (nvpl_int_t i = 0; i < rows; ++i) {
                    if (((Lower == fillMode) ? (j < i) : (j > i)) || ((CblasNonUnit == diag) && (i == j))) {
                    A[i + ld * j] = (nvpl_dcomplex_t){1/1024.f * ((91u + j) & 1023u) - 0.5f,
                        1/1024.f *((479u + j) & 1023u) - 0.5f};
                     } else if (i == j) {
                        A[i + ld * j] = (nvpl_dcomplex_t){1.0f, 0.0f};
                     } else {
                        A[i + ld * j] = (nvpl_dcomplex_t){NAN, NAN};
                     }
                }
            }
         }
    } else {
        if (Full == fillMode) {
            for (nvpl_int_t i = 0; i < rows; ++i) {
                for (nvpl_int_t j = 0; j < cols; ++j) {
                    A[j + ld * i] = (nvpl_dcomplex_t){1/1024.f * ((91u + i) & 1023u) - 0.5f,
                        1/1024.f *((479u + i) & 1023u) - 0.5f};
                 }
            }
        } else {
            for (nvpl_int_t i = 0; i < rows; ++i) {
                for (nvpl_int_t j = 0; j < cols; ++j) {
                    if (((Lower == fillMode) ? (j < i) : (j > i)) || ((CblasNonUnit == diag) && (i == j))) {
                        A[j + ld * i] = (nvpl_dcomplex_t){1/1024.f * ((91u + i) & 1023u) - 0.5f,
                            1/1024.f *((479u + i) & 1023u) - 0.5f};
                    } else if (i == j) {
                        A[j + ld * i] = (nvpl_dcomplex_t){1.0f, 0.0f};
                    } else {
                        A[j + ld * i] = (nvpl_dcomplex_t){NAN, NAN};
                    }
                }
            }
        }
    }
}

/**
 * \brief      Fill data for nvpl_scomplex_t precision matrix
 *
 * \param      A        The pointer of the matrix
 * \param[in]  rows     The rows of the matrix
 * \param[in]  cols     The columns of the matrix
 * \param[in]  ld       The leading demension of the matrix
 * \param[in]  order    row-major or column-major matrix
 * \param[in]  fillMode Matrix lower or upper or all the part is stored
 * \param[in]  diag     Indicates if the elements on the main diagonal of matrix are unity and should not be accessed
 */
static void fill_cmatrix(nvpl_scomplex_t * A, nvpl_int_t rows, nvpl_int_t cols, nvpl_int_t ld, enum CBLAS_ORDER order,
        enum FILL_MODE fillMode, enum CBLAS_DIAG diag) {
    if (CblasColMajor == order) {
        if (Full == fillMode) {
            for (nvpl_int_t j = 0; j < cols; ++j) {
                for (nvpl_int_t i = 0; i < rows; ++i) {
                    A[i + ld * j] = (nvpl_scomplex_t){1/1024.f * ((91u + j) & 1023u) - 0.5f,
                        1/1024.f *((479u + j) & 1023u) - 0.5f};
                }
            }
        } else {
            for (nvpl_int_t j = 0; j < cols; ++j) {
                for (nvpl_int_t i = 0; i < rows; ++i) {
                    if (((Lower == fillMode) ? (j < i) : (j > i)) || ((CblasNonUnit == diag) && (i == j))) {
                    A[i + ld * j] = (nvpl_scomplex_t){1/1024.f * ((91u + j) & 1023u) - 0.5f,
                        1/1024.f *((479u + j) & 1023u) - 0.5f};
                    } else if (i == j) {
                        A[i + ld * j] = (nvpl_scomplex_t){1.0f, 0.0f};
                    } else {
                        A[i + ld * j] = (nvpl_scomplex_t){NAN, NAN};
                    }
                }
            }
         }
    } else {
        if (Full == fillMode) {
            for (nvpl_int_t i = 0; i < rows; ++i) {
                for (nvpl_int_t j = 0; j < cols; ++j) {
                    A[j + ld * i] = (nvpl_scomplex_t){1/1024.f * ((91u + i) & 1023u) - 0.5f,
                        1/1024.f *((479u + i) & 1023u) - 0.5f};
                 }
            }
        } else {
            for (nvpl_int_t i = 0; i < rows; ++i) {
                for (nvpl_int_t j = 0; j < cols; ++j) {
                    if (((Lower == fillMode) ? (j < i) : (j > i)) || ((CblasNonUnit == diag) && (i == j))) {
                        A[j + ld * i] = (nvpl_scomplex_t){1/1024.f * ((91u + i) & 1023u) - 0.5f,
                            1/1024.f *((479u + i) & 1023u) - 0.5f};
                    } else if (i == j) {
                        A[j + ld * i] = (nvpl_scomplex_t){1.0f, 0.0f};
                    } else {
                        A[j + ld * i] = (nvpl_scomplex_t){NAN, NAN};
                    }
                }
            }
        }
    }
}


/**
 * \brief      Fill data for single precision array 
 *
 * \param      A    The pointer of the vector
 * \param[in]  n    The number of elements in the vector A
 * \param[in]  inc  stride between consecutive elements of A
 */

static void fill_svector(float * A, nvpl_int_t n, nvpl_int_t inc) {
    for (nvpl_int_t i = 0; i < (1+(n-1)*labs(inc)); ++i) {
        A[i] = (float)((float)1/1024.f * ((91u + i) & 1023u) - 0.5f);
    }
}


/**
 * \brief      Fill data for double precision array 
 *
 * \param      A    The pointer of the vector
 * \param[in]  n    The number of elements in the vector A
 * \param[in]  inc  stride between consecutive elements of A
 */

static void fill_dvector(double * A, nvpl_int_t n, nvpl_int_t inc) {
    for (nvpl_int_t i = 0; i < (1+(n-1)*labs(inc)); ++i) {
        A[i] = (double)((double)1/1024.f * ((91u + i) & 1023u) - 0.5f);
    }
}

/**
 * \brief      Fill data for single complex precision array 
 *
 * \param      A    The pointer of the vector
 * \param[in]  n    The number of elements in the vector A
 * \param[in]  inc  stride between consecutive elements of A
 */

static void fill_cvector(nvpl_scomplex_t * A, nvpl_int_t n, nvpl_int_t inc) {
    for (nvpl_int_t i = 0; i < (1+(n-1)*labs(inc)); ++i) {
        A[i] = (nvpl_scomplex_t){1/1024.f * ((91u + i) & 1023u) - 0.5f, 1/1024.f * ((479u + i) & 1023u) - 0.5f};
    }
}

/**
 * \brief      Fill data for nvpl_dcomplex_t precision array
 *
 * \param      A    The pointer of the vector
 * \param[in]  n    The number of elements in the vector A
 * \param[in]  inc  stride between consecutive elements of A
 */

static void fill_zvector(nvpl_dcomplex_t * A, nvpl_int_t n, nvpl_int_t inc) {
    for (nvpl_int_t i = 0; i < (1+(n-1)*labs(inc)); ++i) {
        A[i] = (nvpl_dcomplex_t){1/1024.f * ((91u + i) & 1023u) - 0.5f, 1/1024.f * ((479u + i) & 1023u) - 0.5f};
    }
}

/**
 * \brief      Fill data for single precision banded matrix
 *
 * \param      A        The pointer of the matrix
 * \param[in]  rows     The rows of the matrix
 * \param[in]  cols     The columns of the matrix
 * \param[in]  ld       The leading demension of the matrix
 * \param[in]  order    row-major or column-major matrix
 */
static void fill_sbandmatrix(float * A, nvpl_int_t kl, nvpl_int_t ku,
                    nvpl_int_t rows, nvpl_int_t cols,
                    nvpl_int_t ld, enum CBLAS_ORDER order) {
                        
    nvpl_int_t  i, j, l;
    nvpl_int_t  kl1, ku1, i_start, j_start, j_end, kl_rows, ku_rows;  

    if (CblasRowMajor == order) {
        for( i = 0; i < MIN( rows, cols ); i++ ) {
            kl1  = ( i - kl <= 0 ) ? i : kl;
            ku1  = ( i + ku >= cols ) ? MAX(0,cols-i-1) : ku;
            j_start = kl - kl1;
            j_end = j_start + kl1 + ku1;
            for( l=0; l < j_end-j_start+1; l++ )
                A[i*ld + j_start + l] = (float)(i*ld + j_start + l);            
         }
        j_start = kl - cols;
        for( i = MIN( rows, cols ); i < MIN( rows, MIN( rows, cols ) + kl); i++ ) {
            j_start = ( j_start < 0 ) ? 0 : j_start;
            kl1  = cols - i + kl;
            j_end = j_start + kl1 - 1;
            for( l=j_start; l < j_end-j_start+1; l++ )
                A[i*ld + l] = (float)(i*ld + l);
            j_start--;
        }
    } else {
        i_start = (ku > cols ) ? ku - cols + 1 : 0;
        ku_rows = (ku > cols ) ? cols - 1 : ku;
        j_start = ku_rows;
        for( i = 0; i < ku_rows; i++ ) {
            j = j_start*ld;
            for( l=0; l < cols-j_start; l++ )
                A[i + i_start + j + l*ld] =  (float)(i + i_start + j + l*ld);
            j_start--;
        }

        j_end = MIN(rows,cols);
        for( l=0; l < j_end; l++ )
            A[ku + l*ld] = (float)(ku + l*ld);

        kl_rows = ( kl <= rows-1 ) ?  kl : rows - 1;
        for ( i = 1; i < kl_rows+1; i++ ) {
            kl1 = ( i+j_end <= rows ) ? j_end : rows - i;
            for( l=0; l < kl1; l++ )
                A[ku + i + l*ld] = (float)(ku + i + l*ld);
        }
    }
}

/**
 * \brief      Fill data for double precision banded matrix
 *
 * \param      A        The pointer of the matrix
 * \param[in]  rows     The rows of the matrix
 * \param[in]  cols     The columns of the matrix
 * \param[in]  ld       The leading demension of the matrix
 * \param[in]  order    row-major or column-major matrix
 */
static void fill_dbandmatrix(double * A, nvpl_int_t kl, nvpl_int_t ku,
                    nvpl_int_t rows, nvpl_int_t cols,
                    nvpl_int_t ld, enum CBLAS_ORDER order) {
                        
    nvpl_int_t  i, j, l;
    nvpl_int_t  kl1, ku1, i_start, j_start, j_end, kl_rows, ku_rows;  

    if (CblasRowMajor == order) {
        for( i = 0; i < MIN( rows, cols ); i++ ) {
            kl1  = ( i - kl <= 0 ) ? i : kl;
            ku1  = ( i + ku >= cols ) ? MAX(0,cols-i-1) : ku;
            j_start = kl - kl1;
            j_end = j_start + kl1 + ku1;
            for( l=0; l < j_end-j_start+1; l++ )
                A[i*ld + j_start + l] = (double)(i*ld + j_start + l);            
         }
        j_start = kl - cols;
        for( i = MIN( rows, cols ); i < MIN( rows, MIN( rows, cols ) + kl); i++ ) {
            j_start = ( j_start < 0 ) ? 0 : j_start;
            kl1  = cols - i + kl;
            j_end = j_start + kl1 - 1;
            for( l=j_start; l < j_end-j_start+1; l++ )
                A[i*ld + l] = (double)(i*ld + l);
            j_start--;
        }
    } else {
        i_start = (ku > cols ) ? ku - cols + 1 : 0;
        ku_rows = (ku > cols ) ? cols - 1 : ku;
        j_start = ku_rows;
        for( i = 0; i < ku_rows; i++ ) {
            j = j_start*ld;
            for( l=0; l < cols-j_start; l++ )
                A[i + i_start + j + l*ld] =  (double)(i + i_start + j + l*ld);
            j_start--;
        }

        j_end = MIN(rows,cols);
        for( l=0; l < j_end; l++ )
            A[ku + l*ld] = (double)(ku + l*ld);

        kl_rows = ( kl <= rows-1 ) ?  kl : rows - 1;
        for ( i = 1; i < kl_rows+1; i++ ) {
            kl1 = ( i+j_end <= rows ) ? j_end : rows - i;
            for( l=0; l < kl1; l++ )
                A[ku + i + l*ld] = (double)(ku + i + l*ld);
        }
    }
}

/**
 * \brief      Fill data for nvpl_scomplex_t precision banded matrix
 *
 * \param      A        The pointer of the matrix
 * \param[in]  rows     The rows of the matrix
 * \param[in]  cols     The columns of the matrix
 * \param[in]  ld       The leading demension of the matrix
 * \param[in]  order    row-major or column-major matrix
 */
static void fill_cbandmatrix(nvpl_scomplex_t * A, nvpl_int_t kl, nvpl_int_t ku,
                    nvpl_int_t rows, nvpl_int_t cols,
                    nvpl_int_t ld, enum CBLAS_ORDER order) {
                        
    nvpl_int_t  i, j, l;
    nvpl_int_t  kl1, ku1, i_start, j_start, j_end, kl_rows, ku_rows;  

    if (CblasRowMajor == order) {
        for( i = 0; i < MIN( rows, cols ); i++ ) {
            kl1  = ( i - kl <= 0 ) ? i : kl;
            ku1  = ( i + ku >= cols ) ? MAX(0,cols-i-1) : ku;
            j_start = kl - kl1;
            j_end = j_start + kl1 + ku1;
            for( l=0; l < j_end-j_start+1; l++ )
                A[i*ld + j_start + l] = (nvpl_scomplex_t){i*ld + j_start + l, i*ld + j_start + l};            
         }
        j_start = kl - cols;
        for( i = MIN( rows, cols ); i < MIN( rows, MIN( rows, cols ) + kl); i++ ) {
            j_start = ( j_start < 0 ) ? 0 : j_start;
            kl1  = cols - i + kl;
            j_end = j_start + kl1 - 1;
            for( l=j_start; l < j_end-j_start+1; l++ )
                A[i*ld + l] = (nvpl_scomplex_t){i*ld + l, i*ld + l};
            j_start--;
        }
    } else {
        i_start = (ku > cols ) ? ku - cols + 1 : 0;
        ku_rows = (ku > cols ) ? cols - 1 : ku;
        j_start = ku_rows;
        for( i = 0; i < ku_rows; i++ ) {
            j = j_start*ld;
            for( l=0; l < cols-j_start; l++ )
                A[i + i_start + j + l*ld] =  (nvpl_scomplex_t){i + i_start + j + l*ld, i + i_start + j + l*ld};
            j_start--;
        }

        j_end = MIN(rows,cols);
        for( l=0; l < j_end; l++ )
            A[ku + l*ld] = (nvpl_scomplex_t){ku + l*ld, ku + l*ld};

        kl_rows = ( kl <= rows-1 ) ?  kl : rows - 1;
        for ( i = 1; i < kl_rows+1; i++ ) {
            kl1 = ( i+j_end <= rows ) ? j_end : rows - i;
            for( l=0; l < kl1; l++ )
                A[ku + i + l*ld] = (nvpl_scomplex_t){ku + i + l*ld, ku + i + l*ld};
        }
    }
}

/**
 * \brief      Fill data for nvpl_dcomplex_t precision banded matrix
 *
 * \param      A        The pointer of the matrix
 * \param[in]  rows     The rows of the matrix
 * \param[in]  cols     The columns of the matrix
 * \param[in]  ld       The leading demension of the matrix
 * \param[in]  order    row-major or column-major matrix
 */
static void fill_zbandmatrix(nvpl_dcomplex_t * A, nvpl_int_t kl, nvpl_int_t ku,
                    nvpl_int_t rows, nvpl_int_t cols,
                    nvpl_int_t ld, enum CBLAS_ORDER order) {
                        
    nvpl_int_t  i, j, l;
    nvpl_int_t  kl1, ku1, i_start, j_start, j_end, kl_rows, ku_rows;  

    if (CblasRowMajor == order) {
        for( i = 0; i < MIN( rows, cols ); i++ ) {
            kl1  = ( i - kl <= 0 ) ? i : kl;
            ku1  = ( i + ku >= cols ) ? MAX(0,cols-i-1) : ku;
            j_start = kl - kl1;
            j_end = j_start + kl1 + ku1;
            for( l=0; l < j_end-j_start+1; l++ )
                A[i*ld + j_start + l] = (nvpl_dcomplex_t){i*ld + j_start + l, i*ld + j_start + l};            
         }
        j_start = kl - cols;
        for( i = MIN( rows, cols ); i < MIN( rows, MIN( rows, cols ) + kl); i++ ) {
            j_start = ( j_start < 0 ) ? 0 : j_start;
            kl1  = cols - i + kl;
            j_end = j_start + kl1 - 1;
            for( l=j_start; l < j_end-j_start+1; l++ )
                A[i*ld + l] = (nvpl_dcomplex_t){i*ld + l, i*ld + l};
            j_start--;
        }
    } else {
        i_start = (ku > cols ) ? ku - cols + 1 : 0;
        ku_rows = (ku > cols ) ? cols - 1 : ku;
        j_start = ku_rows;
        for( i = 0; i < ku_rows; i++ ) {
            j = j_start*ld;
            for( l=0; l < cols-j_start; l++ )
                A[i + i_start + j + l*ld] =  (nvpl_dcomplex_t){i + i_start + j + l*ld, i + i_start + j + l*ld};
            j_start--;
        }

        j_end = MIN(rows,cols);
        for( l=0; l < j_end; l++ )
            A[ku + l*ld] = (nvpl_dcomplex_t){ku + l*ld, ku + l*ld};

        kl_rows = ( kl <= rows-1 ) ?  kl : rows - 1;
        for ( i = 1; i < kl_rows+1; i++ ) {
            kl1 = ( i+j_end <= rows ) ? j_end : rows - i;
            for( l=0; l < kl1; l++ )
                A[ku + i + l*ld] = (nvpl_dcomplex_t){ku + i + l*ld, ku + i + l*ld};
        }
    }
}

/**
 * \brief      Print data for single precision array 
 *
 * \param      A        The pointer of the vector
 * \param[in]  n        The number of elements in the vector A
 * \param[in]  inc      stride between consecutive elements of A
 * \param[in]  vec_name array'name
 */
static void print_svector(float * A, nvpl_int_t n, nvpl_int_t inc, const char * vec_name) {
    printf("%s=", vec_name);
    for (nvpl_int_t i = 0; i < (1+(n-1)*labs(inc)); ++i) {
        printf("%g ", A[i]);
    }
    printf("\n");
}

/**
 * \brief      Print data for double precision array 
 *
 * \param      A        The pointer of the vector
 * \param[in]  n        The number of elements in the vector A
 * \param[in]  inc      stride between consecutive elements of A
 * \param[in]  vec_name array'name
 */
static void print_dvector(double * A, nvpl_int_t n, nvpl_int_t inc, const char * vec_name) {
    printf("%s=", vec_name);
    for (nvpl_int_t i = 0; i < (1+(n-1)*labs(inc)); ++i) {
        printf("%g ", A[i]);
    }
    printf("\n");
}

/**
 * \brief      Print data for single complex precision array 
 *
 * \param      A        The pointer of the vector
 * \param[in]  n        The number of elements in the vector A
 * \param[in]  inc      stride between consecutive elements of A
 * \param[in]  vec_name array'name
 */
static void print_cvector(nvpl_scomplex_t * A, nvpl_int_t n, nvpl_int_t inc, const char * vec_name) {
    printf("%s=", vec_name);
    for (nvpl_int_t i = 0; i < (1+(n-1)*labs(inc)); ++i) {
        printf("(%g, %g) ", A[i].real, A[i].imag);
    }
    printf("\n");
}

/**
 * \brief      Print data for nvpl_dcomplex_t precision array
 *
 * \param      A        The pointer of the vector
 * \param[in]  n        The number of elements in the vector A
 * \param[in]  inc      stride between consecutive elements of A
 * \param[in]  vec_name array'name
 */
static void print_zvector(nvpl_dcomplex_t * A, nvpl_int_t n, nvpl_int_t inc, const char * vec_name) {
    printf("%s=", vec_name);
    for (nvpl_int_t i = 0; i < (1+(n-1)*labs(inc)); ++i) {
        printf("(%g, %g) ", A[i].real, A[i].imag);
    }
    printf("\n");
}


/**
 * \brief      Print data for single precision matrix 
 *
 * \param      A           The pointer of the matrix
 * \param[in]  rows        The rows of the matrix
 * \param[in]  cols        The columns of the matrix
 * \param[in]  ld          The leading demension of the matrix
 * \param[in]  matrix_name matrix's name
 * \param[in]  order       row-major or column-major matrix
 */

static void print_smatrix(float * A, nvpl_int_t rows, nvpl_int_t cols, nvpl_int_t ld,
        const char * matrix_name, enum CBLAS_ORDER order) {
    printf("%s=", matrix_name);
    if (CblasColMajor == order) {
        for (nvpl_int_t j = 0; j < cols; ++j) {
            printf("\t");
            for (nvpl_int_t i = 0; i < rows; ++i) {
                printf("%g ", A[i + ld * j]);
            }
            printf("\n");
        }
    } else {
        for (nvpl_int_t i = 0; i < rows; ++i) {
            printf("\t");
            for (nvpl_int_t j = 0; j < cols; ++j) {
                printf("%g ", A[j + ld * i]);
            }
            printf("\n");
        }
    }
}

/**
 * \brief      Print data for double precision matrix
 *
 * \param      A           The pointer of the matrix
 * \param[in]  rows        The rows of the matrix
 * \param[in]  cols        The columns of the matrix
 * \param[in]  ld          The leading demension of the matrix
 * \param[in]  matrix_name matrix's name
 * \param[in]  order       row-major or column-major matrix
 */

static void print_dmatrix(double * A, nvpl_int_t rows, nvpl_int_t cols, nvpl_int_t ld,
        const char * matrix_name, enum CBLAS_ORDER order) {
    printf("%s=", matrix_name);
    if (CblasColMajor == order) {
        for (nvpl_int_t j = 0; j < cols; ++j) {
            printf("\t");
            for (nvpl_int_t i = 0; i < rows; ++i) {
                printf("%g ", A[i + ld * j]);
            }
            printf("\n");
        }
    } else {
        for (nvpl_int_t i = 0; i < rows; ++i) {
            printf("\t");
            for (nvpl_int_t j = 0; j < cols; ++j) {
                printf("%g ", A[j + ld * i]);
            }
            printf("\n");
        }
    }
}

/**
 * \brief      Print data for nvpl_scomplex_t precision matrix
 *
 * \param      A           The pointer of the matrix
 * \param[in]  rows        The rows of the matrix
 * \param[in]  cols        The columns of the matrix
 * \param[in]  ld          The leading demension of the matrix
 * \param[in]  matrix_name matrix's name
 * \param[in]  order       row-major or column-major matrix
 */

static void print_cmatrix(nvpl_scomplex_t * A, nvpl_int_t rows, nvpl_int_t cols, nvpl_int_t ld,
        const char * matrix_name, enum CBLAS_ORDER order) {
    printf("%s=", matrix_name);
    if (CblasColMajor == order) {
        for (nvpl_int_t j = 0; j < cols; ++j) {
            printf("\t");
            for (nvpl_int_t i = 0; i < rows; ++i) {
                printf("(%g, %g) ", A[i + ld * j].real, A[i + ld * j].imag);
            }
            printf("\n");
        }
    } else {
        for (nvpl_int_t i = 0; i < rows; ++i) {
            printf("\t");
            for (nvpl_int_t j = 0; j < cols; ++j) {
                printf("(%g, %g) ", A[j + ld * i].real, A[j + ld * i].imag);
            }
            printf("\n");
        }
    }
}

/**
 * \brief      Print data for nvpl_dcomplex_t precision matrix
 *
 * \param      A           The pointer of the matrix
 * \param[in]  rows        The rows of the matrix
 * \param[in]  cols        The columns of the matrix
 * \param[in]  ld          The leading demension of the matrix
 * \param[in]  matrix_name matrix's name
 * \param[in]  order       row-major or column-major matrix
 */

static void print_zmatrix(nvpl_dcomplex_t * A, nvpl_int_t rows, nvpl_int_t cols, nvpl_int_t ld,
        const char * matrix_name, enum CBLAS_ORDER order) {
    printf("%s=", matrix_name);
    if (CblasColMajor == order) {
        for (nvpl_int_t j = 0; j < cols; ++j) {
            printf("\t");
            for (nvpl_int_t i = 0; i < rows; ++i) {
                printf("(%g, %g) ", A[i + ld * j].real, A[i + ld * j].imag);
            }
            printf("\n");
        }
    } else {
        for (nvpl_int_t i = 0; i < rows; ++i) {
            printf("\t");
            for (nvpl_int_t j = 0; j < cols; ++j) {
                printf("(%g, %g) ", A[j + ld * i].real, A[j + ld * i].imag);
            }
            printf("\n");
        }
    }
}

/**
 * \brief      Print data for single precision banded matrix 
 *
 * \param      A           The pointer of the matrix
 * \param[in]  rows        The rows of the matrix
 * \param[in]  cols        The columns of the matrix
 * \param[in]  ld          The leading demension of the matrix
 * \param[in]  matrix_name matrix's name
 * \param[in]  order       row-major or column-major matrix
 */
static void print_sbandmatrix(float * A, nvpl_int_t kl, nvpl_int_t ku,
                        nvpl_int_t rows, nvpl_int_t cols,
                        nvpl_int_t ld, char * matrix_name, enum CBLAS_ORDER order) {
    
    nvpl_int_t  i, j, l;
    nvpl_int_t  kl1, ku1, i_start, j_start, j_end, kl_rows, ku_rows;

    printf("%s=", matrix_name);
    if (CblasRowMajor == order) {
        for( i = 0; i < MIN( rows, cols ); i++ ) {
            printf("\t");
            kl1  = ( i - kl <= 0 ) ? i : kl;
            ku1  = ( i + ku >= cols ) ? MAX(0,cols-i-1) : ku;
            j_start = kl - kl1;
            j_end = j_start + kl1 + ku1;
            for( l=0; l < j_start; l++ )
                printf("        ");
            for( l=0; l < j_end-j_start+1; l++ )
                printf("%6.2f  ", A[i*ld + j_start + l]);
            if (j_end + 1 < ld) {
                for( l=j_end + 1; l < ld; l++)
                    printf("        ");
            }
            printf("\n");
        }
        j_start = kl - cols;
        for( i = MIN( rows, cols ); i < MIN( rows, MIN( rows, cols ) + kl); i++ ) {
            printf("\t");
            j_start = ( j_start < 0 ) ? 0 : j_start;
            kl1  = cols - i + kl;
            j_end = j_start + kl1 - 1;
            for( l=0; l < j_start; l++ )
                printf("        ");
            for( l=j_start; l < j_end-j_start+1; l++ )
                printf("%6.2f  ", A[i*ld + l]);
            for( l=j_end-j_start+1; l < ld; l++ )
                printf("        ");
            j_start--;
            printf("\n");
        }
    } else {
        i_start = (ku > cols ) ? ku - cols + 1 : 0;
        ku_rows = (ku > cols ) ? cols - 1 : ku;
        j_start = ku_rows;
        for( i = 0; i < ku_rows; i++ ) {
            printf("\t");
            j = j_start*ld;
            for( l=0; l < j_start; l++ )
                printf("        ");
            for( l=0; l < cols-j_start; l++ )
                printf("%6.2f  ", A[i + i_start + j + l*ld] );
            j_start--;
            printf("\n");
        }

        j_end = MIN(rows,cols);
        printf("\t");
        for( l=0; l < j_end; l++ )
            printf("%6.2f  ", A[ku + l*ld] );
        if (j_end < cols) {
            for (l=j_end; l < cols; l++)
                printf("        ");
        }
        printf("\n");

        kl_rows = ( kl <= rows-1 ) ?  kl : rows - 1;
        for ( i = 1; i < kl_rows+1; i++ ) {
            printf("\t");
            kl1 = ( i+j_end <= rows) ? j_end : rows - i;
            for( l=0; l < kl1; l++ )
                printf("%6.2f  ", A[ku + i + l*ld] );
            for( l=kl1; l < cols; l++ )
                printf("        ");
            printf("\n");
        }
    }     
}

/**
 * \brief      Print data for double precision banded matrix
 *
 * \param      A           The pointer of the matrix
 * \param[in]  rows        The rows of the matrix
 * \param[in]  cols        The columns of the matrix
 * \param[in]  ld          The leading demension of the matrix
 * \param[in]  matrix_name matrix's name
 * \param[in]  order       row-major or column-major matrix
 */
static void print_dbandmatrix(double * A, nvpl_int_t kl, nvpl_int_t ku,
                        nvpl_int_t rows, nvpl_int_t cols,
                        nvpl_int_t ld, char * matrix_name, enum CBLAS_ORDER order) {
    
    nvpl_int_t  i, j, l;
    nvpl_int_t  kl1, ku1, i_start, j_start, j_end, kl_rows, ku_rows;

    printf("%s=", matrix_name);
    if (CblasRowMajor == order) {
        for( i = 0; i < MIN( rows, cols ); i++ ) {
            printf("\t");
            kl1  = ( i - kl <= 0 ) ? i : kl;
            ku1  = ( i + ku >= cols ) ? MAX(0,cols-i-1) : ku;
            j_start = kl - kl1;
            j_end = j_start + kl1 + ku1;
            for( l=0; l < j_start; l++ )
                printf("        ");
            for( l=0; l < j_end-j_start+1; l++ )
                printf("%6.2f  ", A[i*ld + j_start + l]);
            if (j_end + 1 < ld) {
                for( l=j_end + 1; l < ld; l++)
                    printf("        ");
            }
            printf("\n");
        }
        j_start = kl - cols;
        for( i = MIN( rows, cols ); i < MIN( rows, MIN( rows, cols ) + kl); i++ ) {
            printf("\t");
            j_start = ( j_start < 0 ) ? 0 : j_start;
            kl1  = cols - i + kl;
            j_end = j_start + kl1 - 1;
            for( l=0; l < j_start; l++ )
                printf("        ");
            for( l=j_start; l < j_end-j_start+1; l++ )
                printf("%6.2f  ", A[i*ld + l]);
            for( l=j_end-j_start+1; l < ld; l++ )
                printf("        ");
            j_start--;
            printf("\n");
        }
    } else {
        i_start = (ku > cols ) ? ku - cols + 1 : 0;
        ku_rows = (ku > cols ) ? cols - 1 : ku;
        j_start = ku_rows;
        for( i = 0; i < ku_rows; i++ ) {
            printf("\t");
            j = j_start*ld;
            for( l=0; l < j_start; l++ )
                printf("        ");
            for( l=0; l < cols-j_start; l++ )
                printf("%6.2f  ", A[i + i_start + j + l*ld] );
            j_start--;
            printf("\n");
        }

        j_end = MIN(rows,cols);
        printf("\t");
        for( l=0; l < j_end; l++ )
            printf("%6.2f  ", A[ku + l*ld] );
        if (j_end < cols) {
            for (l=j_end; l < cols; l++)
                printf("        ");
        }
        printf("\n");

        kl_rows = ( kl <= rows-1 ) ?  kl : rows - 1;
        for ( i = 1; i < kl_rows+1; i++ ) {
            printf("\t");
            kl1 = ( i+j_end <= rows) ? j_end : rows - i;
            for( l=0; l < kl1; l++ )
                printf("%6.2f  ", A[ku + i + l*ld] );
            for( l=kl1; l < cols; l++ )
                printf("        ");
            printf("\n");
        }
    }     
}

/**
 * \brief      Print data for nvpl_scomplex_t precision banded matrix
 *
 * \param      A           The pointer of the matrix
 * \param[in]  rows        The rows of the matrix
 * \param[in]  cols        The columns of the matrix
 * \param[in]  ld          The leading demension of the matrix
 * \param[in]  matrix_name matrix's name
 * \param[in]  order       row-major or column-major matrix
 */
static void print_cbandmatrix(nvpl_scomplex_t * A, nvpl_int_t kl, nvpl_int_t ku,
                        nvpl_int_t rows, nvpl_int_t cols,
                        nvpl_int_t ld, char * matrix_name, enum CBLAS_ORDER order) {
    
    nvpl_int_t  i, j, l;
    nvpl_int_t  kl1, ku1, i_start, j_start, j_end, kl_rows, ku_rows;

    printf("%s=", matrix_name);
    if (CblasRowMajor == order) {
        for( i = 0; i < MIN( rows, cols ); i++ ) {
            printf("\t");
            kl1  = ( i - kl <= 0 ) ? i : kl;
            ku1  = ( i + ku >= cols ) ? MAX(0,cols-i-1) : ku;
            j_start = kl - kl1;
            j_end = j_start + kl1 + ku1;
            for( l=0; l < j_start; l++ )
                printf("               ");
            for( l=0; l < j_end-j_start+1; l++ )
                printf("(%5.2f,%5.2f)  ", A[i*ld + j_start + l].real, A[i*ld + j_start + l].imag);
            if (j_end + 1 < ld) {
                for( l=j_end + 1; l < ld; l++)
                    printf("               ");
            }
            printf("\n");
        }
        j_start = kl - cols;
        for( i = MIN( rows, cols ); i < MIN( rows, MIN( rows, cols ) + kl); i++ ) {
            printf("\t");
            j_start = ( j_start < 0 ) ? 0 : j_start;
            kl1  = cols - i + kl;
            j_end = j_start + kl1 - 1;
            for( l=0; l < j_start; l++ )
                printf("               ");
            for( l=j_start; l < j_end-j_start+1; l++ )
                printf("(%5.2f,%5.2f)  ", A[i*ld + l].real, A[i*ld + l].imag);
            for( l=j_end-j_start+1; l < ld; l++ )
                printf("               ");
            j_start--;
            printf("\n");
        }
    } else {
        i_start = (ku > cols ) ? ku - cols + 1 : 0;
        ku_rows = (ku > cols ) ? cols - 1 : ku;
        j_start = ku_rows;
        for( i = 0; i < ku_rows; i++ ) {
            printf("\t");
            j = j_start*ld;
            for( l=0; l < j_start; l++ )
                printf("               ");
            for( l=0; l < cols-j_start; l++ )
                printf("(%5.2f,%5.2f)  ", A[i + i_start + j + l*ld].real, A[i + i_start + j + l*ld].imag);
            j_start--;
            printf("\n");
        }

        j_end = MIN(rows,cols);
        printf("\t");
        for( l=0; l < j_end; l++ )
            printf("(%5.2f,%5.2f)  ", A[ku + l*ld].real, A[ku + l*ld].imag);
        if (j_end < cols) {
            for (l=j_end; l < cols; l++)
                printf("               ");
        }
        printf("\n");

        kl_rows = ( kl <= rows-1 ) ?  kl : rows - 1;
        for ( i = 1; i < kl_rows+1; i++ ) {
            printf("\t");
            kl1 = ( i+j_end <= rows) ? j_end : rows - i;
            for( l=0; l < kl1; l++ )
                printf("(%5.2f,%5.2f)  ", A[ku + i + l*ld].real, A[ku + i + l*ld].imag);
            for( l=kl1; l < cols; l++ )
                printf("               ");
            printf("\n");
        }
    }     
}

/**
 * \brief      Print data for nvpl_dcomplex_t precision banded matrix
 *
 * \param      A           The pointer of the matrix
 * \param[in]  rows        The rows of the matrix
 * \param[in]  cols        The columns of the matrix
 * \param[in]  ld          The leading demension of the matrix
 * \param[in]  matrix_name matrix's name
 * \param[in]  order       row-major or column-major matrix
 */
static void print_zbandmatrix(nvpl_dcomplex_t * A, nvpl_int_t kl, nvpl_int_t ku,
                        nvpl_int_t rows, nvpl_int_t cols,
                        nvpl_int_t ld, char * matrix_name, enum CBLAS_ORDER order) {
    
    nvpl_int_t  i, j, l;
    nvpl_int_t  kl1, ku1, i_start, j_start, j_end, kl_rows, ku_rows;

    printf("%s=", matrix_name);
    if (CblasRowMajor == order) {
        for( i = 0; i < MIN( rows, cols ); i++ ) {
            printf("\t");
            kl1  = ( i - kl <= 0 ) ? i : kl;
            ku1  = ( i + ku >= cols ) ? MAX(0,cols-i-1) : ku;
            j_start = kl - kl1;
            j_end = j_start + kl1 + ku1;
            for( l=0; l < j_start; l++ )
                printf("               ");
            for( l=0; l < j_end-j_start+1; l++ )
                printf("(%5.2f,%5.2f)  ", A[i*ld + j_start + l].real, A[i*ld + j_start + l].imag);
            if (j_end + 1 < ld) {
                for( l=j_end + 1; l < ld; l++)
                    printf("               ");
            }
            printf("\n");
        }
        j_start = kl - cols;
        for( i = MIN( rows, cols ); i < MIN( rows, MIN( rows, cols ) + kl); i++ ) {
            printf("\t");
            j_start = ( j_start < 0 ) ? 0 : j_start;
            kl1  = cols - i + kl;
            j_end = j_start + kl1 - 1;
            for( l=0; l < j_start; l++ )
                printf("               ");
            for( l=j_start; l < j_end-j_start+1; l++ )
                printf("(%5.2f,%5.2f)  ", A[i*ld + l].real, A[i*ld + l].imag);
            for( l=j_end-j_start+1; l < ld; l++ )
                printf("               ");
            j_start--;
            printf("\n");
        }
    } else {
        i_start = (ku > cols ) ? ku - cols + 1 : 0;
        ku_rows = (ku > cols ) ? cols - 1 : ku;
        j_start = ku_rows;
        for( i = 0; i < ku_rows; i++ ) {
            printf("\t");
            j = j_start*ld;
            for( l=0; l < j_start; l++ )
                printf("               ");
            for( l=0; l < cols-j_start; l++ )
                printf("(%5.2f,%5.2f)  ", A[i + i_start + j + l*ld].real, A[i + i_start + j + l*ld].imag);
            j_start--;
            printf("\n");
        }

        j_end = MIN(rows,cols);
        printf("\t");
        for( l=0; l < j_end; l++ )
            printf("(%5.2f,%5.2f)  ", A[ku + l*ld].real, A[ku + l*ld].imag);
        if (j_end < cols) {
            for (l=j_end; l < cols; l++)
                printf("               ");
        }
        printf("\n");

        kl_rows = ( kl <= rows-1 ) ?  kl : rows - 1;
        for ( i = 1; i < kl_rows+1; i++ ) {
            printf("\t");
            kl1 = ( i+j_end <= rows) ? j_end : rows - i;
            for( l=0; l < kl1; l++ )
                printf("(%5.2f,%5.2f)  ", A[ku + i + l*ld].real, A[ku + i + l*ld].imag);
            for( l=kl1; l < cols; l++ )
                printf("               ");
            printf("\n");
        }
    }     
}

static char transpose_to_char(enum CBLAS_TRANSPOSE op) {
  switch (op) {
    case CblasTrans:
      return 'T';
    case CblasConjTrans:
      return 'C';
    case CblasNoTrans:
      return 'N';
    default:
      return 'T';
  }
}

static char order_to_char(enum CBLAS_ORDER op) {
  switch (op) {
    case CblasRowMajor:
      return 'R';
    case CblasColMajor:
      return 'C';
    default:
      return 'C';
  }
}

static char uplo_to_char(enum CBLAS_UPLO op) {
  switch (op) {
    case CblasUpper:
      return 'U';
    case CblasLower:
      return 'L';
    default:
      return 'L';
  }
}

static char diag_to_char(enum CBLAS_DIAG op) {
  switch (op) {
    case CblasNonUnit:
      return 'N';
    case CblasUnit:
      return 'U';
    default:
      return 'N';
  }
}

static char side_to_char(enum CBLAS_SIDE op) {
  switch(op) {
    case CblasLeft:
      return 'L';
    case CblasRight:
      return 'R';
    default:
      return 'L';
  }
}
#endif // #ifndef EXAMPLE_HELPER_H
