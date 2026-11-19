
#!/bin/bash

set -euo pipefail
export LC_ALL=C

# Entra na raiz do projeto
cd "$(dirname "$0")/.."

# Compila os programas
make

# Arquivo que armazenara os resultados
arquivo="resultados/tempos.csv"

# Cria o CSV com o cabecalho
echo "aplicacao,versao,tamanho,threads,execucao,tempo_ms" > "$arquivo"

# Executa um programa cinco vezes
rodar() {
    aplicacao=$1
    versao=$2
    tamanho=$3
    threads=$4
    programa=$5
    shift 5

    for i in 1 2 3 4 5
    do
        saida=$("$programa" "$@")

        # Exibe a saida do programa
        echo "$saida"

        # Verifica se ocorreu erro de corretude
        if echo "$saida" | grep -q "Corretude: ERRO"; then
            echo "Erro de corretude. Experimento interrompido."
            exit 1
        fi

        # Extrai o tempo em milissegundos
        tempo=$(echo "$saida" | awk '/^Tempo:/ {print $2}')

        if [ -z "$tempo" ]; then
            echo "Erro ao obter o tempo."
            exit 1
        fi

        # Salva uma linha no CSV
        echo "$aplicacao,$versao,$tamanho,$threads,$i,$tempo" >> "$arquivo"
    done
}

# Multiplicacao de matrizes
for n in 256 512 1024
do
    rodar matriz sequencial "$n" 1 ./matriz/sequencial "$n"

    for p in 2 4 8
    do
        rodar matriz paralelo "$n" "$p" ./matriz/paralelo "$n" "$p"
    done
done

# Processamento de imagens
for item in "256:imagem1_256.png" "512:imagem2_512.png" "1024:imagem3_1024.png"
do
    n=${item%%:*}
    nome=${item#*:}

    rodar imagem sequencial "$n" 1 ./imagem/sequencial "entradas/$nome"

    for p in 2 4 8
    do
        rodar imagem paralelo "$n" "$p" ./imagem/paralelo "entradas/$nome" "$p"
    done
done

# Contadores
rodar contador sequencial 100000000 1 ./contador/sequencial

for p in 2 4 8
do
    rodar contador mutex 100000000 "$p" ./contador/mutex "$p"
    rodar contador local 100000000 "$p" ./contador/local "$p"
done

echo "Experimentos finalizados."
echo "Resultados salvos em $arquivo"
chmod +x scripts/rodar_exp.sh