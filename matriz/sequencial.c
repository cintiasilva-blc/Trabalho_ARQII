#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s tamanho\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    if (n != 256 && n != 512 && n != 1024) {
        printf("Tamanho invalido.\n");
        return 1;
    }

    // Aloca espaco para as tres matrizes
    int *A = malloc(n * n * sizeof(int));
    int *B = malloc(n * n * sizeof(int));
     // long long oferece uma margem de segurança maior, já que C recebe 
     // a soma varias multiplicacoes, mas poderiamos usar int também
    long long *C = calloc(n * n, sizeof(long long));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(A);
        free(B);
        free(C);
        return 1;
    }

    char nomeA[50], nomeB[50];

    sprintf(nomeA, "entradas/A_%d.txt", n);
    sprintf(nomeB, "entradas/B_%d.txt", n);

    FILE *arqA = fopen(nomeA, "r");
    FILE *arqB = fopen(nomeB, "r");

    if (arqA == NULL || arqB == NULL) {
        printf("Erro ao abrir as matrizes.\n");
        if (arqA != NULL) fclose(arqA);
        if (arqB != NULL) fclose(arqB);
        free(A);
        free(B);
        free(C);
        return 1;
    }

    // Le os valores das matrizes
    for (int i = 0; i < n * n; i++) {
        if (fscanf(arqA, "%d", &A[i]) != 1 ||
            fscanf(arqB, "%d", &B[i]) != 1) {
            printf("Erro ao ler as matrizes.\n");
            fclose(arqA);
            fclose(arqB);
            free(A);
            free(B);
            free(C);
            return 1;
        }
    }

    fclose(arqA);
    fclose(arqB);

    struct timespec inicio, fim;

    // Inicia a medicao
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Multiplicacao sequencial de matrizes
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long long soma = 0;

            for (int k = 0; k < n; k++) {
                // posicao = linha * n + coluna
                soma += (long long)A[i * n + k] * B[k * n + j];
            }

            C[i * n + j] = soma;
        }
    }

    // Finaliza a medicao
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo_ms =
        (fim.tv_sec - inicio.tv_sec) * 1000.0 +
        (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    printf("Matriz: %d x %d\n", n, n);
    printf("Versao: sequencial\n");
    printf("Tempo: %.3f ms\n", tempo_ms);

    // Utiliza o resultado fora da medicao
    printf("Primeiro elemento de C: %lld\n", C[0]);

    free(A);
    free(B);
    free(C);

    return 0;
}
