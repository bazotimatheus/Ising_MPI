Este projeto possui programa do projeto final de HPC 2 do primeiro semestre letivo de 2022 do curso de Física Computacional do ICEx da UFF de Volta Redonda.

O programa tem como objetivo reproduzir o modelo de Ising em duas dimensões pelo método de Monte Carlo.
O livro base para a elaboração do programa é o Computational Physics, second edition, 2006, Nicholas J. Giordano, que pode ser encontrado em:

https://www.google.com.br/books/edition/_/52J6QgAACAAJ?hl=pt-BR&sa=X&ved=2ahUKEwjDrsPNiJv3AhXxMDUKHZZGBCwQ7_IDegQIEBAC

Antes de compilar o programa, recomenda-se a visualização do arquivo "main.c", dentro da pasta src/.
Na linha 8 se encontra

#define MPI

Caso queira rodar o programa em serial, comente esta linha.
Caso contrário, deixe-a como se encontra.

Para a compilação do programa é só executar o comando make no terminal que será gerado um executável.
Recomenda-se antes de compilar o programa olhar o arquivo "ising.h", dentro da pasta lib/, que possui todos os parâmetros do programa.
É possível realizar ajustes no tamanho da malha, no número de passos de Monte Carlo e no número de passos sobre a temperatura que alteram a qualidade dos dados de saída.
Quanto maiores forem os valores escolhidos mais suaves serão os gráficos, porém maior será o tempo de execução.

Ao final da execução será gerado um arquivo "data.out" dentro da pasta data/.
Para gerar os gráficos relacionados aos dados de saída execute o script "graphs.sh" que se encontra dentro da pasta data/. 
Este script irá gerar os gráficos para a magnetização, energia e calor específico no gnuplot.
Os gráficos ficam salvos na pasta graphs/ no formato .png.

Ao final da execução é de bom tom executar o comando make clean no terminal.
Este irá excluir os objetos gerados, o arquivo de saída "data.out" e o executável "ising".
Caso tenha sido executado o script que cria os gráficos, os mesmos não serão excluídos.
