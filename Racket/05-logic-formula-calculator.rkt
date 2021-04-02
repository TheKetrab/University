#lang racket


;; pomocnicza funkcja dla list tagowanych o określonej długości
(define (tagged-tuple? tag len p)
  (and (list? p)
       (= (length p) len)
       (eq? (car p) tag)))

(define (tagged-list? tag p)
  (and (pair? p)
       (eq? (car p) tag)
       (list? (cdr p))))

;; reprezentacja danych wejściowych (z ćwiczeń)
(define (var? x)
  (symbol? x))

(define (var x)
  x)

(define (var-name x)
  x)

;; przydatne predykaty na zmiennych
(define (var<? x y)
  (symbol<? x y))

(define (var=? x y)
  (eq? x y))

(define (literal? x)
  (and (tagged-tuple? 'literal 3 x)
       (boolean? (cadr x))
       (var? (caddr x))))

(define (literal pol x)
  (list 'literal pol x))

(define (literal-pol x)
  (cadr x))

(define (literal-var x)
  (caddr x))

(define (clause? x)
  (and (tagged-list? 'clause x)
       (andmap literal? (cdr x))))

(define (clause . lits)
  (cons 'clause lits))

(define (clause-lits c)
  (cdr c))

(define (cnf? x)
  (and (tagged-list? 'cnf x)
       (andmap clause? (cdr x))))

(define (cnf . cs)
    (cons 'cnf cs))

(define (cnf-clauses x)
  (cdr x))


;; oblicza wartość formuły w CNF z częściowym wartościowaniem. jeśli zmienna nie jest
;; zwartościowana, literał jest uznawany za fałszywy.
(define (valuate-partial val form)
  (define (val-lit l)
    (let ((r (assoc (literal-var l) val)))
      (cond
       [(not r)  false]
       [(cadr r) (literal-pol l)]
       [else     (not (literal-pol l))])))
  (define (val-clause c)
    (ormap val-lit (clause-lits c)))
  (andmap val-clause (cnf-clauses form)))

;; reprezentacja dowodów sprzeczności

(define (axiom? p)
  (tagged-tuple? 'axiom 2 p))

(define (proof-axiom c)
  (list 'axiom c))

(define (axiom-clause p)
  (cadr p))

(define (res? p)
  (tagged-tuple? 'resolve 4 p))

(define (proof-res x pp pn)
  (list 'resolve x pp pn))

(define (res-var p)
  (cadr p))

(define (res-proof-pos p)
  (caddr p))

(define (res-proof-neg p)
  (cadddr p))

;; sprawdza strukturę, ale nie poprawność dowodu
(define (proof? p)
  (or (and (axiom? p)
           (clause? (axiom-clause p)))
      (and (res? p)
           (var? (res-var p))
           (proof? (res-proof-pos p))
           (proof? (res-proof-neg p)))))

;; procedura sprawdzająca poprawność dowodu
(define (check-proof pf form)
  (define (run-axiom c)
    (displayln (list 'checking 'axiom c))
    (and (member c (cnf-clauses form))
         (clause-lits c)))
  (define (run-res x cpos cneg)
    (displayln (list 'checking 'resolution 'of x 'for cpos 'and cneg))
    (and (findf (lambda (l) (and (literal-pol l)
                                 (eq? x (literal-var l))))
                cpos)
         (findf (lambda (l) (and (not (literal-pol l))
                                 (eq? x (literal-var l))))
                cneg)
         (append (remove* (list (literal true x))  cpos)
                 (remove* (list (literal false x)) cneg))))
  (define (run-proof pf)
    (cond
     [(axiom? pf) (run-axiom (axiom-clause pf))]
     [(res? pf)   (run-res (res-var pf)
                           (run-proof (res-proof-pos pf))
                           (run-proof (res-proof-neg pf)))]
     [else        false]))
  (null? (run-proof pf)))


;; reprezentacja wewnętrzna

;; sprawdza posortowanie w porządku ściśle rosnącym, bez duplikatów
(define (sorted? vs)
  (or (null? vs)
      (null? (cdr vs))
      (and (var<? (car vs) (cadr vs))
           (sorted? (cdr vs)))))

(define (sorted-varlist? x)
  (and (list? x)
       (andmap (var? x))
       (sorted? x)))

;; klauzulę reprezentujemy jako parę list — osobno wystąpienia pozytywne i negatywne. Dodatkowo
;; pamiętamy wyprowadzenie tej klauzuli (dowód) i jej rozmiar.
(define (res-clause? x)
  (and (tagged-tuple? 'res-int 5 x)
       (sorted-varlist? (second x))
       (sorted-varlist? (third x))
       (= (fourth x) (+ (length (second x)) (length (third x))))
       (proof? (fifth x))))

(define (res-clause pos neg proof)
  (list 'res-int pos neg (+ (length pos) (length neg)) proof))

(define (res-clause-pos c)
  (second c))

(define (res-clause-neg c)
  (third c))

(define (res-clause-size c)
  (fourth c))

(define (res-clause-proof c)
  (fifth c))

;; przedstawia klauzulę jako parę list zmiennych występujących odpowiednio pozytywnie i negatywnie
(define (print-res-clause c)
  (list (res-clause-pos c) (res-clause-neg c)))

;; sprawdzanie klauzuli sprzecznej
(define (clause-false? c)
  (and (null? (res-clause-pos c))
       (null? (res-clause-neg c))))

;; pomocnicze procedury: scalanie i usuwanie duplikatów z list posortowanych
(define (merge-vars xs ys)
  (cond [(null? xs) ys]
        [(null? ys) xs]
        [(var<? (car xs) (car ys))
         (cons (car xs) (merge-vars (cdr xs) ys))]
        [(var<? (car ys) (car xs))
         (cons (car ys) (merge-vars xs (cdr ys)))]
        [else (cons (car xs) (merge-vars (cdr xs) (cdr ys)))]))

(define (remove-duplicates-vars xs)
  (cond [(null? xs) xs]
        [(null? (cdr xs)) xs]
        [(var=? (car xs) (cadr xs)) (remove-duplicates-vars (cdr xs))]
        [else (cons (car xs) (remove-duplicates-vars (cdr xs)))]))

(define (rev-append xs ys)
  (if (null? xs) ys
      (rev-append (cdr xs) (cons (car xs) ys))))

;; ----- WLASNE FUNKCJE POMOCNICZE -----

(define not-empty?
  (not empty?))

(define (not-null? x)
  (not (eq? x null)))

(define (is-member-of? v xs)
  (cond [(null? xs) #f]
        [(eq? (car xs) v) #t]
        [else (is-member-of? v (cdr xs))]))

;; ----- ----- ----- ----- -----
;; zwraca pierwszy napotkany wspolny element,
;; korzysta z tego, ze listy sa posortowane;
;; jesli nic nie znajdzie, zwroci wartosc null
(define (check-cross list-pos list-neg)  
  (cond
    ;; jesli ktoras sie skonczyla, to zakoncz i zwroc null
    ((or [null? list-pos] [null? list-neg])
     null)
    ;; jesli natrafiles na takie same zmienne, to zwroc ta zmienna
    ((var=? (car list-pos) (car list-neg))
     (car list-pos))
    ;; jesli zmienna pozytywna jest mniejsza, to sprawdz jej nastepna...
    ((var<? (car list-pos) (car list-neg))
     (check-cross (cdr list-pos) list-neg))
    ;; jesli zmienna negatywna jest mniejsza, to sprawdz jej nastepna...
    (else
     (check-cross list-pos (cdr list-neg)))))

;; ----- ----- ----- ----- -----
;; jesli procedura check-cross zwroci null,
;; to klauzula nie jest trywialna
(define (clause-trivial? c)
  (not-null? (check-cross (res-clause-pos c) (res-clause-neg c))))

;; ----- ----- ----- ----- -----  
(define (resolve c1 c2)
  (let ([A-pos (res-clause-pos c1)]
        [A-neg (res-clause-neg c1)]
        [B-pos (res-clause-pos c2)]
        [B-neg (res-clause-neg c2)])

    ;; chcemy stworzyc nowa klauzule

    (let ([first-try (check-cross A-pos B-neg)])
      (if (not-null? first-try)
          ; -> stworz nowa klauzule
          (let*
              ;; v-- scalone pozytywne z A i B, usuniete duplikaty, z wyrzuconym first-try, wszystko posortowane
              ([new-pos (remove-duplicates-vars (merge-vars (remove first-try A-pos)
                                                            B-pos))] 
               ;; v-- scalone negatywne z A i B, usuniete duplikaty, z wyrzuconym first-try, wszystko posortowane
               [new-neg (remove-duplicates-vars (merge-vars (remove first-try B-neg)
                                                            A-neg))]
               [new-proof (proof-res first-try A-pos B-neg)]
               [new-clause (res-clause new-pos new-neg new-proof)])
            (if (clause-false? new-clause)
                #f ;; to nie dorzucaj jej do zbioru rezolwent
                new-clause))

          (let ([second-try (check-cross B-pos A-neg)])
            (if (not-null? second-try)
                ; -> stworz nowa klauzule
                (let*
                    ;; v-- scalone pozytywne z A i B, usuniete duplikaty, z wyrzuconym first-try, wszystko posortowane
                    ([new-pos (remove-duplicates-vars (merge-vars (remove second-try B-pos)
                                                                  A-pos))] 
                     ;; v-- scalone negatywne z A i B, usuniete duplikaty, z wyrzuconym first-try, wszystko posortowane
                     [new-neg (remove-duplicates-vars (merge-vars (remove second-try A-neg)
                                                                  B-neg))] 
                     [new-proof (proof-res second-try B-pos A-neg)]
                     [new-clause (res-clause new-pos new-neg new-proof)])
                  (if (clause-false? new-clause)
                      #f ;; ??? zakoncz dowod ???
                      new-clause))

                ; jesli przeciecie jest puste :
                #f))))))

;; ----- ----- ----- ----- -----
;; TESTY powyzszych procedur:

;; Testy clause-trivial?
;(define moja (cnf . ((clause . ((literal #t 'a)
;                                (literal #t 'p)
;                                (literal #f 'b)))
;                     (clause . ((literal #t 'p)
;                                (literal #f 'q)
;                                (literal #f 'x)
;                                (literal #f 'b)))
;                     (clause . ((literal #t 'q)
;                                (literal #f 'q))))))
;(define przetworzona (form->clauses moja))
;(clause-trivial? (car przetworzona)) ---> #f
;(clause-trivial? (cadr przetworzona)) ---> #f
;(clause-trivial? (caddr przetworzona)) ---> #t

;; Testy resolve
;(define moja (cnf . ((clause . ((literal #t 'p)
;                                (literal #t 'q)
;                                (literal #t 'r)))
;                     (clause . ((literal #t 'p)
;                                (literal #f 'q)
;                                (literal #t 's)))
;                     (clause . ((literal #t 'r)
;                                (literal #f 'p)
;                                (literal #f 'q)))
;                     (clause . ((literal #f 'p)))
;                     (clause . ((literal #t 'p)))
;                     (clause . ((literal #t 'q)
;                                (literal #f 'x))))))
;(define przetworzona (form->clauses moja))
;(resolve (car przetworzona) (cadr przetworzona)) --> '(res-int (p r s) () 3 (resolve q (p q r) (q)))
;(resolve (car przetworzona) (caddr przetworzona)) --> '(res-int (q r) (q) 3 (resolve p (p q r) (p q)))
;(resolve (fourth przetworzona) (fifth przetworzona)) --> #f wyrezolwowano klauzule sprzeczna
;(resolve (fifth przetworzona) (sixth przetworzona)) --> #f nie ma czego rezolwowac

;; ----- ----- ----- ----- -----


(define (resolve-single-prove s-clause checked pending)

  ; procedura zastepuje liste checked lista podmieniona
  ; o to, co da sie zrezolwowac z single clause
  (define (modify-checked-list xs acc)
    (if (null? xs) acc
        (let ([res (resolve s-clause (car xs))])
          ; jesli resolve zwrocilo #f, tzn ze nie da sie zrezolwowac
          ; wpp. zastap ten element na liscie chekced rezolwenta
          (if (eq? #f res)
              (modify-checked-list (cdr xs) (cons (car xs) acc))
              (modify-checked-list (cdr xs) (cons res acc))))))
           
  (let* ((resolvents   (filter-map (lambda (c) (resolve c s-clause))
                                     checked))
         (sorted-rs    (sort resolvents < #:key res-clause-size))
         (sorted-new-checked (sort-clauses (cons s-clause
                                                 (modify-checked-list checked '() )))))
    (subsume-add-prove sorted-new-checked pending sorted-rs)))


;; wstawianie klauzuli w posortowaną względem rozmiaru listę klauzul
(define (insert nc ncs)
  (cond
   [(null? ncs)                     (list nc)]
   [(< (res-clause-size nc)
       (res-clause-size (car ncs))) (cons nc ncs)]
   [else                            (cons (car ncs) (insert nc (cdr ncs)))]))

;; sortowanie klauzul względem rozmiaru (funkcja biblioteczna sort)
(define (sort-clauses cs)
  (sort cs < #:key res-clause-size))

;; główna procedura szukająca dowodu sprzeczności
;; zakładamy że w checked i pending nigdy nie ma klauzuli sprzecznej
(define (resolve-prove checked pending)
  (cond
   ;; jeśli lista pending jest pusta, to checked jest zamknięta na rezolucję czyli spełnialna
   [(null? pending) (generate-valuation (sort-clauses checked))]
   ;; jeśli klauzula ma jeden literał, to możemy traktować łatwo i efektywnie ją przetworzyć
   [(= 1 (res-clause-size (car pending)))
    (resolve-single-prove (car pending) checked (cdr pending))]
   ;; w przeciwnym wypadku wykonujemy rezolucję z wszystkimi klauzulami już sprawdzonymi, a
   ;; następnie dodajemy otrzymane klauzule do zbioru i kontynuujemy obliczenia
   [else
    (let* ((next-clause  (car pending))
           (rest-pending (cdr pending))
           (resolvents   (filter-map (lambda (c) (resolve c next-clause))
                                     checked))
           (sorted-rs    (sort-clauses resolvents)))
      (subsume-add-prove (cons next-clause checked) rest-pending sorted-rs))]))

;; procedura upraszczająca stan obliczeń biorąc pod uwagę świeżo wygenerowane klauzule i
;; kontynuująca obliczenia. Do uzupełnienia.
(define (subsume-add-prove checked pending new)

  ; procedura sprawdza czy c1 zawiera sie w c2
  (define (include? c1 c2)
    ; procedura sprawdza czy xs zawiera sie w ys - sprawdza pos/neg
    (define (try? xs ys)
      (cond [(null? xs) #t]
            [(null? ys) #f]
            [(var=? (car xs) (car ys))
             (try? (cdr xs) (cdr ys))]
            [else (try? xs (cdr ys))]))

    (let ([A-pos (res-clause-pos c1)]
          [A-neg (res-clause-neg c1)]
          [B-pos (res-clause-pos c2)]
          [B-neg (res-clause-neg c2)])
      ; klauzula pusta/sprzeczna nie jest traktowana jako zawieranie
      (if (and (empty? A-pos) (empty? A-neg))
          #f
          (and (try? A-pos B-pos) (try? A-neg B-neg)))))
  ; procedura ktora usuwa z acc nadklauzule klauzuli claus...
  (define (removing-from-list claus xs acc)
    (cond [(null? xs) acc]
          [(include? claus (car xs))
           (removing-from-list claus (cdr xs) (remove* (car xs) acc))]
          [else (removing-from-list claus (cdr xs) acc)]))

  (cond
   [(null? new)                 (resolve-prove checked pending)]
   ;; jeśli klauzula do przetworzenia jest sprzeczna to jej wyprowadzenie jest dowodem sprzeczności
   ;; początkowej formuły
   [(clause-false? (car new))   (list 'unsat (res-clause-proof (car new)))]
   ;; jeśli klauzula jest trywialna to nie ma potrzeby jej przetwarzać
   [(clause-trivial? (car new)) (subsume-add-prove checked pending (cdr new))]
   [else
    (cond
      ; jesli ktorakolwiek z checked zawiera sie w rezolwencie, to jej (rezolwenty) nie dodawaj
      [(ormap (lambda (x) (include? x (car new))) checked)
       (subsume-add-prove checked pending (cdr new))]
      ; jesli ktorakolwiek z pending zawiera sie w rezolwencie, to jej (rezolwenty) nie dodawaj
      [(ormap (lambda (x) (include? x (car new))) pending)
       (subsume-add-prove checked pending (cdr new))]
      ; jesli nowa rezolwenta zawiera sie w checked albo pending, to wyczysc zbedne klauzule
      [(or (ormap (lambda (x) (include? (car new) x)) checked)
           (ormap (lambda (x) (include? (car new) x)) pending))
       (let* ([cleared-check (removing-from-list (car new) checked checked)]
              [cleared-pending (removing-from-list (car new) pending pending)])
         (subsume-add-prove cleared-check (insert (car new) cleared-pending) (cdr new)))]
      ; jesli nie da sie nic skrocic, to po prostu dodaj rezolwente do pending
      [else (subsume-add-prove checked (insert (car new) pending) (cdr new))])]))

;; ----- ----- ----- -----

(define (generate-valuation resolved)

  ;; -> zwraca liste obcieta o wystapienia owej zmiennej
  (define (cut-list-by-positive v xs)
    (filter (lambda (x) (not (is-member-of? v (res-clause-pos x)))) xs))
  (define (cut-list-by-negative v xs)
    (filter (lambda (x) (not (is-member-of? v (res-clause-neg x)))) xs))

  ;; -> dostajesz liste klauzul posortowana wg dlugosci
  (define (make-valuation-list claus acc)
     
      (cond
        ; jesli koniec, to to, co masz
        [(null? claus) acc]
        ; jesli pojedyncza zmienna pozytywna
        [(and (= (length (res-clause-pos (car claus))) 1)
              (empty? (res-clause-neg (car claus))))
         ; to wyciagnij ja, skroc liste do sprawdzenia, dodaj do listy wynikowej
         (let ([var-to-add (car (res-clause-pos (car claus)))])
           (make-valuation-list (cut-list-by-positive var-to-add claus)
                                (cons (list var-to-add #t) acc)))]
        ; jesli pojedyncza zmienna negatywna
        [(and (= (length (res-clause-neg (car claus))) 1)
              (empty? (res-clause-pos (car claus))))
         ; to wyciagnij ja, skroc liste do sprawdzenia, dodaj do listy wynikowej
         (let ([var-to-add (car (res-clause-neg (car claus)))])
           (make-valuation-list (cut-list-by-negative var-to-add claus)
                                (cons (list var-to-add #f) acc)))]
        ; jesli niepusta lista pozytywna, to wez pierwszy element
        [(not (empty? (res-clause-pos (car claus))))
         (let ([var-to-add (car (res-clause-pos (car claus)))])
           (make-valuation-list (cut-list-by-positive var-to-add claus)
                                (cons (list var-to-add #t) acc)))]

        ; jesli niepusta lista negatywna, to wez pierwszy element
        [(not (empty? (res-clause-pos (car claus))))
         (let ([var-to-add (car (res-clause-neg (car claus)))])
           (make-valuation-list (cut-list-by-negative var-to-add claus)
                                (cons (list var-to-add #f) acc)))]

        ; jesli ktoras klauzula sie stala pusta...
        [else (make-valuation-list (cdr claus) acc)]))

  ;; -> zwraca liste list dl 2 : zmienna, t/f
  (list 'sat (make-valuation-list resolved '())))

;; ----- ----- ----- ----- -----
;; TESTY
;; ----- ----- ----- ----- -----
; --> procedura generate-valuation dostaje tylko
;     klauzule domkniete na rezolucje, wiec tylko
;     takie moge dolaczyc do testow.
;(define moja1 (cnf . ((clause . ((literal #t 'p)))
;                     (clause . ((literal #t 'p)
;                                (literal #t 'q)))
;                     (clause . ((literal #t 'r)))
;                     (clause . ((literal #t 'q)
;                                (literal #t 'x))))))
;
;(define moja2 (cnf . ((clause . ((literal #t 'p)))
;                     (clause . ((literal #t 'x)))
;                     (clause . ((literal #t 'x)
;                                (literal #t 's)))
;                     (clause . ((literal #f 'r)
;                                (literal #t 'p)))
;                     (clause . ((literal #f 'r)
;                                (literal #t 'x)
;                                (literal #t 'p)))
;                     (clause . ((literal #f 'a))))))
;
;(define moja3 (cnf . ((clause . ((literal #t 'p)
;                                 (literal #t 'q)))
;                      (clause . ((literal #t 'q)
;                                 (literal #t 'r)))
;                      (clause . ((literal #t 'r)
;                                 (literal #t 's)))
;                      (clause . ((literal #t 's)
;                                 (literal #t 'p))))))

;(define przetworzona1 (form->clauses moja1))
;(define przetworzona2 (form->clauses moja2))
;(define przetworzona3 (form->clauses moja3))
;(generate-valuation przetworzona1) -----> '(sat ((q #t) (r #t) (p #t)))
;(generate-valuation przetworzona2) -----> '(sat ((a #f) (x #t) (p #t)))
;(generate-valuation przetworzona3) -----> '(sat ((r #t) (q #t) (p #t)))

;; ----- ----- ----- -----

;; procedura przetwarzające wejściowy CNF na wewnętrzną reprezentację klauzul
(define (form->clauses f)
  (define (conv-clause c)
    (define (aux ls pos neg)
      (cond
       [(null? ls)
        (res-clause (remove-duplicates-vars (sort pos var<?))
                    (remove-duplicates-vars (sort neg var<?))
                    (proof-axiom c))]
       [(literal-pol (car ls))
        (aux (cdr ls)
             (cons (literal-var (car ls)) pos)
             neg)]
       [else
        (aux (cdr ls)
             pos
             (cons (literal-var (car ls)) neg))]))
    (aux (clause-lits c) null null))
  (map conv-clause (cnf-clauses f)))

(define (prove form)
  (let* ((clauses (form->clauses form)))
    (subsume-add-prove '() '() clauses)))

;; procedura testująca: próbuje dowieść sprzeczność formuły i sprawdza czy wygenerowany
;; dowód/waluacja są poprawne. Uwaga: żeby działała dla formuł spełnialnych trzeba umieć wygenerować
;; poprawną waluację.
(define (prove-and-check form)
  (let* ((res (prove form))
         (sat (car res))
         (pf-val (cadr res)))
    (if (eq? sat 'sat)
        (valuate-partial pf-val form)
        (check-proof pf-val form))))


;; ----- ----- ----- ----- -----
;; ----- TESTY rezolucji -----
;; ----- ----- ----- ----- -----

; niedbala procedura do testow; jesli formula
; jest spelnialna, to wypisze przykladowe wartosciowanie
(define (prove-and-display-valuation cnf-formula)
  (if (eq? #f (prove-and-check cnf-formula))
      #f
      (prove cnf-formula)))
; ---
(define moja1 (cnf . ((clause . ((literal #t 'p)
                                 (literal #t 'r)
                                 (literal #t 'z)
                                 (literal #f 'x)))
                     (clause . ((literal #t 'x)
                                (literal #f 'p)))
                     (clause . ((literal #f 'a)
                                (literal #t 'r)))
                     (clause . ((literal #t 'a))))))
(define moja2 (cnf . ((clause . ((literal #t 'p)
                                 (literal #t 'q)))
                     (clause . ((literal #t 'q)
                                (literal #f 'r)))
                     (clause . ((literal #t 'r)))
                     (clause . ((literal #f 'q)
                                (literal #f 'p)
                                (literal #f 'r))))))
(define moja3 (cnf . ((clause . ((literal #t 'p)
                                 (literal #t 'q)))
                      (clause . ((literal #t 'q)
                                 (literal #t 'r)))
                      (clause . ((literal #t 'r)
                                 (literal #t 's)))
                      (clause . ((literal #t 's)
                                 (literal #t 'p))))))
(define moja4 (cnf . ((clause . ((literal #t 'p)))
                      (clause . ((literal #f 'p))))))
; ---
; z Logika dla informatykow 2017 zad 125 (moja5), zad 126 (moja6), zad 127 (moja7)

(define moja5 (cnf . ((clause . ((literal #f 'p)
                                 (literal #t 'q)))
                      (clause . ((literal #f 'p)
                                 (literal #f 'r)
                                 (literal #t 's)))
                      (clause . ((literal #f 'q)
                                 (literal #t 'r)))
                      (clause . ((literal #t 'p)))
                      (clause . ((literal #f 's))))))
; -----



(define moja6 (cnf . ((clause . ((literal #t 'p)
                                 (literal #t 'r)))
                      (clause . ((literal #f 'r)
                                 (literal #f 's)))
                      (clause . ((literal #t 'q)
                                 (literal #t 's)))
                      (clause . ((literal #t 'q)
                                 (literal #t 'r)))
                      (clause . ((literal #f 'p)
                                 (literal #f 'q)))
                      (clause . ((literal #t 's)
                                 (literal #t 'p))))))
; -----
(define moja7 (cnf . ((clause . ((literal #t 'p)
                                 (literal #t 'q)
                                 (literal #t 'r)))
                      (clause . ((literal #f 'r)
                                 (literal #f 'q)
                                 (literal #f 'r)))
                      (clause . ((literal #f 'q)
                                 (literal #t 'r)))
                      (clause . ((literal #f 'r)
                                 (literal #t 'p))))))

; ----- DOWODY -----
(prove-and-display-valuation moja1)
(prove-and-display-valuation moja2)
(prove-and-display-valuation moja3)
(prove-and-display-valuation moja4)
(prove-and-display-valuation moja5)
(prove-and-display-valuation moja6)
(prove-and-display-valuation moja7)


