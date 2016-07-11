OBJ = main.o fast_ftrl.o utils.o
CFLAGS = -g -Wall -pthread -std=c++0x
CC = g++ $(CFLAGS)

ftrl: ${OBJ}
	${CC} -o ftrl ${OBJ}

main.o: main.cc ftrl.h utils.h
	${CC} -c main.cc

fast_ftrl.o: fast_ftrl.cc fast_ftrl.h
	${CC} -c fast_ftrl.cc

utils.o: utils.cc utils.h
	${CC} -c utils.cc

clean:
	rm *.o


