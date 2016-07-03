OBJ = main.o ftrl.o utils.o
CFLAGS = -g -Wall -pthread -std=c++0x
CC = g++ $(CFLAGS)

ftrl: ${OBJ}
	${CC} -o ftrl ${OBJ}

main.o: main.cc ftrl.h utils.h
	${CC} -c main.cc

ftrl.o: ftrl.cc ftrl.h
	${CC} -c ftrl.cc

utils.o: utils.cc utils.h
	${CC} -c utils.cc

clean:
	rm *.o


