CC = gcc
CFLAGS = -ansi -Wall -pedantic -std=c99

sim:	sim.o plants.o exploiters.o mutualists.o landscape.o randpois.o randunif.o FFT.o as183.o
	$(CC) $(CFLAGS) -o sim sim.o plants.o exploiters.o mutualists.o landscape.o randpois.o randunif.o FFT.o as183.o -lm

sim.o: sim.c
	$(CC) $(CFLAGS) -c sim.c

plants.o: plants.c
	$(CC) $(CFLAGS) -c plants.c

exploiters.o: exploiters.c
	$(CC) $(CFLAGS) -c exploiters.c

mutualists.o: mutualists.c
	$(CC) $(CFLAGS) -c mutualists.c

landscape.o: landscape.c
	$(CC) $(CFLAGS) -c landscape.c

randpois.o: randpois.c
	$(CC) $(CFLAGS) -c randpois.c

randunif.o: randunif.c
	$(CC) $(CFLAGS) -c randunif.c

FFT.o: FFT.c
	$(CC) $(CFLAGS) -c FFT.c

as183.o: as183.c
	$(CC) $(CFLAGS) -c as183.c

clean:
	rm -rf *.o *.dat daj
