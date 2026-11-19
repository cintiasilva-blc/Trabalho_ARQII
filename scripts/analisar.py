import os
import pandas as pd
import matplotlib.pyplot as plt

# Pastas do projeto
os.makedirs("resultados/graficos", exist_ok=True)
os.makedirs("resultados/tabelas", exist_ok=True)

# Le os tempos registrados
dados = pd.read_csv("resultados/tempos.csv")

# Calcula as estatisticas de cada configuracao
tabela = dados.groupby(
    ["aplicacao", "versao", "tamanho", "threads"]
)["tempo_ms"].agg(
    execucoes="count",
    media="mean",
    mediana="median",
    minimo="min",
    maximo="max",
    desvio="std"
).reset_index()

# Verifica se existem cinco execucoes
if (tabela["execucoes"] != 5).any() or len(tabela) != 31:
    print("Aviso: confira as configuracoes e execucoes do CSV.")

# Calcula speedup e eficiencia
tabela["Tseq"] = 0.0
tabela["Tpar"] = tabela["mediana"]
tabela["speedup"] = 0.0
tabela["eficiencia"] = 0.0

for i in tabela.index:
    aplicacao = tabela.loc[i, "aplicacao"]
    tamanho = tabela.loc[i, "tamanho"]
    threads = tabela.loc[i, "threads"]

    # Busca o tempo sequencial da mesma aplicacao e tamanho
    referencia = tabela[
        (tabela["aplicacao"] == aplicacao) &
        (tabela["tamanho"] == tamanho) &
        (tabela["versao"] == "sequencial")
    ]

    if len(referencia) == 1:
        tseq = referencia.iloc[0]["mediana"]
        tpar = tabela.loc[i, "mediana"]

        tabela.loc[i, "Tseq"] = tseq
        tabela.loc[i, "speedup"] = tseq / tpar
        tabela.loc[i, "eficiencia"] = (
            (tseq / tpar) / threads * 100
        )

# Salva a tabela completa
tabela.to_csv(
    "resultados/tabelas/estatisticas.csv",
    index=False,
    float_format="%.4f"
)

print("\nRESULTADOS DOS EXPERIMENTOS\n")
print(tabela.round(3).to_string(index=False))

# Graficos das matrizes e imagens
for aplicacao in ["matriz", "imagem"]:
    for tamanho in [256, 512, 1024]:

        parte = tabela[
            (tabela["aplicacao"] == aplicacao) &
            (tabela["tamanho"] == tamanho)
        ].sort_values("threads")

        # Grafico de tempo
        plt.figure()
        plt.plot(parte["threads"], parte["mediana"], "o-")
        plt.xlabel("Numero de threads")
        plt.ylabel("Tempo (ms)")
        plt.title(f"{aplicacao} {tamanho} - Tempo")
        plt.xticks([1, 2, 4, 8])
        plt.grid(True)
        plt.savefig(
            f"resultados/graficos/{aplicacao}_{tamanho}_tempo.png"
        )
        plt.close()

        # Grafico de speedup
        plt.figure()
        plt.plot(
            parte["threads"], parte["speedup"],
            "o-", label="Obtido"
        )
        plt.plot(
            [1, 2, 4, 8], [1, 2, 4, 8],
            "--", label="Ideal"
        )
        plt.xlabel("Numero de threads")
        plt.ylabel("Speedup")
        plt.title(f"{aplicacao} {tamanho} - Speedup")
        plt.xticks([1, 2, 4, 8])
        plt.legend()
        plt.grid(True)
        plt.savefig(
            f"resultados/graficos/{aplicacao}_{tamanho}_speedup.png"
        )
        plt.close()

        # Grafico de eficiencia
        plt.figure()
        plt.plot(parte["threads"], parte["eficiencia"], "o-")
        plt.xlabel("Numero de threads")
        plt.ylabel("Eficiencia (%)")
        plt.title(f"{aplicacao} {tamanho} - Eficiencia")
        plt.xticks([1, 2, 4, 8])
        plt.grid(True)
        plt.savefig(
            f"resultados/graficos/{aplicacao}_{tamanho}_eficiencia.png"
        )
        plt.close()

# Grafico comparando as versoes do contador
contador = tabela[tabela["aplicacao"] == "contador"]

plt.figure()

for versao in ["sequencial", "mutex", "local"]:
    parte = contador[
        contador["versao"] == versao
    ].sort_values("threads")

    plt.plot(
        parte["threads"],
        parte["mediana"],
        "o-",
        label=versao
    )

plt.yscale("log")

plt.xlabel("Numero de threads")
plt.ylabel("Tempo (ms)")
plt.title("Comparacao dos contadores")
plt.xticks([1, 2, 4, 8])
plt.legend()
plt.grid(True)
plt.savefig("resultados/graficos/contador_comparacao.png")
plt.close()

print("\nTabelas e graficos gerados com sucesso!")
