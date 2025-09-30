#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <omp.h>

static void jacobi(
    std::size_t n,
    double alpha,
    int max_iter,
    double tol,
    int& iters,
    double& err
) {
    const double off = -alpha / double(n - 1);
    const double b = 1.0 - alpha;

    std::vector<double> x(n, 0.0);
    err = INFINITY;

    int teste = 0;

    for (iters = 1; iters <= max_iter; ++iters) {
        double sum_x = 0.0;
        for (std::size_t i = 0; i < n; ++i) sum_x += x[i];

        double maxdiff = 0.0;
        #pragma omp parallel for num_threads(18)
        for (std::size_t i = 0; i < n; ++i) {
            double old = x[i];
            double val = b - off * (sum_x - old);
            x[i] = val;
            double d = std::fabs(val - old);
            if (d > maxdiff) maxdiff = d;
        }
        err = maxdiff;
        if (err < tol) teste = 1;
    }

}

static void jacobi_multi_parallel(
    std::size_t n,
    double alpha,
    int max_iter,
    double tol,
    int& iters,
    double& err
) {
    const double off = -alpha / double(n - 1);
    const double b = 1.0 - alpha;

    std::vector<double> x(n, 0.0);
    std::vector<double> x_new(n, 0.0);
    err = INFINITY;

   for (iters = 1; iters <= max_iter; ++iters) {
        double sum_x = 0.0;
        
        // Calcular soma de x em paralelo
        #pragma omp parallel for reduction(+:sum_x) num_threads(18)
        for (std::size_t i = 0; i < n; ++i) {
            sum_x += x[i];
        }

        // Atualizar valores em paralelo
        #pragma omp parallel for num_threads(18)
        for (std::size_t i = 0; i < n; ++i) {
            double old = x[i];
            x_new[i] = b - off * (sum_x - old);
        }

        // Calcular erro máximo em paralelo
        double maxdiff = 0.0;
        #pragma omp parallel for reduction(max:maxdiff) num_threads(18)
        for (std::size_t i = 0; i < n; ++i) {
            double d = std::fabs(x_new[i] - x[i]);
            if (d > maxdiff) maxdiff = d;
        }

        // Copiar novos valores
        #pragma omp parallel for num_threads(18)
        for (std::size_t i = 0; i < n; ++i) {
            x[i] = x_new[i];
        }

        err = maxdiff;
        if (err < tol) break;
    }

}

int main() {
    std::size_t n;
    double alpha;

    std::cout << "Digite n e alpha (ex: 200000 0.9999): " << std::flush;
    if (!(std::cin >> n >> alpha) || n < 2 || alpha <= 0.0 || alpha >= 1.0) {
        std::cout << "Entrada invalida (n>=2 e 0<alpha<1).\n" << std::flush;
        return 1;
    }

    const int max_iter = 1000000;
    const double tol = 1e-8;

    auto t0 = std::chrono::high_resolution_clock::now();
    int iters = 0; double err = 0.0;
    jacobi(n, alpha, max_iter, tol, iters, err);
    auto t1 = std::chrono::high_resolution_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    double mb = (1.0 * n * sizeof(double)) / (1024.0 * 1024.0);

    std::cout << "\nResumo:\n" << std::flush;
    std::cout << " n = " << n << "\n" << std::flush;
    std::cout << " alpha = " << alpha << "\n" << std::flush;
    std::cout << " iteracoes = " << iters << "\n" << std::flush;
    std::cout << " erro (||x_k - x_{k-1}||_inf) = " << err << "\n" << std::flush;
    std::cout << " tempo = " << ms << " ms (~" << (ms / 1000.0) << " s)\n" << std::flush;
    std::cout << " memoria estimada (vetor x) ~ " << mb << " MB\n" << std::flush;

    return 0;
}
