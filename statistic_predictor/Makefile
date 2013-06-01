all: statistics_predictor
CC = g++
INSTDIR = 
INCLUDE = .
CFLAGS = 

statistics_predictor: main.o stock.o
	$(CC) -o a.out stock.o main.o

main.o: main.cc
	$(CC) -c main.cc

stock.o: stock.cc
	$(CC) -c stock.cc

clean: 
	rm a.out main.o stock.o

#install: socket_test
#	if( -d $(INSTDIR)) then\
#		echo "test"\
#	endif

