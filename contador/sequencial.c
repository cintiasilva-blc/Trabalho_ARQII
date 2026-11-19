#include <stdio.h>
#include <time.h>

int main() {
    volatile long long contador = 0;
    struct timespec inicio, fim;

    // Inicia a medicao
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Realiza 100 milhoes de incrementos
    for (long long i = 0; i < 100000000LL; i++) {
        contador++;
    }

    // Finaliza a medicao
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo =
        (fim.tv_sec - inicio.tv_sec) * 1000.0 +
        (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    printf("Versao: sequencial\n");
    printf("Contador: %lld\n", contador);
    printf("Tempo: %.3f ms\n", tempo);
    printf("Corretude: %s\n",
           contador == 100000000LL ? "OK" : "ERRO");

    return contador == 100000000LL ? 0 : 1;
}
