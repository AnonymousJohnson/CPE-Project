main: main.o network.o
	g++ -g  main.o network.o -o main -lpthread
	
main.o: network.h main.cpp
	g++ -g -c main.cpp 
	
network.o: network.h network.cpp
	g++ -g -c network.cpp
	
make clean:
	rm *.o main
