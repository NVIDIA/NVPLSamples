#include <algorithm>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

#include "nvpl_scalapack.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  int r_val(0);

  ///
  /// BLACS initialization
  ///
  constexpr nvpl_int_t keep_mpi{0};
  nvpl_int_t mpi_rank{0}, mpi_size{0};
  Cblacs_pinfo(&mpi_rank, &mpi_size); /// mpi init, rank/size

  nvpl_int_t nprow{2}, npcol{2};
  if (mpi_size < (nprow * npcol)) {
    if (mpi_rank == 0)
      std::cout << "Error: mpi_size (" << mpi_size << ") is smaller than the requested grid size (" << nprow << " x "
                << npcol << ")" << std::endl;
    Cblacs_exit(keep_mpi);
    return -1;
  }

  nvpl_int_t ic{-1}, what{0}, icontxt{0};
  Cblacs_get(ic, what,
             &icontxt); /// get a value for nvpl_internal default, ic{-1} is not used, what{0} is system default context

  nvpl_int_t myrow{0}, mycol{0};
  char grid_layout('C');
  Cblacs_gridinit(&icontxt, &grid_layout, nprow, npcol);    /// create process grid
  Cblacs_gridinfo(icontxt, &nprow, &npcol, &myrow, &mycol); /// set process grid with input icontxt

  if (icontxt < 0) {
    /// do nothing: this process does not participate in computation
  } else {
    try {
      ///
      /// Problem setup
      ///
      nvpl_int_t izero{0}, ione{1};
      nvpl_int_t m{1000}, n{1000}, k{1000}, mb{128};
      char transA = 'N', transB = 'N';

      nvpl_int_t mA{0}, nA{0}, mB{0}, nB{0}, mC{0}, nC{0};
      if (transA == 'N') { /// A = m x k
        mA = numroc_(&m, &mb, &myrow, &izero, &nprow);
        nA = numroc_(&k, &mb, &mycol, &izero, &npcol);
      } else { /// A = k x m
        mA = numroc_(&k, &mb, &myrow, &izero, &nprow);
        nA = numroc_(&m, &mb, &mycol, &izero, &npcol);
      }

      if (transB == 'N') { /// B = k x n
        mB = numroc_(&k, &mb, &myrow, &izero, &nprow);
        nB = numroc_(&n, &mb, &mycol, &izero, &npcol);
      } else { /// B = n x k
        mB = numroc_(&n, &mb, &myrow, &izero, &nprow);
        nB = numroc_(&k, &mb, &mycol, &izero, &npcol);
      }

      {                                                /// C = m x n
        mC = numroc_(&m, &mb, &myrow, &izero, &nprow); // mp
        nC = numroc_(&n, &mb, &mycol, &izero, &npcol); // nq
      }

      ///
      /// Allocate and randomize local matrices
      ///
      std::vector<double> A(mA * nA), B(mB * nB), C(mC * nC);
      std::default_random_engine generator(mpi_rank);
      std::uniform_real_distribution<double> distribution(0.0, 1.0);

      std::generate(A.begin(), A.end(), [&]() { return distribution(generator); });
      std::generate(B.begin(), B.end(), [&]() { return distribution(generator); });
      std::generate(C.begin(), C.end(), [&]() { return distribution(generator); });

      ///
      /// Initialize matrix descriptor
      ///
      constexpr nvpl_int_t dlen{9};
      nvpl_int_t info{0}, descA[dlen], descB[dlen], descC[dlen];

      nvpl_int_t lddA = std::max<nvpl_int_t>(1, mA);
      nvpl_int_t lddB = std::max<nvpl_int_t>(1, mB);
      nvpl_int_t lddC = std::max<nvpl_int_t>(1, mC);
      descinit_(descA, &m, &k, &mb, &mb, &izero, &izero, &icontxt, &lddA, &info);
      if (info) {
        throw std::runtime_error("Error: failed to descinit on matrix A");
      }
      descinit_(descB, &k, &n, &mb, &mb, &izero, &izero, &icontxt, &lddB, &info);
      if (info) {
        throw std::runtime_error("Error: failed to descinit on matrix B");
      }
      descinit_(descC, &m, &n, &mb, &mb, &izero, &izero, &icontxt, &lddC, &info);
      if (info) {
        throw std::runtime_error("Error: failed to descinit on matrix C");
      }

      ///
      /// Perform PDGEMM
      ///
      double t{0};
      char barrier_scope_all('A');
      {
        constexpr double billion = 1e9;
        struct timespec ts_beg {
        }, ts_end{};

        double alpha{1}, beta{1};
        Cblacs_barrier(icontxt, &barrier_scope_all);
        clock_gettime(CLOCK_MONOTONIC, &ts_beg);
        pdgemm_(&transA, &transB, &m, &n, &k, &alpha, A.data(), &ione, &ione, descA, B.data(), &ione, &ione, descB,
                &beta, C.data(), &ione, &ione, descC);
        Cblacs_barrier(icontxt, &barrier_scope_all);
        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        t = ((static_cast<double>(ts_end.tv_sec) - static_cast<double>(ts_beg.tv_sec)) +
             (static_cast<double>(ts_end.tv_nsec) - static_cast<double>(ts_beg.tv_nsec)) / billion);
      }
      if (mpi_rank == 0) {
        std::cout << "time for pdgemm " << t << std::endl;
      }
    } catch (const std::exception &e) {
      std::cout << "Error: an exception is caught \n" << e.what() << "\n";
      r_val = -1;
    }
    /// gridexit is called by participating procs.
    Cblacs_gridexit(icontxt);
  }

  /// if keep_mpi is true, Cblacs_exit does not invoke mpi_finalize
  Cblacs_exit(keep_mpi);

  return r_val;
}
