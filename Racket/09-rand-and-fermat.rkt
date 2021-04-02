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




;; DODANE
; z podrecznika
(define (smallest-divisor n)
  (find-divisor n 2))
(define (find-divisor n test-divisor)
  (cond [(> (square test-divisor) n) n]
        [(divides? test-divisor n) test-divisor]
        [else (find-divisor n (+ test-divisor 1))]))
(define (divides? a b)
  (= (remainder b a) 0))
(define (prime? n)
  (= n (smallest-divisor n)))

;expmod
(define (expmod b e m)
  (cond ((zero? e) 1)
        ((even? e)
         (remainder (square (expmod b (/ e 2) m))
                    m))
        (else
         (remainder (* b (expmod b (- e 1) m))
                    m))))

(define (square x) (* x x))
; ----- ----- -----





;;
;; WHILE
;;

; memory

(define empty-mem
  null)

(define (set-mem x v m)
  (cond [(null? m)
         (list (cons x v))]
        [(eq? x (caar m))
         (cons (cons x v) (cdr m))]
        [else
         (cons (car m) (set-mem x v (cdr m)))]))

(define (get-mem x m)
  (cond [(null? m) 0]
        [(eq? x (caar m)) (cdar m)]
        [else (get-mem x (cdr m))]))

; arith and bool expressions: syntax and semantics

(define (const? t)
  (number? t))

