SRC = main.cpp
SRC-PARALLEL = parallel-main.cpp

compile:
	g++ -fopenmp $(SRC) -o app

clean:
	rm -f app


compile-parallel:
	g++ -fopenmp $(SRC-PARALLEL) -o app-parallel