set grid
set yzeroaxis
set xzeroaxis
set title "c = Dzeta( 0.5 + t*i )"
set xlabel "t"
plot "data.csv" using 1:2 with lines title "Real(c)", \
     "data.csv" using 1:3 with lines title "Imag(c)"