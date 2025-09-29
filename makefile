run:
	./main

build:
	g++ -o main main.cpp

all: build run

clean:
	rm -f main
