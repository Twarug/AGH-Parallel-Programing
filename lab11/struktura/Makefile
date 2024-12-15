NAZWA = out

# kompilator c
CCOMP = mpicc

# konsolidator
LOADER = mpicc

# program uruchomieniowy MPI
MPI_run = mpiexec #-oversubscribe

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
#INC = -I../pomiar_czasu

# biblioteki
#LIB = -L../pomiar_czasu -lm
LIB = -lm

all: main run 

run: main
	$(MPI_run) -np 4 ./$(NAZWA)

main : main.o
	$(LOADER) $(OPT) main.o -o $(NAZWA) $(LIB)

main.o: main.c 
	$(CCOMP) -c $(OPT) main.c

clean:
	rm -f *.o $(NAZWA)
