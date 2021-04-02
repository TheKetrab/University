
set yzeroaxis

set boxwidth 0.03 absolute
set style fill solid 1.0 noborder

bin_width = 0.025;
bin_number(x) = floor(x/bin_width)
rounded(x) = bin_width * ( bin_number(x) + 0.1 )

plot 'normal.csv' using (rounded($1)):(1) smooth frequency with boxes
