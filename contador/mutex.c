
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Contador compartilhado entre as threads
volatile long long contador = 0;

// Mutex para proteger o contador
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Funcao executada pelas threads
void *incrementar(void *arg) {
    long long quantidade = *(long long *)arg;

    for (long long i = 0; i < quantidade; i++) {
        pthread_mutex_lock(&mutex);
        contador++;
        pthread_mutex_unlock(&mutex);
    }

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
    long long quantidades[8];

    // Quantidade de incrementos por thread
    long long porThread = 100000000LL / p;
    long long resto = 100000000LL % p;

    struct timespec inicio, fim;

    // Inicia antes de criar as threads
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < p; i++) {
        quantidades[i] = porThread;

        // Distribui os incrementos restantes
        if (i < resto) {
            quantidades[i]++;
        }

        int erro = pthread_create(
            &threads[i], NULL, incrementar, &quantidades[i]
        );

        if (erro != 0) {
            printf("Erro ao criar thread %d\n", i);

            for (int j = 0; j < i; j++) {
                pthread_join(threads[j], NULL);
            }

            pthread_mutex_destroy(&mutex);
            return 1;
        }
    }

    // Espera todas as threads
    for (int i = 0; i < p; i++) {
        pthread_join(threads[i], NULL);
    }

    // Finaliza a medicao
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo =
        (fim.tv_sec - inicio.tv_sec) * 1000.0 +
        (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    printf("Versao: paralela com mutex\n");
    printf("Threads: %d\n", p);
    printf("Contador: %lld\n", contador);
    printf("Tempo: %.3f ms\n", tempo);
    printf("Corretude: %s\n",
           contador == 100000000LL ? "OK" : "ERRO");

    pthread_mutex_destroy(&mutex);

    return contador == 100000000LL ? 0 : 1;
}
