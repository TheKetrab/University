
1. Struktura
   Przygotowane rozwiązanie zawiera kilka modułów, które można użyć
   do obliczeń wyrażeń w rachunku lambda.
      a) Lambda: definiuje wyrażenia w rachunku lambda
      b) DeBruijn: moduł przeznaczony do prowadzenia obliczeń
         na termach w postaci 'nameless'
      c) Util: kilka pomocniczych procedur
      d) Parser: dokonuje konwersji pomiędzy składnią konkretną i abstrakcyjną języka
      e) Main: program wykonujący polecenia podane na standardowym wejściu
      f) Test: skrypt z testami automatycznymi

2. Kompilacja
   Program należy skompilować używając GNU Make. W katalogu znajduje się przygotowany
   plik Makefile. Poleceniem 'make' można skompilować całe rozwiązanie i wygenerować
   odpowiednie moduły oraz program: prog. Poleceniem 'make clean' można usunąć
   wygenerowane pliki .cmo, .cmi oraz program prog.

3. Uruchamianie programu
   Skompilowany program należy uruchomić poleceniem:
   ./prog IN_FILE OUT_FILE [n|normalize=default|c|compare] [cbn=default|cbv]
     - 1 argument to plik z danymi wejściowymi.
     - 2 argument to plik, do którego zapisane będzie wyjście.
     - (opcjonalnie) 3 argument to tryb działania programu:
       normalizacja termu albo porównywanie dwóch termów
     - (opcjonalnie) 4 argument to strategia ewaluacji:
       semantyka naturalna call by name albo semantyka naturalna call by value

    Przykłady:
    ./prog input output        // normalizacja termu uzywając cbn
    ./prog input output n cbv  // normalizacja termu używając cbv (bo np. cbn trwało by zbyt długo)
    ./prog input output c      // porównywanie termów używając cbn
    ./prog input output c cbv  // porównywanie termów używając cbv 

    UWAGA:
    W trybie 'normalize' plik wejściowy musi mieć tylko jedną linijkę, w której
    zapisany jest term. W trybie 'compare' plik wejsciowy musi mieć dokładnie
    dwie linijki, w których kolejno zapisane są t1 i t2 do porównania.

    Konkretny przykład:
      input:
        if false 5 add ( mul 1 3 ) 5
        ( \x. sub x ( if true 5 2 ) ) 13
      run:
        ./prog input output c cbv // użycie cbn może potrwać kilka sekund
      output:
        true

-----
Wszystkie części zadania zostały zrealizowane.
Bardziej szczegółowe komentarze znajdują się w poszczególnych plikach.