(define (true? t)
  (eq? t 'true))

(define (false? t)
  (eq? t 'false))

; DODANE - expmod
(define (op? t)
  (and (list? t)
       (member (car t) '(+ - * / = > >= < <= expmod not and or mod))))

(define (op-op e)
  (car e))

(define (op-args e)
  (cdr e))

(define (op->proc op)
  (cond [(eq? op '+) +]
        [(eq? op '*) *]
        [(eq? op '-) -]
        [(eq? op '/) /]
        [(eq? op '=) =]
        [(eq? op '>) >]
        [(eq? op '>=) >=]
        [(eq? op '<)  <]
        [(eq? op '<=) <=]
        [(eq? op 'expmod) expmod] ; DODANE
        [(eq? op 'not) not]
        [(eq? op 'and) (lambda x (andmap identity x))]
        [(eq? op 'or) (lambda x (ormap identity x))]
        [(eq? op 'mod) modulo]
        [(eq? op 'rand) (lambda (max) (min max 4))])) ; chosen by fair dice roll.
                                                      ; guaranteed to be random.

(define (var? t)
  (symbol? t))

(define (eval-arith e m)
  (cond [(true? e) true]
        [(false? e) false]
        [(var? e) (get-mem e m)]
        
        [(op? e)
         (apply
          (op->proc (op-op e))
          (map (lambda (x) (eval-arith x m))
               (op-args e)))]
        [(const? e) e]))

;; syntax of commands

(define (assign? t)
  (and (list? t)
       (= (length t) 3)
       (eq? (second t) ':=)))

(define (assign-var e)
  (first e))

(define (assign-expr e)
  (third e))

(define (if? t)
  (tagged-tuple? 'if 4 t))

(define (if-cond e)
  (second e))

(define (if-then e)
  (third e))

(define (if-else e)
  (fourth e))

(define (while? t)
  (tagged-tuple? 'while 3 t))

(define (while-cond t)
  (second t))

(define (while-expr t)
  (third t))

(define (block? t)
  (list? t))


;; state

(define (res v s)
  (cons v s))

(define (res-val r)
  (car r))

(define (res-state r)
  (cdr r))

;; psedo-random generator

(define initial-seed
  123456789)

(define (rand max)
  (lambda (i)
    (let ([v (modulo (+ (* 1103515245 i) 12345) (expt 2 32))])
      (res (modulo v max) v))))

;; WHILE interpreter

(define (old-eval e m)
  (cond [(assign? e)
         (set-mem
          (assign-var e)
          (eval-arith (assign-expr e) m)
          m)]
        [(if? e)
         (if (eval-arith (if-cond e) m)
             (old-eval (if-then e) m)
             (old-eval (if-else e) m))]
        [(while? e)
         (if (eval-arith (while-cond e) m)
             (old-eval e (old-eval (while-expr e) m))
             m)]
        [(block? e)
         (if (null? e)
             m
             (old-eval (cdr e) (old-eval (car e) m)))]))

; ===== ===== =====
 ; DODANE

; eval zwraca nam stan w postaci pary: (memory . seed)
; val - mem, state - seed

(define (rand? e)
  (and (list? e) (eq? (car e) 'rand)))


; eval-arith-st zwraca pare (v . seed)
(define (eval-arith-st e m seed)
  (cond
    ; jesli jest t/f to zwroc pare z tym samym ziarnem
    [(true? e) (res true seed)]
    [(false? e) (res false seed)]
    [(rand? e)
     ; jesli jest randem, to oblicz wyrazenie a nastepnie wykonaj
     ; procedure rand z wartoscia wyrazenia zaaplikowana do nowego ziarna
     ; --> ((rand max) n) zwraca pare - (value . seed)
     (let ((done (eval-arith-st (cadr e) m seed)))
       ((rand (res-val done)) (res-state done)))]
    [(var? e) (res (get-mem e m) seed)]
    [(op? e)
     (res (apply
           (op->proc (op-op e))
           (map (lambda (x) (res-val (eval-arith-st x m seed)))
                (op-args e)))
          seed)]
    [(const? e) (res e seed)]))

; eval arith zwraca wartosc i nowe ziarno



; zwraca pamiec i ziarno
(define (eval e m seed)

  (cond [(assign? e)
         (let ((done (eval-arith-st (assign-expr e) m seed)))
           (res (set-mem
                 (assign-var e)
                 (res-val done)
                 m)
                (res-state done)))]
        [(if? e)
         (let ((done (eval-arith-st (if-cond e) m seed)))
           (if (res-val done)
               (eval (if-then e) m (res-state done))
               (eval (if-else e) m (res-state done))))]
        [(while? e)
         (let ((done (eval-arith-st (while-cond e) m seed))) 
           (if (res-val done)
               (let ((done-expr (eval (while-expr e) m (res-state done)))) 
               (eval e (res-val done-expr) (res-state done-expr)))
               (res m (res-state done))))]
        [(block? e)
         (if (null? e)
             (res m seed)
             (let ((done-car (eval (car e) m seed)))
               (eval (cdr e) (res-val done-car) (res-state done-car))))]))


(define (run e)
  (eval e empty-mem initial-seed))

;;


; ===== ===== =====
; Rozszerzylem jezyk WHILE o operacje
; arytmetyczna expmod, ktora jest oparta
; na algorytmie zamieszczonym w podreczniku
; w rozdziale pierwszym.
(define fermat-test `{
   
   (composite := false)
   (while (> k 0)
    {
       (a := (+ 2 (rand (- n 4))))
       (if (= 1 (expmod a (- n 1) n))
           (k := (- k 1))         ; then
           [ (k := 0)             ; else
             (composite := true) ]) ;
     })
})


(define (probably-prime? n k) ; check if a number n is prime using
                              ; k iterations of Fermat's primality
                              ; test
  (let ([memory (set-mem 'k k
                (set-mem 'n n empty-mem))])
    (not (get-mem
           'composite
           (res-val (eval fermat-test memory initial-seed))))))


#|
; testy
; --> porownywane ze spisem liczb pierwszych na Wikipedii
; https://pl.wikisource.org/wiki/Liczby_pierwsze
(probably-prime? 43 15)
; #t
(probably-prime? 9623 15)
; #t
(probably-prime? 9623 1500)
; #t
(probably-prime? 9620 1500)
; #f

; losowe liczby zlozone
(probably-prime? 34523948129 1500)
; #f (dzieli sie przez trzy)
(probably-prime? 18476155 20000)
; #f (dzieli sie przez 11)
(probably-prime? 251179447 20000)
;#f (iloczyn dwoch liczb pierwszych: 16339 * 15373)
|#


; ===== TESTY LOSOWE =====
(define (make-and-display-test x k)
  (display x)
  (display " ")
  (display "[prime? ")
  (let ((p1 (prime? x)))
    (display p1)
    (display "] ")
    (display "[probably-prime? ")
    (let ((p2 (probably-prime? x k)))
      (display p2)
      (display "] ===>> ")
      (if (eq? p1 p2)
          (display "SUCCESS\n")
          (display "FAILED!!!\n")))))

; lista losowych liczb
(define (rand-list len max)
  (if (= len 0)
      null
      (cons (random max) (rand-list (- len 1) max))))

; wyswietli automatycznie porownanie
; procedur prime i probably-prime?
(define multi-test
  (let ((l (rand-list 100 1000000000))
        (k 1000))
    (map (lambda (x) (make-and-display-test x k)) l)))