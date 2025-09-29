# Gauss-Jacobi With Parallelization

Implementação do método de Jacobi “matrix-free” para um sistema linear com estrutura especial:
- A tem 1 na diagonal e −alpha/(n−1) fora da diagonal.
- b tem todos os elementos iguais a 1 − alpha.
- O algoritmo evita alocar a matriz A (usa apenas o vetor x), reduzindo memória e custo por iteração para O(n).

## Por que “matrix-free”?

- Memória: não guarda A (n×n). Em vez de O(n²) doubles, usa O(n) (apenas x e constantes).
- Desempenho: cada iteração faz:
  - 1 redução (soma global de x) → O(n)
  - 1 varredura para atualizar x e medir o erro → O(n)
  Total O(n) por iteração, contra O(n²) se A fosse percorrida explicitamente.
- Equivalência: como A_ii=1 e A_ij=off (constante) para i≠j, a soma ∑_{j≠i} A_ij x_j = off·(sum_x − x_i). Isso permite computar Jacobi sem acessar A.
- Escalabilidade: cabe em memória e paraleliza com reduções (soma e máximo) sem corrida de dados.

## Termos e parâmetros

- n: dimensão do sistema (tamanho de x e de A: n×n).
- alpha (0<alpha<1): parâmetro do problema; define off = −alpha/(n−1) e b = 1−alpha.
- off: valor constante fora da diagonal de A.
- b: todos os termos independentes iguais a 1−alpha.
- x: vetor de incógnitas (inicializado em 0).
- max_iter: limite máximo de iterações.
- tol: tolerância de parada.
- err: norma infinito da variação entre iterações, err = ||x^{k+1} − x^k||_∞ (máxima diferença absoluta por componente).
- iters: número de iterações realizadas.
- Convergência: como |off|·(n−1) = alpha < 1, A é estritamente diagonal dominante; Jacobi converge.

## Como compilar

Requisitos: compilador C++17.

- Windows (MinGW/GCC):
```powershell
g++ -O3 -std=c++17 -o main.exe main.cpp
```

- Windows (MSVC):
```powershell
cl /O2 /std:c++17 main.cpp
```

- Linux (Makefile incluso):
```bash
make
```

Opcional (paralelização OpenMP):
- GCC/Clang: adicione `-fopenmp`
- MSVC: adicione `/openmp`

## Como executar

Execute o binário e informe n e alpha no prompt:

```powershell
.\main.exe
Digite n e alpha (ex: 200000 0.9999): 200000 0.99999
```

Condições de entrada: n ≥ 2 e 0 < alpha < 1.

## Benchmark (exemplo do autor)

- Parâmetros: n = 200000, alpha = 0.99999
- Tempo: ~14 minutos
- Memória observada: ~16 GB
- Observação: tempos/memória variam conforme hardware e ambiente. A complexidade do algoritmo é O(n) em memória e tempo por iteração.

## Detalhes de I/O

O programa usa:
- `std::ios::sync_with_stdio(false)` e `std::cin.tie(nullptr)` para acelerar I/O com streams C++ (não misturar com printf/scanf).

## Paralelização (resumo)

- Redução da soma: `sum_x = sum(x[i])`
- Atualização e erro (redução do máximo): atualiza cada `x[i]` e calcula `maxdiff`
- Diretivas sugeridas: `#pragma omp parallel for reduction(+:sum_x)` e `reduction(max:maxdiff)`
