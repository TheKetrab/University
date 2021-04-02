# MyBomberman

TODO video

## Download:
1. Pobierz zip: TODO link
2. Wypakuj w dowolnym miejscu.
3. Odpal plik exe, nie wyrzucając innych plików z folderu!

## Sterowanie
< , ^ , > , v - sterowanie graczem  
Q , E , R , T - sterowanie kamerą  
Spacja - stawienie bomby  

## Opis:
Gra została stworzona podczas kursu Unity na Uniwersytecie Wrocławskim.  
Jest to własna implementacja gry zręcznościowej Bomberman.  
Nie ma trybu na wielu graczy, jest tylko zaimplementowane proste AI.  
@ Bartłomiej Grochowski 2018  

## Logika:

#### Mapa
Mapa jest wczytywana z pliku jpg (pikselami).
1. GREEN - możliwe do zniszczenia bombą
2. BLACK - niemożliwe do zniszczenia bombą
3. WHITE - puste

#### Bomby
Stawiamy bomby, żeby wysadzać obiekty typu GREEn oraz przeciwników.  
Za wysadzenie GREEN, pojawia się bonus:  
1. serce - dodatkowe życie  
2. sprężynka - zwiększenie szybkości  
3. kula - możliwość stawiania kilku bomb na raz  
4. krzyżyk - zwiększenie zasięgu wybuchu  
  
Bomba jest przenikalna, dopóki stawiający z niej nie wyjdzie.  
Wybucha po 3 sekundach produkując dym (SMOKE) w 4 strony.  
Kolizja z dymu z inną bombą powoduje, że bomba znika.  

## AI
Każdy z przeciwników ma cały czas wyznaczony własny cel (FindTask)  
Szuka najlepszej opcji:  
1. jeśli bomba (lub dym) - ucieka
2. jeśli coś do zebrania - to bierze
3. jeśli inny gracz - to wysadza go
4. jeśli coś do wysadzenia (GREEN) - to wysadza
5. else -> OptionNotFound (nic interesującego)

Na tej zasadzie wyznacza sobie nowy punkt 'goal'.  
I potem idzie do tego punktu.
