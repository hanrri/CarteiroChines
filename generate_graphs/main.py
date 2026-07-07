from get_data import getData
from generate_graphs import gen_graphs
from reg_super_fat.super_fat import regression
from pathlib import Path
import matplotlib.pyplot as plt
import numpy
import os

if __name__ == "__main__":
    
    NN = "nn.json" # dados de execucao da euristica do vizinho mais proximo
    GREEDY = "greedy.json" # dados de execucao da euristica gulosa 
    ENUMERATION = "enumeration.json" # Dados de execucao da euritisca de enumeracao (precisa da regresao)
    THEME = "theme.json"
    paths = [NN, GREEDY, ENUMERATION]
    
    # Pasta onde os gráficos serão salvos
    OUTPUT_DIR = Path(__file__).parent.parent / "graphs"
    
    # Carregando tema e configurações dos gráficos
    theme = getData(file=THEME, dir="datas")
    
    raw_data = []
    for path in paths:
        raw_data.append(getData(file=path, dir="datas"))
    
    # Organizando os dados originais
    data_base = {
        NN: raw_data[0],
        GREEDY: raw_data[1],
        ENUMERATION: raw_data[2]
    }
    
    
    # Calculando as medias 
    plot_data = {}
    
    for key in paths:
        x_vals = []
        y_time_means = []
        y_cost_means = []
        
        for test in data_base[key]:
            x_vals.append(test["n_vertices"])
            
            # Médias de Tempo
            time_mean = numpy.mean(test["tempos_ms"])
            y_time_means.append(time_mean)
            
            # Médias de Custo de Ciclo
            cost_mean = numpy.mean(test["custos_ciclo"])
            y_cost_means.append(cost_mean)
            
        plot_data[key] = {
            "x": numpy.array(x_vals),
            "y_time": numpy.array(y_time_means),
            "y_cost": numpy.array(y_cost_means)
        }


    # Regresão de Stiling para enumeracao
    new_x_enum, new_y_enum, _ = regression(
        plot_data[ENUMERATION]["x"],
        plot_data[ENUMERATION]["y_time"],
        news_points=1000,
        min_x=25,   
        max_x=1000  
    )

    # Configurações do arquivo de tema
    colors = theme.get("colors", {})
    labels_alg = theme.get("labels_algorithm", {})
    labels_display = theme.get("labels", {})

    color_nn = colors.get(NN, "black")
    color_greedy = colors.get(GREEDY, "green")
    color_enum = colors.get(ENUMERATION, "red")

    SAVE_DPI = 300 # Qualidade dos graficos

    # GRÁFICO 1: CUSTO RECORTE (25 até 75 Vértices)
    plt.figure(figsize=(10, 6))
    plt.plot(
        plot_data[NN]["x"], 
        plot_data[NN]["y_cost"], 
        color=color_nn, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(NN, "Vizinho Próximo")
    )
             
    plt.plot(
        plot_data[GREEDY]["x"], 
        plot_data[GREEDY]["y_cost"], 
        color=color_greedy, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(GREEDY, "Gulosa")
    )
             
    plt.scatter(
        plot_data[ENUMERATION]["x"], 
        plot_data[ENUMERATION]["y_cost"], 
        color=color_enum, 
        s=60, 
        zorder=5, 
        label=f"{labels_alg.get(ENUMERATION)} (Pontos Reais)"
    )
    plt.plot(
        plot_data[ENUMERATION]["x"], 
        plot_data[ENUMERATION]["y_cost"], 
        color=color_enum, 
        linestyle="-", 
        linewidth=1.5, 
        label=f"{labels_alg.get(ENUMERATION)} (Conexão Real)"
    )

    plt.xlim(25, 75)
    y_max_cost = max(plot_data[ENUMERATION]["y_cost"])
    y_min_cost = min(min(plot_data[NN]["y_cost"][:3]), min(plot_data[GREEDY]["y_cost"][:3]))
    plt.ylim(y_min_cost * 0.9, y_max_cost * 1.1)

    plt.xlabel(labels_display.get("x_label", "Quantidade de Vértices"))
    plt.ylabel("Custo do Ciclo (Média)")
    plt.title(f"{labels_display.get('title', 'Comparação')} - Custo: Recorte (25 a 75 Vértices)")
    plt.legend(loc="upper left")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/01_custo_recorte_25_75.png", dpi=SAVE_DPI)
    plt.close()


    # GRÁFICO 2: TEMPO VISÃO GLOBAL (Até 1000 Vértices - Escala Linear)
    plt.figure(figsize=(10, 6))
    plt.plot(
        plot_data[NN]["x"], 
        plot_data[NN]["y_time"], 
        color=color_nn, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(NN, "Vizinho Próximo")
    )

    plt.plot(
        plot_data[GREEDY]["x"], 
        plot_data[GREEDY]["y_time"], 
        color=color_greedy, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(GREEDY, "Gulosa")
    )

    plt.scatter(
        plot_data[ENUMERATION]["x"], 
        plot_data[ENUMERATION]["y_time"], 
        color=color_enum, 
        s=60, 
        zorder=5, 
        label=f"{labels_alg.get(ENUMERATION)} (Pontos Reais)"
    )
    plt.plot(
        new_x_enum, 
        new_y_enum, 
        color=color_enum, 
        linestyle="--", 
        linewidth=2, 
        label=f"{labels_alg.get(ENUMERATION)} (Regressão Stirling)"
    )

    plt.xlabel(labels_display.get("x_label", "Quantidade de Vértices"))
    plt.ylabel(labels_display.get("y_label", "Tempo de Execução (ms)"))
    plt.title(f"{labels_display.get('title', 'Comparação')} - Tempo de Execução")
    plt.legend(loc="upper left")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/02_tempo_global_linear.png", dpi=SAVE_DPI)
    plt.close()
    
    
    # GRÁFICO 3: TEMPO VISÃO GLOBAL (1000 Vértices)
    plt.figure(figsize=(10, 6))
    plt.plot(
        plot_data[NN]["x"], 
        plot_data[NN]["y_time"], 
        color=color_nn, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(NN, "Vizinho Próximo")
    )

    plt.plot(
        plot_data[GREEDY]["x"], 
        plot_data[GREEDY]["y_time"], 
        color=color_greedy, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(GREEDY, "Gulosa")
    )

    plt.xlabel(labels_display.get("x_label", "Quantidade de Vértices"))
    plt.ylabel(labels_display.get("y_label", "Tempo de Execução (ms)"))
    plt.title(f"{labels_display.get('title', 'Comparação')} - Tempo de Execução")
    plt.legend(loc="upper left")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/03_tempo_global_heuristicas.png", dpi=SAVE_DPI)
    plt.close()
    
    
    # GRÁFICO 4: TEMPO VISÃO GLOBAL (Até 1000 Vértices - Escala LOG)
    plt.figure(figsize=(10, 6))    
    plt.plot(
        plot_data[NN]["x"], 
        plot_data[NN]["y_time"], 
        color=color_nn, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(NN, "Vizinho Próximo")
    )

    plt.plot(
        plot_data[GREEDY]["x"], 
        plot_data[GREEDY]["y_time"], 
        color=color_greedy, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(GREEDY, "Gulosa")
    )

    plt.scatter(
        plot_data[ENUMERATION]["x"], 
        plot_data[ENUMERATION]["y_time"], 
        color=color_enum, 
        s=60, 
        zorder=5, 
        label=f"{labels_alg.get(ENUMERATION)} (Pontos Reais)"
    )
    plt.plot(
        new_x_enum, 
        new_y_enum, 
        color=color_enum, 
        linestyle="--", 
        linewidth=2, 
        label=f"{labels_alg.get(ENUMERATION)} (Regressão Stirling)"
    )

    plt.xlabel(labels_display.get("x_label", "Quantidade de Vértices"))
    plt.ylabel("Tempo de Execução (Escala Logarítmica - ms)")
    plt.title(f"{labels_display.get('title', 'Comparação')} - Tempo de execução")
    plt.yscale('log') 
    plt.legend(loc="upper left")
    plt.grid(True, which="both", linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/04_tempo_global_log.png", dpi=SAVE_DPI)
    plt.close()


    # GRÁFICO 5: TEMPO RECORTE (25 até 75 Vértices)
    plt.figure(figsize=(10, 6))
    plt.plot(
        plot_data[NN]["x"], 
        plot_data[NN]["y_time"], 
        color=color_nn, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(NN, "Vizinho Próximo")
    )

    plt.plot(
        plot_data[GREEDY]["x"], 
        plot_data[GREEDY]["y_time"], 
        color=color_greedy, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(GREEDY, "Gulosa")
    )

    plt.scatter(
        plot_data[ENUMERATION]["x"], 
        plot_data[ENUMERATION]["y_time"], 
        color=color_enum, s=60, 
        zorder=5, 
        label=f"{labels_alg.get(ENUMERATION)} (Pontos Reais)"
    )
    plt.plot(
        new_x_enum, 
        new_y_enum, 
        color=color_enum, 
        linestyle="--", 
        linewidth=2, 
        label=f"{labels_alg.get(ENUMERATION)} (Regressão Stirling)"
    )

    plt.xlim(25, 75)
    y_max_real = max(plot_data[ENUMERATION]["y_time"])
    y_min_real = min(min(plot_data[NN]["y_time"]), min(plot_data[GREEDY]["y_time"]))
    plt.ylim(y_min_real * 0.5, y_max_real * 3)

    plt.xlabel(labels_display.get("x_label", "Quantidade de Vértices"))
    plt.ylabel(labels_display.get("y_label", "Tempo de Execução (ms)"))
    plt.title(f"{labels_display.get('title', 'Comparação')} - Tempo: Recorte (25 a 75 Vértices)")
    plt.legend(loc="upper left")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/05_tempo_recorte_25_75_linear.png", dpi=SAVE_DPI)
    plt.close()
    
    
    # GRÁFICO 6: TEMPO RECORTE (25 até 75 Vértices - Escala LOG)
    plt.figure(figsize=(10, 6)) 
    plt.plot(
        plot_data[NN]["x"], 
        plot_data[NN]["y_time"], 
        color=color_nn, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(NN, "Vizinho Próximo")
    )

    plt.plot(
        plot_data[GREEDY]["x"], 
        plot_data[GREEDY]["y_time"], 
        color=color_greedy, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(GREEDY, "Gulosa")
    )

    plt.scatter(
        plot_data[ENUMERATION]["x"], 
        plot_data[ENUMERATION]["y_time"], 
        color=color_enum, 
        s=60, 
        zorder=5, 
        label=f"{labels_alg.get(ENUMERATION)} (Pontos Reais)"
    )
    plt.plot(
        new_x_enum, 
        new_y_enum, 
        color=color_enum, 
        linestyle="--", 
        linewidth=2, 
        label=f"{labels_alg.get(ENUMERATION)} (Regressão Stirling)"
    )

    plt.xlim(25, 75)
    y_max_real = max(plot_data[ENUMERATION]["y_time"])
    y_min_real = min(min(plot_data[NN]["y_time"]), min(plot_data[GREEDY]["y_time"]))
    plt.ylim(y_min_real * 0.5, y_max_real * 3)

    plt.xlabel(labels_display.get("x_label", "Quantidade de Vértices"))
    plt.ylabel("Tempo de Execução (Escala Logarítmica - ms)")
    plt.title(f"{labels_display.get('title', 'Comparação')} - Tempo: Recorte (25 a 75 Vértices)")
    plt.yscale('log') 
    plt.legend(loc="upper left")
    plt.grid(True, which="both", linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/06_tempo_recorte_25_75_log.png", dpi=SAVE_DPI)
    plt.close()


    # GRÁFICO 7: COMPARAÇÃO DE CUSTO (25 até 1000 Vértices)
    plt.figure(figsize=(10, 6))
    plt.plot(
        plot_data[NN]["x"], 
        plot_data[NN]["y_cost"], 
        color=color_nn, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(NN, "Vizinho Próximo")
    )

    plt.plot(
        plot_data[GREEDY]["x"], 
        plot_data[GREEDY]["y_cost"], 
        color=color_greedy, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(GREEDY, "Gulosa")
    )

    plt.xlabel(labels_display.get("x_label", "Quantidade de Vértices"))
    plt.ylabel("Custo do Ciclo (Média)")
    plt.title(f"{labels_display.get('title', 'Comparação')} - Comparação de Custo")
    plt.legend(loc="upper left")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/07_custo_global_heuristicas.png", dpi=SAVE_DPI)
    plt.close()

    print(f"\n Sucesso! Todos os 7 gráficos foram organizados e salvos com 300 DPI na pasta: '{OUTPUT_DIR}/'")
    
    
    # GRÁFICO 8: COMPARAÇÃO DE CUSTO RECORTE (25 até 75 Vértices)
    plt.figure(figsize=(10, 6))
    plt.plot(
        plot_data[NN]["x"], 
        plot_data[NN]["y_cost"], 
        color=color_nn, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(NN, "Vizinho Próximo")
    )
             
    plt.plot(
        plot_data[GREEDY]["x"], 
        plot_data[GREEDY]["y_cost"], 
        color=color_greedy, 
        linestyle="-", 
        linewidth=2, 
        label=labels_alg.get(GREEDY, "Gulosa")
    )
             
    plt.plot(
        plot_data[ENUMERATION]["x"], 
        plot_data[ENUMERATION]["y_cost"], 
        color=color_enum, 
        linestyle="-", 
        linewidth=2, 
        label=f"{labels_alg.get(ENUMERATION)} (Enumeração)"
    )

    plt.xlim(25, 75)    
    y_max_cost = max(plot_data[ENUMERATION]["y_cost"])
    y_min_cost = min(min(plot_data[NN]["y_cost"][:3]), min(plot_data[GREEDY]["y_cost"][:3]))
    plt.ylim(y_min_cost * 0.9, y_max_cost * 1.1)

    plt.xlabel(labels_display.get("x_label", "Quantidade de Vértices"))
    plt.ylabel("Custo do Ciclo (Média)")
    plt.title(f"{labels_display.get('title', 'Comparação')} - Custo: Recorte (25 a 75 Vértices)")
    plt.legend(loc="upper left")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/08_custo_recorte_tres_abordagens.png", dpi=SAVE_DPI)
    plt.close()

    print(f"\n Sucesso! Todos os gráficos salvos na pasta: '{OUTPUT_DIR}/'")