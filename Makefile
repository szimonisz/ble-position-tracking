CC=gcc
CFLAGS = -g 

all: trilateration-server

trilateration-server: trilateration-server.o 
	$(CC) -o trilateration-server trilateration-server.o $(LIBS)

clean:
	rm -f trilareation-server trilateration-server.o 
