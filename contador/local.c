#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Informacoes de cada thread
typedef struct {
    long long quantidade;
    long long resultado;
} Dados;

// Funcao executada por cada thread
void *incrementar(void *arg) {
    Dados *d = (Dados *)arg;

    // Cada thread possui seu proprio contador
    volatile long long contador = 0;

    for (long long i = 0; i < d->quantidade; i++) {
        contador++;
    }

    // Grava o resultado apenas no final
    d->resultado = contador;

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s threads\n", argv[0]);
        return 1;
    }

    int p = atoi(argv[1]);

    if (p != 2 && p != 4 && p != 8) {
        printf("Numero de threads invalido.\n");
        return 1;
    }

    pthread_t threads[8];
    Dados dados[8];

    long long porThread = 100000000LL / p;
    long long resto = 100000000LL % p;

    struct timespec inicio, fim;

    // Inicia a medicao antes de criar as threads
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < p; i++) {
        dados[i].quantidade = porThread;

        if (i < resto) {
            dados[i].quantidade++;
        }

        dados[i].resultado = 0;

        int erro = pthread_create(
            &threads[i], NULL, incrementar, &dados[i]
        );

        if (erro != 0) {
            printf("Erro ao criar thread %d\n", i);

            for (int j = 0; j < i; j++) {
                pthread_join(threads[j], NULL);
            }

            return 1;
        }
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < p; i++) {
        pthread_join(threads[i], NULL);
    }

    // Soma os resultados parciais
    long long total = 0;

    for (int i = 0; i < p; i++) {
        total += dados[i].resultado;
    }

    // Finaliza a medicao
    clock_gettime(CLOCK_MONOTONIC, &fim);

    printf("Versao: paralela com contadores locais\n");
    printf("Threads: %d\n", p);
    printf("Contador: %lld\n", total);

    double tempo =
        (fim.tv_sec - inicio.tv_sec) * 1000.0 +
        (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    printf("Tempo: %.3f ms\n", tempo);
    printf("Corretude: %s\n",
           total == 100000000LL ? "OK" : "ERRO");

    return total == 100000000LL ? 0 : 1;
}
