CFLAGS = -g -Wall -Wstrict-prototypes  
PROGS = runmain  
OBJECTS = Jacobi-Parallel.o  
LDFLAGS = -lm  
CC = gcc  
MCC = mpicc


all: Jacobi-Parallel.o make-matrix.o RMS-Serial.o MatrixVector-Serial.o 
  
Jacobi-Parallel.o: Jacobi-Parallel.c  
	$(MCC) $(CFLAGS) $(LDFLAGS) Jacobi-Parallel.c -o Jacobi-Parallel

RMS-Serial.o: RMS-Serial.c
	$(CC) $(CFLAGS) $(LDFLAGS) RMS-Serial.c -o RMS-Serial 
 
MatrixVector-Serial.o: MatrixVector-Serial.c
	$(CC) $(CFLAGS) $(LDFLAGS) MatrixVector-Serial.c -o MatrixVector-Serial

make-vector.o: make-vector.c
	$(CC) $(CFLAGS) make-vector.c -o make-vector

make-matrix.o: make-matrix.c
	$(CC) $(CFLAGS) make-matrix.c -o make-matrix
 
clean:  
	rm -f $(PROGS) *.o core*