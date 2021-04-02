#lang racket

(require "calc.rkt")

(define (def-name p)
  (car p))

(define (def-prods p)
  (cdr p))

(define (rule-name r)
  (car r))

(define (rule-body r)
  (cdr r))

(define (lookup-def g nt)
  (cond [(null? g) (error "unknown non-terminal" g)]
        [(eq? (def-name (car g)) nt) (def-prods (car g))]
        [else (lookup-def (cdr g) nt)]))

(define parse-error 'PARSEERROR)

(define (parse-error? r) (eq? r 'PARSEERROR))

(define (res v r)
  (cons v r))

(define (res-val r)
  (car r))

(define (res-input r)
  (cdr r))

;;

(define (token? e)
  (and (list? e)
       (> (length e) 0)
       (eq? (car e) 'token)))

(define (token-args e)
  (cdr e))

(define (nt? e)
  (symbol? e))

;;

(define (parse g e i)
  (cond [(token? e) (match-token (token-args e) i)]
        [(nt? e) (parse-nt g (lookup-def g e) i)]))

(define (parse-nt g ps i)
  (if (null? ps)
      parse-error
      (let ([r (parse-many g (rule-body (car ps)) i)])
        (if (parse-error? r)
            (parse-nt g (cdr ps) i)
            (res (cons (rule-name (car ps)) (res-val r))
                 (res-input r))))))

(define (parse-many g es i)
  (if (null? es)
      (res null i)
      (let ([r (parse g (car es) i)])
        (if (parse-error? r)
            parse-error
            (let ([rs (parse-many g (cdr es) (res-input r))])
              (if (parse-error? rs)
                  parse-error
                  (res (cons (res-val r) (res-val rs))
                       (res-input rs))))))))

(define (match-token xs i)
  (if (and (not (null? i))
           (member (car i) xs))
      (res (car i) (cdr i))
      parse-error))

;;

(define num-grammar
  '([digit {DIG (token #\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9)}]
    [numb {MANY digit numb}
          {SINGLE digit}]))

(define (node-name t)
  (car t))

(define (c->int c)
  (- (char->integer c) (char->integer #\0)))

(define (walk-tree-acc t acc)
  (cond [(eq? (node-name t) 'MANY)
         (walk-tree-acc
          (third t)
          (+ (* 10 acc) (c->int (second (second t)))))]
        [(eq? (node-name t) 'SINGLE)
         (+ (* 10 acc) (c->int (second (second t))))]))

(define (walk-tree t)
  (walk-tree-acc t 0))

;;

; DODANE
(define (c->s c) ; char->symbol
  (string->symbol (string c)))
; ----- -----

; tej procedurze podajemy:
; t - drzewo do przejscia
; act-op - operator poprzedniego t
; acc - dotychczas zbudowane wyrazenie
; bedziemy przechodzic po drzewie i budowac wyrazenie
; podajac przy tym operator do rozwazenia ( przy laczeniu )
; oraz dotychczas zbudowane wyrazenie
;
;   op1
;  /   \
; 2    op2    -->  (op2 (op1 2 3) 4)
;     /   \
;    3     4
;
; czyli obliczamy wartosc 2, i przy obliczeniu prawego poddrzewa
; przeslemy do rozwazenia op1 oraz dotychczas zbudowane wyrazenie.
; op2 obliczy 3 i jako 'dotychczasowe wyrazenie' zapamieta: (op1 2 3),
; a na prawym poddrzewie wywola sie z operatorem op2 i (op1 2 3)
; UWAGA: trzeba pamietac, ze czasem op2 moze miec wyzszy priorytet niz op1,
;        wtedy troszke inaczej budujemy wyrazenie

(define (ar-walk-t t act-op acc)
  (cond

    [(eq? (node-name t) 'BASE-NUM)
     (let ((done (walk-tree (second t))))
       ; jesli nie bylo niczego do dolaczenia, to zwracamy to wyrazenie
       ; a jesli przyszlismy tutaj z jakims wyrazeniem, tzn. ze musimy
       ; je polaczyc operatorem, z ktorym tutaj przyszlismy
       (if (null? acc)
         done
         (binop-cons (c->s act-op) acc done)))]
    
    [(eq? (node-name t) 'PARENS)
     ; policz wyrazenie wewnatrz nawiasow,
     ; jesli jest do czego dolaczyc to dolacz
     ; z operatorem z ktorym tu jestes
     (let ((done (ar-walk-t (third t) act-op '())))
       (if (null? acc)
           done
           (binop-cons (c->s act-op) acc done)))]

    [(eq? (node-name t) 'ADD-MANY)
     ; policz wyrazenie po lewej (czyli z pustym akumulatorem)
     (let ((ldone (ar-walk-t (second t) act-op '())))
       ; i teraz musimy przeslac wynik:
       (ar-walk-t (fourth t) ; <-- przejdz po prawym drzewie
                  (third t)  ; <-- jako operator podaj obecnie przerabiany
                  ; jako acc = analogicznie, jak w BASE-NUM
                  (if (null? acc)
                    ldone
                    (binop-cons (c->s act-op) acc ldone))))]

    [(eq? (node-name t) 'MULT-MANY)
     ; musimy pamietac, ze mnozenie i dzielenie ma wyzszy
     ; priorytet niz dodawanie, dlatego rozwazamy dwa przypadki:
     ; 1: przyszlismy tutaj z act-op * lub /
     ; --> wtedy normalnie przesylamy acc
     ; 2: przyszlismy z + lub -,
     ; --> wtedy odp jest utworzone wyrazenie (ACT-OP acc (walktree right))
     (let ((ldone (ar-walk-t (second t) act-op '())))
       (if (or (eq? act-op #\*) (eq? act-op #\/))
           (ar-walk-t (fourth t)
                      (third t)
                      (if (null? acc)
                          ldone
                          (binop-cons (c->s act-op) acc ldone)))
           (let ((rdone (ar-walk-t (fourth t) (third t) ldone)))
             (if (null? acc)
                 rdone
                 (binop-cons (c->s act-op) acc rdone)))))]
    
    [(or (eq? (node-name t) 'ADD-SINGLE)
         (eq? (node-name t) 'MULT-SINGLE))
     ; zejdz po prostu w glab drzewa
      (ar-walk-t (second t) act-op acc)]
    [else (error "nothing found:" (node-name t))]))



; add-expr to wyrazenie z + lub -, ma mniejszy priorytet
; mult-expr to wyrazenie z * lub /, ma wiekszy priorytet
(define arith-grammar
  (append num-grammar
     '([add-expr {ADD-MANY   mult-expr (token #\+ #\-) add-expr} ;DODANE
                 {ADD-SINGLE mult-expr}]
       [mult-expr {MULT-MANY base-expr (token #\* #\/) mult-expr} ;DODANE
                  {MULT-SINGLE base-expr}]
       [base-expr {BASE-NUM numb}
                  {PARENS (token #\() add-expr (token #\))}])))

(define (arith-walk-tree t)
  (ar-walk-t t #\+ '())) ; DODANE (drzewo rozkladu zaczyna sie od add-many)

(define (calc s)
 (eval
  (arith-walk-tree
   (car
    (parse
       arith-grammar
       'add-expr
       (string->list s))))))

; DODANE
; procedura do spr przerobionych wyrazen
(define (test exp)
  (arith-walk-tree
   (car
    (parse
       arith-grammar
       'add-expr
       (string->list exp)))))

#|
; TESTY
(test "123")
; 123
(test "123+11")
; '(+ 123 11)
(test "123+11+666")
; '(+ (+ 123 11) 666)
(test "123+11-666")
; '(- (+ 123 11) 666)
(test "123-11-666")
; '(- (- 123 11) 666)
(test "5/6/7/8")
; '(/ (/ (/ 5 6) 7) 8)
(test "3*6+(11-2)")
; '(+ (* 3 6) (- 11 2))
(test "3*6+(11*2)")
; '(+ (* 3 6) (* 11 2))
(test "(2-3)-44")
; '(- (- 2 3) 44)
(test "(2-(3-44))")
; '(- 2 (- 3 44))
(test "2*(3-44)/6")
; '(/ (* 2 (- 3 44)) 6) 
(test "3*1-2*3-5-6")
; '(- (- (- (* 3 1) (* 2 3)) 5) 6)
|#