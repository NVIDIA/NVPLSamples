#include <nvpl_lapack.h>
#include <omp.h>
#include <stdio.h>

/* Auxiliary routine: printing a vector of floats */
void print_svector(const char* desc, nvpl_int_t n, const float* vec) {
	nvpl_int_t j;
	printf("\n %s\n", desc);
	for (j = 0; j < n; j++)
		printf(" %6.2f", vec[j]);
	printf("\n");
}

/* An example of a user routine which might call parallel NVPL LAPACK
   routines inside. This example shows the recommended practice of
   saving the (potentially) previous local settings for LAPACK and
   restoring them after the call */
void lib_call_with_internal_nvpl_lapack_usage(int nthreads, int n_, float alpha,
                                              float* x, int incx_, float tau) {
	nvpl_int_t n = n_;
	nvpl_int_t incx = incx_;

	printf(
	    "Calling lib_call_with_internal_nvpl_lapack_usage() with "
	    "nthreads = %d\n",
	    nthreads);

	/* Setting local number of threads for NVPl LAPACK to use and
     saving previously set nthreads for NVPL LAPACK */
	int nthreads_prev = nvpl_lapack_set_num_threads_local(nthreads);
	/* Ignore NVPL LAPACK specific environment variables within the library */
	auto env_mode_prev =
	    nvpl_lapack_set_env_mode_local(NVPL_LAPACK_ENV_MODE_IGNORE);
	slarfg_(&n, &alpha, x, &incx, &tau);
	/* Restoring the previous local setting */
	nvpl_lapack_set_num_threads_local(nthreads_prev);
	nvpl_lapack_set_env_mode_local(env_mode_prev);

	printf("(inside the library calls LAPACK setting for nthreads = %d \n",
	       nvpl_lapack_get_max_threads());
}

/* An example of a potentially error-prone user routine which might call
   parallel NVPL LAPACK routines inside */
void lib_call_with_internal_nvpl_lapack_usage_bad(int nthreads, int n_,
                                                  float alpha, float* x,
                                                  int incx_, float tau) {
	nvpl_int_t n = n_;
	nvpl_int_t incx = incx_;

	printf(
	    "Calling lib_call_with_internal_nvpl_lapack_usage_bad() "
	    "with nthreads = %d\n",
	    nthreads);

	/* Just setting the local numnber of threads for NVPl LAPACK to use.
     This overwrites the previous settings and can lead to unexpected side
     effects. */
	nvpl_lapack_set_num_threads_local(nthreads);
	slarfg_(&n, &alpha, x, &incx, &tau);

	printf("(inside the library calls LAPACK setting for nthreads = %d \n",
	       nvpl_lapack_get_max_threads());
}

/* Main program */
int main() {
	int library_nthreads = 6;
	nvpl_int_t n, incx;
	float alpha, tau;

	/* Initialization */
	float x0[4] = {1, 2, 3, 4};
	float x1[4] = {1, 2, 3, 4};

	n = 5;
	incx = 1;
	alpha = 1.0;
	tau = 3.0;

	printf("NVPL LAPACK version: %d \n", nvpl_lapack_get_version());
	printf("sizeof(nvpl_int_t) = %d \n", (int) sizeof(nvpl_int_t));

	print_svector("Initial vector #0", n - 1, x0);
	print_svector("Initial vector #1", n - 1, x1);
	printf("\n");

	printf("OpenMP max number of threads = %d \n", omp_get_max_threads());

	/* Setting the global upper limit on the number of threads
     which can be used by NVPL LAPACK */
	nvpl_lapack_set_num_threads(7);

	/* Local setting takes the highest precedence and shadows the previously
     set global limit */
	nvpl_lapack_set_num_threads_local(4);
	printf("(before the library calls LAPACK setting for nthreads = %d \n",
	       nvpl_lapack_get_max_threads());

	lib_call_with_internal_nvpl_lapack_usage(library_nthreads, n, alpha, x0,
	                                         incx, tau);

	printf(
	    "(after calling lib_call_with_internal_nvpl_lapack_usage() LAPACK "
	    "setting for nthreads = %d \n",
	    nvpl_lapack_get_max_threads());

	lib_call_with_internal_nvpl_lapack_usage_bad(library_nthreads, n, alpha, x1,
	                                             incx, tau);

	printf(
	    "(after calling lib_call_with_internal_nvpl_lapack_usage_bad() "
	    "LAPACK setting for nthreads = %d \n",
	    nvpl_lapack_get_max_threads());

	/* Print generated reflectors */
	print_svector("Householder reflector #0", n - 1, x0);
	print_svector("Householder reflector #1", n - 1, x1);
	printf("\n");

	return 0;
}
