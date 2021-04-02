# Racket
@ metody programowania 2018,
racket - funkcyjny język, wersja Scheme (Lisp)

### # 01-newton-cube-root
przybliżanie pierwiastka trzeciego stopnia za pomocą [(x/y^2) + 2y] / 3

### # 02a-continued-fraction
obliczanie wartości ułamków łańcuchowych (fi,cbrt,PI,e,arctg)

### # 02b-nth-root
obliczanie pierwiastka n-tego stopnia, używając tłumienia przez uśrednianie

### # 03-rsa
implementacja:
1. RSA-unconvert-list
2. solve-ax+by=1 (euklides)
3. encrypt-and-sign
4. authenticate-and-decrypt

### # 04-heapsort
kopiec binarny i heapsort

### # 05-logic-formula-calculator
rezolucja dla rachunku zdań (spełnialność formuł logicznych)

### # 06-intellisense
'hole-context' zwraca domknięcie w miejscu 'hole'

### # 07-fresh-vars
implementacja wychwytywania świeżych zmiennych,  
tj. czy nowodeklarowana zmienna jest przykrywana, czy nowa (nieużyta)  
podobnie jak kompilator -> przemianowujemy zmienne na x(i), i€N  
i wyrzucamy ich deklaracje na początek programu

### # 08a-arith-evaluator
kalkulator wyrażeń arytmetycznych
(+ 2 3) -> 5

### # 08b-lazy-let
zdefiniowanie lazy-leta do własnego języka  
lazy-let to wyrażenie przechowujące domknięcie i wyrażenie,  
które ma zostać obliczone przy zawołaniu zmiennej.  
(let x (1+1)) -> oblicza 1+1=2 i używa x=2  
(lazy-let x (1+1)) -> nie oblicza 1+1 i zawsze przy użyciu x oblicza 1+1  

### # 09-rand-and-fermat
rozszerzenie języka o konstrukcję rand (przekazywanie cały czas ziarna (stanu))  
wykorzystanie randa do testu Fermata (losowe testowanie pierwszości liczb)

### # 10-arith-parser
parser wyrażeń arytmetycznych (parsuje np. 5+3*2-(1-6)-7 )  
zamienia na drzewo i oblicza wynik

### # 11
brak pracowni :)

#### 12-fifo-lifo
implementacja stosu i kolejki za pomocą listy  
(kolejka -> pomysł z dwiema listami i reverse)

### # 13-nonograms
rozwiązywacz nonogramów (logiczna układanka, picross)

### # 14-objective-rpg-textgame
gra rpg, bieganie po instytucie informatyki i rozwiązywanie zadań

