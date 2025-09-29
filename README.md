# Gauss-Jacobi With Parallelization

Implementação do método de Jacobi “matrix-free” para um sistema linear com estrutura especial:
- A tem 1 na diagonal e −alpha/(n−1) fora da diagonal.
- b tem todos os elementos iguais a 1 − alpha.
- O algoritmo evita alocar a matriz A (usa apenas o vetor x), reduzindo memória e custo por iteração para O(n).

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

Observação: o Makefile usa comandos de Unix. No Windows, utilize MinGW/MSYS ou compile manualmente como acima.

## Como executar

Execute o binário e informe n e alpha no prompt:

```powershell
.\main.exe
Digite n e alpha (ex: 200000 0.9999): 200000 0.99999
```

Condições de entrada: n ≥ 2 e 0 < alpha < 1.

A saída mostra um resumo com número de iterações, erro final, tempo total e uma estimativa de memória do vetor x.

## Parâmetros recomendados (benchmark do autor)

Para obter um resultado com:
- tempo aproximado: 14 minutos
- uso de memória: ~16 GB
- parâmetros: n = 200000 e alpha = 0.99999

Observação: tempos e consumo de memória dependem do hardware e do ambiente. Os valores acima foram medidos no ambiente do autor. A estimativa impressa pelo programa para o vetor x é O(n) (por exemplo, ~1.6 MB para n=200000); o consumo total observado pode variar conforme o sistema.

## Paralelização (opcional)

O código pode ser paralelizado com OpenMP usando reduções:
- soma global de x por iteração
- redução do máximo do delta (erro)

Ative com `-fopenmp` (GCC/Clang) ou `/openmp` (MSVC) e diretivas `#pragma omp parallel for reduction(...)`.

## Licença

Livre uso acadêmico/educacional (defina aqui a licença desejada).
