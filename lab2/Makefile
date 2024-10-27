# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
INC = -I../pomiar_czasu

# biblioteki
LIB = -L../pomiar_czasu -lpomiar_czasu -lm

# zaleznosci i komendy
fork: fork.o
	$(LOADER) $(OPT) fork.o -o fork $(LIB)

# jak uzyskac plik fork.o ?
fork.o: fork.c ../pomiar_czasu/pomiar_czasu.h program
	$(CCOMP) -c $(OPT) fork.c $(INC)

# zaleznosci i komendy
clone: clone.o
	$(LOADER) $(OPT) clone.o -o clone $(LIB)

# jak uzyskac plik clone.o ?
clone.o: clone.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone.c $(INC)

test: test.o
	$(LOADER) $(OPT) test.o -o test $(LIB)

test.o: test.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) test.c $(INC)

clean:
	rm -f *.o fork clone test program

program.o: program.c
	$(CCOMP) -c $(OPT) program.c $(INC)

program: program.o
	$(LOADER) $(OPT) program.o -o program $(LIB)