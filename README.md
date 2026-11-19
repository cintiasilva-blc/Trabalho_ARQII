## Avaliação de Desempenho e Programação Paralela com Threads

Trabalho prático da disciplina Arquitetura e Organização de Computadores II (12029)\
Ciência da Computação - Universidade Estadual de Maringá (UEM).

**Professora:** Sandra Cossul  
**Autoras:** 
- Cintia da Silva Bulcão
- Giovana da Silva Bulcão

### Descrição

O trabalho tem como objetivo comparar o desempenho de aplicações sequenciais e paralelas utilizando a linguagem C e a biblioteca POSIX Threads (pthreads).

Foram implementadas três aplicações:

- Multiplicação de matrizes de dimensões 256×256, 512×512 e 1024×1024.
- Processamento de imagens em escala de cinza utilizando filtro de média 3×3.
- Contadores sequenciais e paralelos, com mutex e contadores locais.

As versões paralelas foram testadas utilizando 2, 4 e 8 threads.

### Ambiente de execução

- Processador: Intel Core i5-13420H
- Memória RAM: 16 GB
- Sistema operacional: Fedora Linux 44
- Compilador: GCC 
- Compilação: `-O2 -pthread`, utilizando `-lm` nas aplicações de imagens

### Compilação

Na raiz do projeto, execute:

```bash
make
```

### Arquivos de entrada

As matrizes são geradas utilizando o código disponibilizado pela professora.

Para gerá-las:

```bash
mkdir -p entradas
cd entradas
../matriz/gerador
cd ..
```

As imagens originais fornecidas pela professora devem ser colocadas na pasta `entradas/`, com os nomes:

- `imagem1_256.png`
- `imagem2_512.png`
- `imagem3_1024.png`

### Execução dos programas

Exemplos:

```bash
./matriz/sequencial 256
./matriz/paralelo 256 4

./imagem/sequencial entradas/imagem1_256.png
./imagem/paralelo entradas/imagem1_256.png 4

./contador/sequencial
./contador/mutex 4
./contador/local 4
```

### Experimentos

Para executar todas as configurações, cinco vezes cada:

```bash
bash scripts/rodar_exp.sh
```

Os tempos serão registrados em `resultados/tempos.csv`.

Para gerar as estatísticas e os gráficos:

```bash
python3 scripts/analisar.py
```

Os resultados serão salvos nas pastas `resultados/tabelas` e `resultados/graficos`.

### Resultados

O desempenho foi avaliado por meio do tempo de execução, speedup e eficiência paralela. Foram realizadas cinco execuções de cada configuração, utilizando a mediana como medida principal.

As tabelas e os gráficos estão disponíveis na pasta `resultados/`.
