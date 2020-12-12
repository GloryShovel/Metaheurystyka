all: mhe

parser.o:
	g++ -c parser.cpp

solution.o:
	g++ -c solution.cpp

specimen.o:
	g++ -c specimen.cpp

population.o:
	g++ -c population.cpp

mhe.o:
	g++ -c mhe.cpp

mhe: parser.o solution.o specimen.o population.o mhe.o
	g++ parser.o solution.o specimen.o population.o mhe.o -o mhe

clean:
	rm -f $(APP) *.o
