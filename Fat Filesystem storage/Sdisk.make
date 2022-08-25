Sdisk: main.o Sdisk.o
	g++ main.o Sdisk.o -o Sdisk

Sdisk.o: Sdisk.cpp
	g++ -c Sdisk.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o