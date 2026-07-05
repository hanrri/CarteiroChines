import matplotlib.pyplot as plt

def gen_graphs(title: str, xlabel: str, ylabel: str, colors: dict, dados: dict, labels: dict):
    
    # Plotando cada algoritmo no grafico
    for k, v in dados.items():
        plt.scatter(v["x_values"], v["y_values"], color=colors[k], label=labels[k])
        plt.plot(v["x_values"], v["y_values"], color=colors[k], linestyle="--")

    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.tight_layout()
    plt.show()