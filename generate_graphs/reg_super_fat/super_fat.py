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

def regression(x_values: list, y_values: list, news_points: int, max_x = None, min_x = None):
    #  evitar problemas de tipos
    x_values = numpy.array(x_values)
    y_values = numpy.array(y_values)
    
    if max_x is None:
        max_x = max(x_values)
    
    if min_x is None:
        min_x = min(x_values)
    
    ln_y_values = normalize_log(y_values)
    
    parameters, _ = curve_fit(
        modelo_stirling, 
        x_values, 
        ln_y_values, 
        bounds=([0, -numpy.inf, -numpy.inf], [numpy.inf, numpy.inf, numpy.inf])
    )
    
    x_news = numpy.linspace(min_x, max_x, news_points)

    # gera novos valores usando o modelo
    ln_predicts = modelo_stirling(x_news, *parameters)
    predicts = unnormalize_log(ln_predicts)

    return x_news, predicts, parameters