SRC = main.cpp

compile:
	g++ -fopenmp $(SRC) -o app

clean:
	rm -f app
