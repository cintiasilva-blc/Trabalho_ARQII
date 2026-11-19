#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Representa os dados que cada thread recebera
typedef struct {
    int id;
    int n;
    int p;
    int *A;
    int *B;
    long long *C;
} Dados;

// Funcao executada pelas threads
void *multiplicar(void *arg) {
    Dados *d = (Dados *)arg;

    int n = d->n;
    int inicio = d->id * n / d->p;
    int fim = (d->id + 1) * n / d->p;

    // Cada thread calcula apenas suas linhas
    for (int i = inicio; i < fim; i++) {
        for (int j = 0; j < n; j++) {
            long long soma = 0;

            for (int k = 0; k < n; k++) {
                soma += (long long)d->A[i*n+k] * d->B[k*n+j];
            }

            d->C[i*n+j] = soma;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s tamanho threads\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int p = atoi(argv[2]);

    if ((n != 256 && n != 512 && n != 1024) ||
        (p != 2 && p != 4 && p != 8)) {
        printf("Tamanho ou numero de threads invalido.\n");
        return 1;
    }

    // Aloca as matrizes
    int *A = malloc(n*n*sizeof(int));
    int *B = malloc(n*n*sizeof(int));
    long long *C = calloc(n*n, sizeof(long long));
    long long *referencia = calloc(n*n, sizeof(long long));

    if (!A || !B || !C || !referencia) {
        printf("Erro ao alocar memoria.\n");
        free(A);
        free(B);
        free(C);
        free(referencia);
        return 1;
    }

    char nomeA[50], nomeB[50];
    sprintf(nomeA, "entradas/A_%d.txt", n);
    sprintf(nomeB, "entradas/B_%d.txt", n);

    FILE *arqA = fopen(nomeA, "r");
    FILE *arqB = fopen(nomeB, "r");

    if (!arqA || !arqB) {
        printf("Erro ao abrir matrizes.\n");
        if (arqA) fclose(arqA);
        if (arqB) fclose(arqB);
        free(A);
        free(B);
        free(C);
        free(referencia);
        return 1;
    }

    // Leitura fora da medicao
    for (int i = 0; i < n*n; i++) {
        if (fscanf(arqA, "%d", &A[i]) != 1 ||
            fscanf(arqB, "%d", &B[i]) != 1) {
            printf("Erro ao ler matrizes.\n");
            fclose(arqA);
            fclose(arqB);
            free(A);
            free(B);
            free(C);
            free(referencia);
            return 1;
        }
    }

    fclose(arqA);
    fclose(arqB);

    pthread_t threads[8];
    Dados dados[8];

    struct timespec inicio, fim;

    // Comeca a medir antes de criar as threads
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < p; i++) {
        dados[i].id = i;
        dados[i].n = n;
        dados[i].p = p;
        dados[i].A = A;
        dados[i].B = B;
        dados[i].C = C;

        int erro = pthread_create(
            &threads[i], NULL, multiplicar, &dados[i]
        );

        if (erro != 0) {
            printf("Erro ao criar thread %d: %d\n", i, erro);

            // Espera as threads ja criadas
            for (int j = 0; j < i; j++)
                pthread_join(threads[j], NULL);

            free(A);
            free(B);
            free(C);
            free(referencia);
            return 1;
        }
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < p; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo_ms =
        (fim.tv_sec - inicio.tv_sec) * 1000.0 +
        (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    // Calcula a referencia sequencial fora da medicao
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long long soma = 0;

            for (int k = 0; k < n; k++) {
                soma += (long long)A[i*n+k] * B[k*n+j];
            }

            referencia[i*n+j] = soma;
        }
    }

    // Verifica se as duas matrizes sao iguais
    int correto = 1;

    for (int i = 0; i < n*n; i++) {
        if (C[i] != referencia[i]) {
            correto = 0;
    }
}

    printf("Matriz: %d x %d\n", n, n);
    printf("Versao: paralela\n");
    printf("Threads: %d\n", p);
    printf("Tempo: %.3f ms\n", tempo_ms);
    printf("Corretude: %s\n", correto ? "OK" : "ERRO");

    free(A);
    free(B);
    free(C);
    free(referencia);

    return correto ? 0 : 1;
}
