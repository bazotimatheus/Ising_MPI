#!/bin/bash
# Este script cria o gráfico do calor especifico pela temperatura a partir do arquivo "data.out"
# Deve ser executado no terminal com o gnuplot
# > gnuplot ./calor_especifico.sh

set terminal png
plot "data.out" u 1:4 w p title "Calor Especifico"
set xlabel "Tempertura"
set ylabel "Calor Especifico"
set output "calor_especifico.png"
replot
q