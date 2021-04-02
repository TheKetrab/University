set xrange [0:1000]
set yrange [0:100]

s_avg = `awk < binomial.csv '{total = total + $1}END{print total/1000}'`
avg = s_avg*1

plot "binomial.csv" with points pointtype 7 pointsize 0.5 title "points", \
     50 with lines linetype 3 linewidth 1 linecolor rgb "black" title "binomial 100, 0.5", \
     avg with lines linetype 3 title sprintf("avg: %f",avg)
