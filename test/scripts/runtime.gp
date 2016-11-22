reset
set ylabel 'time(sec)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime1.png'

plot [:][:10]'output.txt' using 2:xtic(1) with histogram title 'original', \
'' using 3:xtic(1) with histogram title 'optimized'  , \
'' using 4:xtic(1) with histogram title 'B+ tree' , \
'' using 5:xtic(1) with histogram title 'B+ tree bulk' , \
'' using ($0-0.1):(1.9):2 with labels title ' ' textcolor lt 1, \
'' using ($0-0.1):(1.6):3 with labels title ' ' textcolor lt 2, \
'' using ($0-0.1):(1.3):4 with labels title ' ' textcolor lt 3, \
'' using ($0-0.1):(1):5 with labels title ' ' textcolor lt 4, \
