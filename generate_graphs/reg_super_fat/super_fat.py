import numpy 
from scipy.optimize import curve_fit

def modelo_stirling(x, a, b, c):
    # Modelo para funções tipo fatorial: x*ln(x) + x
    return a * x * numpy.log(x + 1e-9) + b * x + c

def normalize_log(values: list) -> list:
    # Normaliza por log natural
    return numpy.log(values)

def unnormalize_log(values: list) -> list:
    # Desnomaliza por log natural
    return numpy.exp(values)

def regression(x_values: list, y_values: list, news_points: int):
    ln_y_values = normalize_log(y_values)
    
    # Ajusta os parâmetros (a, b, c) aos seus pontos conhecidos
    parameters, _ = curve_fit(modelo_stirling, x_values, ln_y_values)

    # gera novos pontos x dentro do intervalo (min(x_values), max(x_values))
    x_news = numpy.linspace(min(x_values), max(x_values), news_points)

    # Calcula os novos valores usando o modelo
    ln_predicts = modelo_stirling(x_news, *parameters)
    predicts = unnormalize_log(ln_predicts)  # Voltando do logaritmo para a escala original

    return predicts, x_news, parameters