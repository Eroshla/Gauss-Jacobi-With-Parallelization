run:
	./main

build:
	g++ -o main main.cpp

make:
	g++ -o main main.cpp
	./main

clean:
	rm -f main
