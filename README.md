## Avaliação de Desempenho e Programação Paralela com Threads

Trabalho prático da disciplina **Arquitetura e Organização de Computadores II** (12029)\
Ciência da Computação - UEM (Universidade Estadual de Maringá)\
**Professora:** Sandra Cossul.

### Sobre o projeto
O objetivo é avaliar experimentalmente o impacto do uso de múltiplas threads no desempenho de aplicações, comparando versões sequenciais e paralelas (C com POSIX Threads). São analisados tempo de execução, speedup, eficiência paralela e escalabilidade, relacionando os resultados a conceitos como processadores multicore, memória cache e coerência de cache (protocolo MESI).

### Aplicações

- **Multiplicação de matrizes** (256, 512 e 1024)
- **Filtro de média 3x3 em imagens** em escala de cinza
- **Contadores paralelos** até 100 milhões (mutex compartilhado vs. contadores locais)

### Como executar

```bash
gcc -O2 -pthread programa.c -o programa
./programa <tamanho> <threads>
```

### Ambiente de testes
- Processador: Intel Core i5-13420H (8 núcleos físicos / 12 threads lógicas)
- RAM: 16 GB
- SO / Compilador: Fedora Linux 44 / GCC 16.2.1 (-O2 -pthread)

### Autoras
Cintia da Silva Bulcão  [GitHub](https://github.com/cintiasilva-blc)\
Giovana da Silva Bulcão [GitHub](https://github.com/giovanabulcao)
