
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Informacoes de cada thread
typedef struct {
    int id;
    int largura;
    int altura;
    int p;
    unsigned char *entrada;
    unsigned char *saida;
} Dados;

// Funcao executada pelas threads
void *filtrar(void *arg) {
    Dados *d = (Dados *)arg;

    int largura = d->largura;
    int altura = d->altura;

    // Define as linhas de cada thread
    int inicio = d->id * altura / d->p;
    int fim = (d->id + 1) * altura / d->p;

    for (int i = inicio; i < fim; i++) {
        for (int j = 0; j < largura; j++) {

            // Copia os pixels das bordas
            if (i == 0 || j == 0 ||
                i == altura - 1 || j == largura - 1) {

                d->saida[i * largura + j] =
                    d->entrada[i * largura + j];

            } else {

                int soma = 0;

                // Soma os 9 pixels vizinhos
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        soma += d->entrada[
                            (i + x) * largura + (j + y)
                        ];
                    }
                }

                d->saida[i * largura + j] = soma / 9;
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s imagem.png threads\n", argv[0]);
        return 1;
    }

    int p = atoi(argv[2]);

    if (p != 2 && p != 4 && p != 8) {
        printf("Numero de threads invalido.\n");
        return 1;
    }

    int largura, altura, canais;

    // Leitura fora da medicao
    unsigned char *entrada = stbi_load(
        argv[1], &largura, &altura, &canais, 1
    );

    if (entrada == NULL) {
        printf("Erro ao carregar imagem.\n");
        return 1;
    }

    // Aloca as imagens de saida
    unsigned char *saida = malloc(largura * altura);
    unsigned char *referencia = malloc(largura * altura);

    if (saida == NULL || referencia == NULL) {
        printf("Erro ao alocar memoria.\n");
        stbi_image_free(entrada);
        free(saida);
        free(referencia);
        return 1;
    }

    pthread_t threads[8];
    Dados dados[8];

    struct timespec inicio, fim;

    // Inicio da medicao paralela
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Cria as threads
    for (int i = 0; i < p; i++) {
        dados[i].id = i;
        dados[i].largura = largura;
        dados[i].altura = altura;
        dados[i].p = p;
        dados[i].entrada = entrada;
        dados[i].saida = saida;

        int erro = pthread_create(
            &threads[i], NULL, filtrar, &dados[i]
        );

        if (erro != 0) {
            printf("Erro ao criar thread %d.\n", i);

            for (int j = 0; j < i; j++) {
                pthread_join(threads[j], NULL);
            }

            stbi_image_free(entrada);
            free(saida);
            free(referencia);
            return 1;
        }
    }

    // Espera todas terminarem
    for (int i = 0; i < p; i++) {
        pthread_join(threads[i], NULL);
    }

    // Fim da medicao
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo_ms =
        (fim.tv_sec - inicio.tv_sec) * 1000.0 +
        (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    // Calcula a referencia sequencial fora da medicao
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {

            if (i == 0 || j == 0 ||
                i == altura - 1 || j == largura - 1) {

                referencia[i * largura + j] =
                    entrada[i * largura + j];

            } else {
                int soma = 0;

                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        soma += entrada[
                            (i + x) * largura + (j + y)
                        ];
                    }
                }

                referencia[i * largura + j] = soma / 9;
            }
        }
    }

    // Compara todos os pixels sem usar break
    int correto = 1;

    for (int i = 0; i < largura * altura; i++) {
        if (saida[i] != referencia[i]) {
            correto = 0;
        }
    }

    printf("Imagem: %d x %d\n", largura, altura);
    printf("Versao: paralela\n");
    printf("Threads: %d\n", p);
    printf("Tempo: %.3f ms\n", tempo_ms);
    printf("Corretude: %s\n", correto ? "OK" : "ERRO");

    stbi_image_free(entrada);
    free(saida);
    free(referencia);

    return correto ? 0 : 1;
}
