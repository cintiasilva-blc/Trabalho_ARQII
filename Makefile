CC = gcc
FLAGS = -O2 -pthread

all: matriz/gerador matriz/sequencial matriz/paralelo imagem/sequencial imagem/paralelo contador/sequencial contador/mutex contador/local

matriz/gerador: matriz/gerador.c
	$(CC) $(FLAGS) matriz/gerador.c -o matriz/gerador

matriz/sequencial: matriz/sequencial.c
	$(CC) $(FLAGS) matriz/sequencial.c -o matriz/sequencial

matriz/paralelo: matriz/paralelo.c
	$(CC) $(FLAGS) matriz/paralelo.c -o matriz/paralelo

imagem/sequencial: imagem/sequencial.c imagem/stb_image.h
	$(CC) $(FLAGS) imagem/sequencial.c -o imagem/sequencial -lm

imagem/paralelo: imagem/paralelo.c imagem/stb_image.h
	$(CC) $(FLAGS) imagem/paralelo.c -o imagem/paralelo -lm

contador/sequencial: contador/sequencial.c
	$(CC) $(FLAGS) contador/sequencial.c -o contador/sequencial

contador/mutex: contador/mutex.c
	$(CC) $(FLAGS) contador/mutex.c -o contador/mutex

contador/local: contador/local.c
	$(CC) $(FLAGS) contador/local.c -o contador/local

clean:
	rm -f matriz/gerador matriz/sequencial matriz/paralelo
	rm -f imagem/sequencial imagem/paralelo
	rm -f contador/sequencial contador/mutex contador/local

.PHONY: all clean
