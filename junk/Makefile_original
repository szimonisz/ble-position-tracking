CC=gcc
CFLAGS= -I.
DEPS = trilatmath.h
OBJ = trilateration-server.o trilatmath.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

trilateration-server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f trilateration-server trilateration-server.o trilatmath.o

