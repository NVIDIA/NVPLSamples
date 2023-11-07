#include <iostream>
#include <random>

#include <nvpl_lapack.h>

int main() {

	const nvpl_int_t m = 512;
	const nvpl_int_t n = 512;
	const nvpl_int_t num_batches = 128;
	const nvpl_int_t min_mn = std::min(m, n);
	std::vector<double> matrices(m * n * num_batches);
	std::vector<nvpl_int_t> ipivs(num_batches * min_mn);
	std::vector<nvpl_int_t> infos(num_batches);

	const double min = 1e-3;
	const double max = 7;
	std::uniform_real_distribution<double> dist(min, max);
	size_t seed = 1;
	std::default_random_engine rng(seed);
	for (auto& e : matrices) {
		e = dist(rng);
	}

	std::cout << "  - begin parallel section..." << std::endl;
#pragma omp parallel for
	for (nvpl_int_t batch_id = 0; batch_id < num_batches; ++batch_id) {
		// Within a parallel section the number of internal threads is limited to one.
		dgetrf_(&m, &n, matrices.data() + batch_id * m * n, &m,
		        ipivs.data() + batch_id * min_mn, infos.data() + batch_id);
	}

	for (nvpl_int_t batch_id = 0; batch_id < num_batches; ++batch_id) {
		if (infos[batch_id] != 0) {
			std::cout << "  - info[" << batch_id << "] = " << infos[batch_id]
			          << std::endl;
		}
	}
}
