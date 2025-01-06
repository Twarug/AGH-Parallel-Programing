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

all: MPI_pi run 

run: MPI_pi
	$(MPI_run) -np 4 ./$(NAZWA)

# zaleznosci i komendy
MPI_pi : MPI_pi.o
	$(LOADER) $(OPT) MPI_pi.o -o $(NAZWA) $(LIB)

# jak uzyskac plik moj_program.o ?
MPI_pi.o: MPI_pi.c 
	$(CCOMP) -c $(OPT) MPI_pi.c

clean:
	rm -f *.o $(NAZWA)
