#lang racket

;; expressions

(define (const? t)
  (number? t))

(define (op? t)
  (and (list? t)
       (member (car t) '(+ - * /))))

(define (op-op e)
  (car e))

(define (op-args e)
  (cdr e))

(define (op-cons op args)
  (cons op args))

(define (op->proc op)
  (cond [(eq? op '+) +]
        [(eq? op '*) *]
        [(eq? op '-) -]
        [(eq? op '/) /]))

(define (let-def? t)
  (and (list? t)
       (= (length t) 2)
       (symbol? (car t))))

(define (let-def-var e)
  (car e))

(define (let-def-expr e)
  (cadr e))

(define (let-def-cons x e)
  (list x e))

(define (let? t)
  (and (list? t)
       (= (length t) 3)
       (eq? (car t) 'let)
       (let-def? (cadr t))))

(define (let-def e)
  (cadr e))

(define (let-expr e)
  (caddr e))

(define (let-cons def e)
  (list 'let def e))

(define (var? t)
  (symbol? t))

(define (var-var e)
  e)

(define (var-cons x)
  x)

(define (arith/let-expr? t)
  (or (const? t)
      (and (op? t)
           (andmap arith/let-expr? (op-args t)))
      (and (let? t)
           (arith/let-expr? (let-expr t))
           (arith/let-expr? (let-def-expr (let-def t))))
      (var? t)))

;; let-lifted expressions

(define (arith-expr? t)
  (or (const? t)
      (and (op? t)
           (andmap arith-expr? (op-args t)))
      (var? t)))

(define (let-lifted-expr? t)
  (or (and (let? t)
           (let-lifted-expr? (let-expr t))
           (arith-expr? (let-def-expr (let-def t))))
      (arith-expr? t)))

;; generating a symbol using a counter

(define (number->symbol i)
  (string->symbol (string-append "x" (number->string i))))

;; environments (could be useful for something)

(define empty-env
  null)

(define (add-to-env x v env)
  (cons (list x v) env))

(define (find-in-env x env)
  (cond [(null? env) (error "undefined variable" x)]
        [(eq? x (caar env)) (cadar env)]
        [else (find-in-env x (cdr env))]))


; ===== MOJE PROCEDURY =====

; ----- ----- ----- ---- -----
; procedura usuwa zbedne nawiasy
; procedura ma PROBLEM z listami typu '((* 2 3)),
; (clear-list '((* 2 3)))
; --> '((* 2 3))
; ALE
; (clear-list '((((((* 2 3)))))))
; TEZ zwroci --> '((* 2 3))
(define (clear-list xs)
  (cond
    [(null? xs) null]
    [(or (symbol? xs)
         (number? xs))
     xs]
    [(and (list? (car xs))
          (= (length (car xs)) 1))
     (append (clear-list (car xs)) (clear-list (cdr xs)))]
    [else (append (list (clear-list (car xs))) (clear-list (cdr xs)))]))

; ----- ----- -----
; TESTY

;(clear-list '( 5 ((2 (((3 (((((4)) 2)))))) 5))))
; --> '(5 (2 (3 (4 2)) 5))

;(clear-list '((((2)) 4) 3))
; --> '((2 4) 3)

;(clear-list '( 2 3 4 5 6 7 ))
; --> '(2 3 4 5 6 7)

;(clear-list '((((2) (3) (5) (6 7 8))) 4))
; --> '((2 3 5 (6 7 8)) 4)

;(clear-list '((((())))))
; --> '(())

;(clear-list '(2 3 ()))
; --> '(2 3 ())

; ----- ----- -----


; ----- ----- ----- ----- -----
; zastepuje wszystkie symbole
; s1 w liscie xs na symbol s2
(define (replace-in-list s1 s2 xs)
  
  (define (iter xs)
    (cond [(null? xs) null]
          [(list? (car xs))
           (cons (replace-in-list s1 s2 (car xs))
                 (replace-in-list s1 s2 (cdr xs)))]
          [(eq? (car xs) s1)
           (cons s2
                 (replace-in-list s1 s2 (cdr xs)))]
          [else (cons (car xs)
                      (replace-in-list s1 s2 (cdr xs)))]))

  (define (change xs)
    (cond [(null? xs) null]
          [(eq? xs s1) s2]
          [else xs]))

  (if (list? xs)
      (iter xs)
      (change xs)))

; ----- ----- -----
; TESTY

;(replace-in-list 'x 'y '(x 2 (2 4 z) (y) (x) 5 x))
; --> '(y 2 (2 4 z) (y) (y) 5 y)

;(replace-in-list 'x 'x1 '(2 (55 x) (+ 2 13 x (33 x5 x x1)) 88 11 x))
; --> '(2 (55 x1) (+ 2 13 x1 (33 x5 x1 x1)) 88 11 x1)

; ----- ----- -----


;; the let-lift procedure

; ===== POMYSL: =====
; przechodzimy sie po calej liscie zbierajac
; dane o wiazaniach (letach) i trzymajac
; w pamieci nowe, przemianowane wyrazenie.
; na koncu zamieniamy wynikowa strukture
; na wyrazenie w naszym jezyku z wiazaniami
; na poczatku i przemianowanymi zmiennymi

(define (let-lift e)

  ; ----- ----- -----
  ; struktura, w ktorej przechowujemy:
  ; - liste wiazan zmiennych
  ; - liste przechowujaca wyrazenie bez let'ow
  ; - numer wywolania (do stworzenia swiezej zmiennej)
  (define (answer let-list expr-list nr)
    (list let-list expr-list nr))
  (define (answer-let-list a)
    (car a))
  (define (answer-expr-list a)
    (cadr a))
  (define (answer-nr a)
    (caddr a))

  ; ----- ----- -----
  ; posuwamy sie po wyrazeniu do konca,
  ; rozszerzajac caly czas ans
  (define (lift-in-expr e ans)
         
    (cond

      [(null? e) ans]

      ; ===== ===== ===== ===== =====
      ;      JESLI NIE JEST PARA
      ; ===== ===== ===== ===== =====
      ; --- const/var ---
      [(or (const? e) (var? e))
       (answer
           ; ta sama lista let'ow
           (answer-let-list ans)
           ; do listy wynikowej dolacz na koniec stala/zmienna
           (append (answer-expr-list ans) (list e))
           ; licznik wywolan ++
           (+ 1 (answer-nr ans)))]

      ; --- let ---
      [(let? e)
         (lift-in-let e ans)]

      ; ===== ===== ===== ===== =====
      ;        JESLI JEST PARA
      ; ===== ===== ===== ===== =====
      
       ; --- const/var ---
       [(or (const? (car e)) (var? (car e)))
        (lift-in-expr
           (cdr e)
           (answer
              ; ta sama lista let'ow
              (answer-let-list ans)
              ; do listy wynikowej dolacz na koniec stala/zmienna
              (append (answer-expr-list ans) (list (car e)))
              ; licznik wywolan ++
              (+ 1 (answer-nr ans))))]

       ; --- operator ---
       [(op? (car e))
        (let
            ; zapisujemy sobie, co zwroci wywolanie
            ; na liscie argumentow, ale podajemy puste listy!
            ((new-ans (lift-in-expr
                        (op-args (car e))
                        (answer
                          '()
                          '()
                          (+ 1 (answer-nr ans))))))

            ; i wywolujemy na nastepnym elemencie listy
            ; z nowym answer (laczymy lety i wyrazenia)      
            (lift-in-expr
              (cdr e)
              (answer
                ; laczy to, co mieliscmy z tym, co znalezlismy w srodku
                (append (answer-let-list new-ans)
                        (answer-let-list ans))
                ; teraz mamy nowe wyrazenie:
                (append (answer-expr-list ans)
                        (list (cons (op-op (car e))
                                    (answer-expr-list new-ans))))
                ; i licznik sie zwieksza
                (+ 1 (answer-nr new-ans)))))]

       ; --- let ---
       [(let? (car e))
        ; wywolaj sie na nastepnym majac na uwadze nowopowstaly answer
        ; UWAGA - tutaj nie trzeba zwiekszac licznika, sam sie zwiekszy
        ; w odpowiednim miejscu w procedurze lift-in-let
        (lift-in-expr
           (cdr e)
           (lift-in-let (car e) ans))]))

  ; ----- ----- -----
  ; jesli natrafilismy na leta (let (x e1) e2),
  ; to najpierw wchodzimy do czesci definiujacej.
  ; tam obliczamy wartosc wyrazenia e1.
  ; jesli w e1 sa jakies lety, to dolaczamy let-list z e1
  ; NA POCZATEK let-listy, nastepnie tworzymy liste '(x e1)
  ; i dolaczamy ja NA KONIEC let-listy.
  ; natepnie przeszukujemy wyrazenie e2
  (define (lift-in-let e ans)
    ; bedziemy podmieniac symbol
    (let
        ; sprawdzamy jaki to symbol
        ((def-symbol (let-def-var (let-def e)))
         ; generujemy nowy symbol
         (new-symbol (number->symbol (answer-nr ans))))

      (let*
          ; wywolaj sie w czesci definiujacej
          ; (nie musimy zmieniac licznika ans,
          ; zrobi to procedura lift-in-let-def).
          ; podajemy tez symbol do podmiany w let-liscie
          ((def-ans (lift-in-let-def (let-def e)
                                     ans
                                     new-symbol))
           ; korzystajac z def-ans wywolaj sie
           ; na wyrazeniu z podmienionymi zmiennymi
           (expr-ans (lift-in-expr
                        (replace-in-list def-symbol
                                         new-symbol
                                         (let-expr e))
                        (answer
                          (answer-let-list def-ans)
                          (answer-expr-list def-ans)
                          (+ 1 (answer-nr def-ans))))))
               
        ; jako answer zwroc:
        ; - let-list takie jakie zwrocil expr-ans
        ; - dolacz jako liste do wyniku to, co zwrocil expr-ans
        ; - nowy numer
        (answer
          (answer-let-list expr-ans)
          (append (answer-expr-list ans)
                  (list (answer-expr-list expr-ans)))
          (+ 1 (answer-nr expr-ans))))))

  
   ; ----- ----- -----
   (define (lift-in-let-def e ans new-var)
     ; zapamietaj answer policzony dla wyrazenia w let-defie
     (let ((new-ans (lift-in-expr
                       (let-def-expr e)
                       (answer
                          '()
                          '()
                          (+ 1 (answer-nr ans))))))

       ; zwroc nowy answer
       (answer
          ; jako liste wiazan zwracamy polaczone to, co bylo wiazane
          ; wewnatrz wyrazenia (let-def-expr e) i na koniec tej listy
          ; dolaczamy to, co teraz chcemy dowiazac (z nazwa swiezej zmiennej)
          (append (answer-let-list new-ans)
                  (answer-let-list ans)
                  (list (list new-var (answer-expr-list new-ans))))
          ; jako liste wyrazen zwracamy liste pusta
          '()
          ; licznik ++
          (+ 1 (answer-nr new-ans)))))


  ; ----- ----- -----
  ; procedura zmienia answer
  ; w przetransformowane wyrazenie 
  (define (answer->list ans)
    
    (define (letlist->expression l rest)
      (cond [(null? l) rest]
            [else (let-cons (car l) (letlist->expression (cdr l) rest))]))

    ; czyscimy listy z bezsensownych nawiasow
    (let ((expr (clear-list (answer-expr-list ans)))
          (lets (clear-list (answer-let-list ans))))

           (letlist->expression lets expr)))


  ;; --- wynik ---
  (let ((res (lift-in-expr e (answer '() '() 0))))
    (clear-list (answer->list res))))
    ; ^^^----- to jest rozwiazanie problemu, o ktorym napisalem
    ;          przy definicji procedury clear-list. czasem expr
    ;          moze byc rowne ((((...lista...)))) i wtedy
    ;          clear-list zwraca ((...lista...)). jesli dolaczymy
    ;          to [expr] na koniec listy wynikowej (procedura
    ;          answer->list), to clear-list poprawnie wyczysci liste


; ----- ----- -----
; TESTY

; -----
; testy, czy struktura sie zgadza (lety
; i wyrazenia zagniezdzone na rozne sposoby)


;(let-lift '(+ 10 (* ( let (x 7) (+ x 2)) 2)))
; --> '(let (x3 7) (+ 10 (* (+ x3 2) 2)))

;(let-lift '(+ 10 (+ 6 (let (x 3) (+ 6 55)))))
; --> '(let (x4 3) (+ 10 (+ 6 (+ 6 55))))

;(let-lift '(+ 10 (+ 6 (* 2 3 (- 1 5)) (+ 6 55))))
; --> '(+ 10 (+ 6 (* 2 3 (- 1 5)) (+ 6 55)))

;(let-lift '(let (x 3) (+ (let (y 5) 2) (let (z 33) 1))))
; --> '(let (x0 3) (let (x5 5) (let (x11 33) (+ 2 1))))

;(let-lift '((let (x (+ 2 3)) (let (y 2) (+ 1 5)))))
; --> '(let (x0 (+ 2 3)) (let (x6 2) (+ 1 5)))

;(let-lift '((let (x (let (y 3) 5)) 88)))
; --> '(let (x1 3) (let (x0 5) 88))

;(let-lift '(+ x 2 (let (x 5) 6)))
; --> '(let (x3 5) (+ x 2 6))

;(let-lift '(+ 10 (let (x (let (y 12) 3)) 2) (let (z 5) 55)))
; --> '(let (x3 12) (let (x2 3) (let (x13 5) (+ 10 2 55))))

;(let-lift '(+ a 5 (let (x (let (y 2) (+ 33 2 (* 1 5)))) (let (z 5) 13)) 5 (let (t 6) 8)))
; --> '(let (x4 2) (let (x3 (+ 33 2 (* 1 5))) (let (x18 5) (let (x26 6) (+ a 5 13 5 8)))))

;(let-lift '(+ a 5 (let (x (let (y 2) 6)) (+ 2 (let (z 15) 6))) (let (t 7) 88) (* 2 3)))
; --> '(let (x4 2) (let (x3 6) (let (x14 15) (let (x21 7) (+ a 5 (+ 2 6) 88 (* 2 3))))))

;(let-lift '(+ 10 (* ( let (x 7) (+ x 2)) 2)))
; --> '(let (x3 7) (+ 10 (* (+ x3 2) 2)))


; -----
; testy, czy przemianowanie zmiennych
; dziala jak nalezy (rozne wyrazenia)


;(let-lift '( let (x (- 2 ( let (z 3) z))) (+ x 2)))
; --> '(let (x3 3) (let (x0 (- 2 x3)) (+ x0 2)))

;(let-lift '(+ 16 (let (x (+ x 3)) (* x 12 3))))
; --> (let (x2 (+ x 3)) (+ 16 (* x2 12 3)))

; (let-lift '(+ x 3 (let (x 3) (* x y 5))))
; --> '(let (x3 3) (+ x 3 (* x3 y 5)))

;(let-lift '(+ ( let (x 5) x) ( let (x 1) x)))
; --> '(let (x1 5) (let (x7 1) (+ x1 x7)))

;(let-lift '(let (x (let (y 2) (- 2 y))) (* (let (z x) (+ z 6)) x)))
; --> '(let (x1 2) (let (x0 (- 2 x1)) (let (x12 x0) (* (+ x12 6) x0))))

;(let-lift '(let (x (let (x 2) (- 2 x))) (* (let (x x) (+ x 6)) x)))
; --> '(let (x1 2) (let (x0 (- 2 x1)) (let (x12 x0) (* (+ x12 6) x0))))




; TEST SEMANTYKI
; wyrazenie E  i (let-lift E) powinny byc
; rownowazne czyli powinny zwracac ten sam wynik

; ===== KOD Z WYKLADU (uszczuplony) =====
(define (env-for-let def env)
  (add-to-env
    (let-def-var def)
    (eval-env (let-def-expr def) env)
    env))

(define (eval-env e env)
  (cond [(const? e) e]
        [(op? e)
         (apply (op->proc (op-op e))
                (map (lambda (a) (eval-env a env))
                     (op-args e)))]
        [(let? e)
         (eval-env (let-expr e)
                   (env-for-let (let-def e) env))]
        [(var? e) (find-in-env (var-var e) env)]))

; ----- ----- -----
(define (equal-expressions? e1 e2)
  (= (eval-env e1 '()) (eval-env e2 '())))

; ----- TEST -----
(define (test)
  (define test-list
    (list
     '(+ 13 2 (* 1 6) (- 55 3) (/ 2 22))
     '(+ 3 (let (x 3) (+ 6 6 x)) (* 1 (- 0 1)))
     '(let (x (let (x 2) (- 2 x))) (* (let (x x) (+ x 6)) x))
     '(let (x 3) (+ 8 x (let (x 8) (+ 2 x))))
     '(let (x (let (x 5) (let (x 6) 7))) (+ x 22))
     '(let
        (x (let (y (+ 1 (let (z (let (t 5) 6)) z))) y))
        (+ 13 15 (* 1 1 1 2 (let (x 2) (+ x (let (y 2) (- y 100)))))))
     '(+ 3 (let (x 2) (* 5 (let (y 4) (+ y x)))) 6)
     '(let (x 5) (+ (let (y 2) (let (x 3) (* 2 y x)))))))
  
  (andmap (lambda (x) (equal-expressions? x (let-lift x))) test-list))

(test)
