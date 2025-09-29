#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

static void jacobi(std::size_t n, double alpha,
                   int max_iter, double tol,
                   int& iters, double& err) {
    const double off = -alpha / double(n - 1);
    const double b = 1.0 - alpha;

    std::vector<double> x(n, 0.0);
    err = INFINITY;

    for (iters = 1; iters <= max_iter; ++iters) {
        double sum_x = 0.0;
        for (std::size_t i = 0; i < n; ++i) sum_x += x[i];

        double maxdiff = 0.0;
        for (std::size_t i = 0; i < n; ++i) {
            double old = x[i];
            double val = b - off * (sum_x - old);
            x[i] = val;
            double d = std::fabs(val - old);
            if (d > maxdiff) maxdiff = d;
        }
        err = maxdiff;
        if (err < tol) break;
    }

}

int main() {
    std::size_t n;
    double alpha;

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cout << "Digite n e alpha (ex: 200000 0.9999): ";
    if (!(std::cin >> n >> alpha) || n < 2 || alpha <= 0.0 || alpha >= 1.0) {
        std::cout << "Entrada invalida (n>=2 e 0<alpha<1).\n";
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

    std::cout << "\nResumo:\n";
    std::cout << " n = " << n << "\n";
    std::cout << " alpha = " << alpha << "\n";
    std::cout << " iteracoes = " << iters << "\n";
    std::cout << " erro (||x_k - x_{k-1}||_inf) = " << err << "\n";
    std::cout << " tempo = " << ms << " ms (~" << (ms / 1000.0) << " s)\n";
    std::cout << " memoria estimada (vetor x) ~ " << mb << " MB\n";

    return 0;
}
