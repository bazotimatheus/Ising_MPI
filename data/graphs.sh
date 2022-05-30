#!/bin/bash
# Este script monta todos os gráficos a partir do arquivo "data.out".
# Para executa-lo basta checar se ele possui permissão para execucao.
# Caso nao tenha execute o comando:
# > chmod +x graphs.sh

gnuplot ./magnetizacao.sh
gnuplot ./energia.sh
gnuplot ./calor_especifico.sh

mv magnetizacao.png ../graphs/.
mv energia.png ../graphs/.
mv calor_especifico.png ../graphs/.

clear