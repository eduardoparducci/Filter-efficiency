# Preparação
  Abaixo descrevemos a ideia de implementação do projeto.
---

## 1 Tópicos
  - [x] Onde será feito o paralelismo
  - [x] Quais serão os parâmetros a ser variados (imagem, no. de threads/processos, ...)
  - [x] Ferramentas utilizadas
  - [x] Como será feita a comparação (quais gráficos, etc..)
  - [x] Divisão do trabalho
  - [x] Tratamento de borda

### 1.1 Threads
  A divisão de trabalho será feita de acordo com o número de *threads*/processos, sendo
  que cada um deles irá processar `1 / (nº de threads ou processos)` da imagem.

### 1.2 Parâmetros de variação
  Os parâmetros de análise serão os seguintes:
  - Tamanho de imagem: 5 imagens entre baixa resolução e 4k.
  - Número de threads: 5 variações entre 2 e 8.
  - Número de processos: 5 variações entre 2 e 8.
  Sendo assim, ao final será gerado um gráfico de cores indicando o tempo gasto em função
  do tamanho da imagem e do número de *threads*/processos.

### 1.3 Ferramentas 
  - FreeImage como interface de entrada e saída (receber e gerar imagem).
  - Linguagem C para implementação das funções.
  - Biblioteca cronômetro (fornecida pelo prof.) para análise de tempo.
  - Octave (ou outro software que preferirem) para gerar os gráficos.

### 1.4 Observações sobre a implementação
  Planejamos implementar o filtro [Sobel](https://pt.wikipedia.org/wiki/Filtro_Sobel) que
  consiste na convolução com duas matrizes de ordem 3x3.
  Para o tratamento de borda temos 3 opções:
  - **Estender**: copiar o valor das linhas e colunas das extremidades.
  - **Dar a volta**: pegar o valor da coluna/linha da extremidade oposta.
  - **Cortar**: desconsiderar os pixels da extremidade na aplicação do filtro.

---
## Otimizações (opcional)
> Reduzir o número de falhas de cache 

> Utilizar o for de forma que instruções MMX o reconheçam
