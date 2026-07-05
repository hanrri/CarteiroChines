## Aproximação de Stirling para Fatoriais

A **Aproximação de Stirling** descreve o comportamento assintótico do fatorial clássico. Adaptando essa aproximação para o comportamento do duplo fatorial ($x!!$), sabemos que a função se comporta dominantemente como:

$$x!! \approx \left(\frac{x}{e}\right)^{\frac{x}{2}}$$

Para transformar isso em um modelo passível de regressão linearizável, aplicamos o **logaritmo natural ($\ln$)** em ambos os lados da equação:

$$\ln(x!!) \approx \ln\left( \left(\frac{x}{e}\right)^{\frac{x}{2}} \right)$$

Utilizando as propriedades dos logaritmos ($\ln(a^b) = b \cdot \ln(a)$ e $\ln(a/b) = \ln(a) - \ln(b)$):

$$\ln(x!!) \approx \frac{x}{2} \cdot \ln\left(\frac{x}{e}\right)$$

$$\ln(x!!) \approx \frac{x}{2} \cdot (\ln(x) - \ln(e))$$


Como $\ln(e) = 1$:


$$\ln(x!!) \approx \frac{1}{2}x\ln(x) - \frac{1}{2}x$$

### O Modelo Generalizado (`modelo_stirling`)

Para permitir que o algoritmo ajuste dinamicamente a curva a partir de qualquer amostragem de dados (com ou sem ruído), introduzimos os parâmetros livres $a$, $b$ e $c$. Isso transforma a aproximação teórica no modelo matemático:

$$\ln(y) = a \cdot x \ln(x) + b \cdot x + c$$

> **Nota de Estabilização Numérica:** No código, usa-se `x + 1e-9` dentro do logaritmo para evitar uma indefinição matemática ($\ln(0) = -\infty$) caso $x = 0$ seja fornecido.

---