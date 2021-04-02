## router
Program symuluje zachowanie zewnętrznego routera jakiejś sieci. Odpalany na wielu różnych komputerach/maszynach połączonych ze sobą, czyta IP swoich sąsiadów i dodaje ich do swojej tablicy osiągalnych sieci. Utrzymuje informacje o tych sieciach używając algorytmu wektorów odległości (wysyła swoją tablicę odległości do sąsiadów i odbiera info od innych). Wysyłane pakiety idą protokołem UDP. Router reaguje wyłączenie którejś karty sieciowej bądź sąsiada (maszyny) i regularnie próbuje ponownie nawiązać połączenie. Przykład użycia został opisany w <a href="https://github.com/TheKetrab/University/blob/master/Sieci%20Komputerowe/router/tests/README.md">router/tests/README.md</a>

## traceroute
Program symuluje wykonanie linuksowego traceroute'a. Wysyła po 3 żądania z coraz większym TTL (Time To Live w icmp_header) i mierzy średni RRT (Round Trip Time) ile czasu czekaliśmy na odpowiedź (echo reply).

Przykład użycia:
```
$ sudo ./traceroute 156.17.254.113

> 1. 192.168.55.1 2.21 ms
> 2. 10.108.85.1 3.40 ms
> 3. 91.224.105.17 7.01 ms
> 4. 91.198.97.46 4.89 ms
> 5. 80.50.238.57 5.34 ms
> 6. 195.205.0.106 13.95 ms
> 7. 80.50.231.22 31.50 ms
> 8. 212.191.224.106 35.16 ms
> 9. 156.17.254.113 41.29 ms
```

## transport
Program symuluje ramkę TCP (algorytm okna przesuwnego) wysyłając odpowiednie żądania do serwera przez protokół UDP. Program pobiera plik z serwera i zapisuje bajty do pliku podanego na wejście.

## webserver
Jest to mój własny server HTTP napisany w C. Użycie:
```
             PORT  KATALOG 
 ./webserver 8888 webpages/ 
```
W katalogu umieścić folder o nazwie hosta, a w środku zasoby oraz index.html
Przykład:
```
--webserver
  |--localhost
     |--index.html
     |--plik.txt
     |--file.bin
     |--css
        |--style.css
```
Teraz można połączyć się przeglądarką z serwerem:
localhost:8888/index.html
