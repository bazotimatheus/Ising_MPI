#!/bin/bash
# Este script cria o gráfico da energia pela temperatura a partir do arquivo "data.out"
# Deve ser executado no terminal com o gnuplot
# > gnuplot ./energia.sh

set terminal png
plot "data.out" u 1:3 w p title "Energia"
set xlabel "Tempertura"
set ylabel "Energia"
set output "energia.png"
replot
q