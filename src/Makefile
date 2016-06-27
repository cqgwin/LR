main: main.o ftrl.o utils.o
	cc -o main main.o ftrl.o utils.o

main.o: main.cc ftrl.h utils.h
	cc -c main.cc

frtl.o: ftrl.h ftrl.cc
	cc -c ftrl.cc

utils.o: utils.cc utils.h
	cc -c utils.cc

clean:
	rm main.o ftrl.o utils.o
