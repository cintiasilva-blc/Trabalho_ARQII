#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s imagem.png\n", argv[0]);
        return 1;
    }

    int largura, altura, canais;

    // Carrega a imagem em escala de cinza
    unsigned char *entrada = stbi_load(
        argv[1], &largura, &altura, &canais, 1
    );

    if (entrada == NULL) {
        printf("Erro ao carregar imagem.\n");
        return 1;
    }

    // Aloca memoria para a imagem resultante
    unsigned char *saida = malloc(largura * altura);

    if (saida == NULL) {
        printf("Erro ao alocar memoria.\n");
        stbi_image_free(entrada);
        return 1;
    }

    struct timespec inicio, fim;

    // Inicio da medicao
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Percorre todos os pixels
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {

            // Mantem os pixels das bordas
            if (i == 0 || j == 0 ||
                i == altura - 1 || j == largura - 1) {

                saida[i * largura + j] =
                    entrada[i * largura + j];

            } else {

                int soma = 0;

                // Soma os 9 pixels da vizinhanca
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        soma += entrada[
                            (i + x) * largura + (j + y)
                        ];
                    }
                }

                // Calcula a media
                saida[i * largura + j] = soma / 9;
            }
        }
    }

    // Fim da medicao
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo_ms =
        (fim.tv_sec - inicio.tv_sec) * 1000.0 +
        (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    // Soma de verificacao, fora da medicao
    unsigned long long total = 0;

    for (int i = 0; i < largura * altura; i++) {
        total += saida[i];
    }

    printf("Imagem: %d x %d\n", largura, altura);
    printf("Versao: sequencial\n");
    printf("Tempo: %.3f ms\n", tempo_ms);
    printf("Soma dos pixels: %llu\n", total);

    stbi_image_free(entrada);
    free(saida);

    return 0;
}
