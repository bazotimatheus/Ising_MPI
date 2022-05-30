#!/bin/bash
# Este script cria o gráfico da magnetizacao pela temperatura a partir do arquivo "data.out"
# Deve ser executado no terminal com o gnuplot
# > gnuplot ./magnetizacao.sh

set terminal png
plot "data.out" u 1:2 w p title "Magnetizacao"
set xlabel "Tempertura"
set ylabel "Magnetizacao"
set output "magnetizacao.png"
replot
q