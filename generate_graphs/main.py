from get_data import getData
from generate_graphs import gen_graphs

if __name__ == "__main__":
    
    theme = getData(file="theme.json", dir="theme")
    
    data_base = getData(file="dados.json", dir="data_base")
    
    # Grafico
    gen_graphs(
        title=theme["labels"]["title"],
        xlabel=theme["labels"]["x_label"],
        ylabel=theme["labels"]["y_label"],
        colors=theme["colors"],
        labels=theme["labels_algorithm"],
        dados=data_base
    )