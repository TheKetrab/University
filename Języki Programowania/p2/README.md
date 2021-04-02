
1. Interpretacja polecenia
   a) Termy uznałem domyślnie za nameless (przejście z postaci z nazwami zmiennych
      na indeksy De'Bruijna było tematem poprzedniego zadania pracownianego (moduł DeBruijn.ml)).
   b) Posługujemy się rachunkiem a'la Church, więc argument abstrakcji należy jawnie otypować.
      Dlatego zapis termu λx. λy (x+y) to: Lam(Nat,Lam(Nat,Add(Var(1),Var(0)))) (postać nameless)
   c) Gdybyśmy używali postaci z nazwami zmiennych, to musielibyśmy po prostu sprawdzić, czy nazwa zmiennej
      nie należy przypadkiem do zbioru wyjątków (delta) - nie chcemy mieć zmiennych, które mają nazwy jak wyjątki
   d) Wyjątki rozróżniamy po nazwie i typie, to znaczy możemy mieć dwa wyjątki o tej samej nazwie, ale różnym typie:
        Exception("name1",tyT1,
          Exception("name1",tyT2,
            ... ))
   e) Przez postać nameless zmiennych, zmienia się trochę interpretacja catch:
        zamiast: catch { ei xi => ti }
        mamy:    catch { ei => ti }
      Jeśli ti nie jest lambdą, to argument wyrzucony przez throw zignorujemy.
      Jeśli ti jest lambdą, to argument wyrzucony przez throw obliczymy
        i dodamy do kontekstu i zaaplikujemy, czyli wszystkie odwołania do Var(0)
        w ti to będzie właśnie argument rzucony przez throw
      Jeśli ti oblicza się do lambdy np: (if true then Lam(Nat,Var(0)) else ...),
        ale sam w sobie nie jest lambdą, to NIE TRAKTUJEMY tego jako użycie argumentu wyjątku,
        tylko ignorujemy rzucony przez throw argument i zwracamy term ti
        (czyli, żeby dobrze się otypowało ti musi mieć oczekiwany typ bez wywołania z argumentem)
      Nazywam to ŚWIADOMYM UŻYCIEM WYJĄTKU.
      Możemy dzięki temu napisać:
        try { 17 }
        catch { "e1" -> 42 }            (* argument zignorowany, zwraca Nat *)
        catch { "e2" -> Lam(tyT,42) }   (* argument użyty, więc zwraca Nat *)
      ale tu już będzie typeerror:
        catch { "e3" -> App(Lam(Unit,Lam(tyT,42))    UnitVal)}
        (* jakby obliczyć ti, to wyszłoby Lam(tyT,42), ale tutaj NIE UŻYWAMY ŚWIADOMIE
           argumentu, zatem typem będzie Arr(tyT,Nat), a nie Nat -> dlatego typeerror *)

2. Struktura
   W module Lambda zdefiniowany jest język termów, którego używamy
   oraz reguły ewaluacji (call by value) i inferencja typów.
   Skrypt Test.ml zawiera testy automatyczne. Można w nim dopisywać
   własne testy w celu dalszego testowania.

3. Kompilacja
   Należy skompilować moduł Lambda - poleceniem 'make' uruchamiamy przygotowany Makefile.

4. Uruchamianie programu
   W ramach 'programu' moduł Lambda udostępnia trzy funkcje:
   a) typeof - zwraca typ termu lub wyrzuca wyjątek TypeError
   b) eval - sprawdza, czy wyrażenie da się otypować
             (jeśli nie, to rzuca wyjątek TypeError) i ewaluuje term;
             zwraca wartość i typ danego wyrażenia;
             może rzucić wyjątek UnhandledException
   c) run - wykonuje funkcję eval i łapie potencjalne wyjątki;
            wypisuje na wyjście informacje o finalnym stanie obliczeń

   W skrypcie Test.ml pokazane są przykłady użycia procedury 'run'.
   Należy odpalić skrypt: ocaml < Test.ml

-----
Zadanie zostało zrealizowane w części podstawowej (niebonusowej).
Bardziej szczegółowe komentarze znajdują się w poszczególnych plikach.