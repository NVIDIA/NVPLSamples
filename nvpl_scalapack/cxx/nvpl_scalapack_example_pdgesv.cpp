#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>
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
      nvpl_int_t m{1000}, nrhs{1}, mb{128};

      /// Solve AX = B
      nvpl_int_t mA{0}, nA{0}, mB{0}, nB{0};

      mA = numroc_(&m, &mb, &myrow, &izero, &nprow);
      nA = numroc_(&m, &mb, &mycol, &izero, &npcol);

      mB = numroc_(&m, &mb, &myrow, &izero, &nprow);
      nB = numroc_(&nrhs, &mb, &mycol, &izero, &npcol);

      ///
      /// Allocate and randomize local matrices
      ///
      std::vector<double> A(mA * nA), B(mB * nB), X(mB * nB), Acopy(mA * nA);
      std::default_random_engine generator(mpi_rank);
      std::uniform_real_distribution<double> distribution(0.0, 1.0);

      std::generate(A.begin(), A.end(), [&]() { return distribution(generator); });
      std::generate(B.begin(), B.end(), [&]() { return distribution(generator); });

      std::vector<nvpl_int_t> ipiv(mA + mb);

      ///
      /// Copy A and B
      ///
      std::copy(A.begin(), A.end(), Acopy.begin()); /// A -> Acopy
      std::copy(B.begin(), B.end(), X.begin());     /// B -> X

      ///
      /// Initialize matrix descriptor
      ///
      constexpr nvpl_int_t dlen{9};
      nvpl_int_t info{0}, descA[dlen], descB[dlen];

      nvpl_int_t lddA = std::max<nvpl_int_t>(1, mA);
      nvpl_int_t lddB = std::max<nvpl_int_t>(1, mB);

      descinit_(descA, &m, &m, &mb, &mb, &izero, &izero, &icontxt, &lddA, &info);
      if (info) {
        throw std::runtime_error("Error: failed to descinit on matrix A");
      }
      descinit_(descB, &m, &nrhs, &mb, &mb, &izero, &izero, &icontxt, &lddB, &info);
      if (info) {
        throw std::runtime_error("Error: failed to descinit on matrix B");
      }

      ///
      /// Perform PDGESV
      ///
      double t{0};
      char barrier_scope_all('A');
      {
        constexpr double billion = 1e9;
        struct timespec ts_beg {
        }, ts_end{};

        Cblacs_barrier(icontxt, &barrier_scope_all);
        clock_gettime(CLOCK_MONOTONIC, &ts_beg);
        pdgesv_(&m, &nrhs, A.data(), &ione, &ione, descA, ipiv.data(), X.data(), &ione, &ione, descB, &info);
        Cblacs_barrier(icontxt, &barrier_scope_all);
        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        t = ((static_cast<double>(ts_end.tv_sec) - static_cast<double>(ts_beg.tv_sec)) +
             (static_cast<double>(ts_end.tv_nsec) - static_cast<double>(ts_beg.tv_nsec)) / billion);
      }
      if (info) {
        std::stringstream ss;
        ss << "Error: mpi rank " << mpi_rank << " is failed with info = " << info << std::endl;
        throw std::runtime_error(ss.str());
      }
      if (mpi_rank == 0) {
        std::cout << "time for pdgesv " << t << std::endl;
      }

      ///
      /// Check solution
      ///
      {
        /// workspace to compute norm 'I', see pdlange api reference
        /// https://www.netlib.org/scalapack/explore-html/db/dd0/pdlange_8f_source.html
        nvpl_int_t wlen = numroc_(&m, &mb, &myrow, &izero, &nprow);
        std::vector<double> w(wlen);

        const double epsilon = pdlamch_(&icontxt, "E");                                         // Epsilon
        const double Anorm = pdlange_("I", &m, &m, A.data(), &ione, &ione, descA, w.data());    /// || A ||
        const double Xnorm = pdlange_("I", &m, &nrhs, X.data(), &ione, &ione, descB, w.data()); /// || X ||

        double alpha{1}, beta{-1};
        char transA('N'), transB('N');
        pdgemm_(&transA, &transB, &m, &nrhs, &m, &alpha, Acopy.data(), &ione, &ione, descA, X.data(), &ione, &ione,
                descB, &beta, B.data(), &ione, &ione, descB);
        const double Rnorm = pdlange_("I", &m, &nrhs, B.data(), &ione, &ione, descB, w.data()); /// || A * X - B ||
        const double residual = Rnorm / (Anorm * Xnorm * epsilon * double(m)); /// ||AX-B|| / ( ||X|| ||A|| eps N || )

        const double threshold(10);
        if (mpi_rank == 0) {
          std::cout << "test threshold = " << threshold << std::endl;
          std::cout << "||A * X  - B|| / ( ||X|| * ||A|| * eps * N ) = " << residual << std::endl;
          if (residual < threshold) {
            std::cout << "The answer is correct." << std::endl;
          } else {
            std::cout << "The answer is suspicious." << std::endl;
          }
        }
        if (residual >= threshold) {
          throw std::runtime_error("Error: residual is greater or equal to the test threshold");
        }
      }
    } catch (const std::exception &e) {
      std::cout << "Error: an exception is caught \n" << e.what() << "\n";
      r_val = -1;
    }
    /// gridexit is done by participating procs.
    Cblacs_gridexit(icontxt);
  }

  /// if keep_mpi is true, Cblacs_exit does not invoke mpi_finalize
  Cblacs_exit(keep_mpi);

  return r_val;
}
