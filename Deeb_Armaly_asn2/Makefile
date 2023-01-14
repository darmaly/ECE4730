CFLAGS = -g -Wall -Wstrict-prototypes  
PROGS = runmain  
OBJECTS = mm-parallel.o  
LDFLAGS = -lm  
CC = gcc  
MCC = mpicc

all: mm-parallel.o make-matrix.o print-matrix.o mm-serial.o 
  
mm-parallel.o: mm-parallel.c  
	$(MCC) $(CFLAGS) $(LDFLAGS) mm-parallel.c -o mm-parallel 
 
mm-serial.o: mm-serial.c
	$(CC) $(CFLAGS) $(LDFLAGS) mm-serial.c -o mm-serial

print-matrix.o: print-matrix.c
	$(CC) $(CFLAGS) print-matrix.c -o print-matrix

make-matrix.o: make-matrix.c
	$(CC) $(CFLAGS) make-matrix.c -o make-matrix
 
clean:  
	rm -f $(PROGS) *.o core* 	