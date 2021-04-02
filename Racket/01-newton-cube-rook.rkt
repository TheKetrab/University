#lang racket

;; Bartlomiej Grochowski
;; oparte na algorytmie z wykladu

(define (dist x y)
  (abs (- x y)))

(define (cube x)
  (* x x x))

(define (cbrt x)
  ;; poprawa przyblizenia Newtona
  (define (average y)
    (/ (+ (/ x (* y y))
          (* 2 y))
       3))
  (define (improve approx)
    (average approx))
  ;; spr czy koniec
  (define (good-enough? approx)
    (< (dist x (cube approx)) 0.0001))
  ;; glowna procedura
  (define (iter approx)
    (if (good-enough? approx) approx
        (iter (improve approx))))
  
  (iter 1.0))

;; -----
;; TESTY
;; -----
;;
;; porownanie wynikow programu i kalkulatora
;;
;; 0         -> 0.03901844231062338
;; kalk      -> 0.00000000000000000
;; --blad rzedu 10^(-2)
;;
;; -1        -> -1.000000001794607
;; kalk      -> -1.000000000000000
;; --blad rzedu 10^(-9)
;;
;; 2         -> 1.259933493449977
;; kalk      -> 1.259921049894873
;; --blad rzedu 10^(-5)
;;
;; 10        ->  2.154434691772293
;; kalk      ->  2.154434690031883
;; --blad rzedu 10^(-9)
;;
;; -10       -> -2.154441111003160 // blad do liczby przeciwnej z poprzedniego testu rzedu 10^(-5)
;; kalk      -> -2,154434690031883 // liczba przeciwna do 10^(1/3)
;; --blad rzedu 10^(-5)
;;
;; 27        -> 3.0000005410641766
;; kalk      -> 3.0000000000000000
;; --blad rzedu 10^(-7)
;;
;; 0.000000000005 (5*10^(-12))  -> 0.039018443001993704
;; kalk -> 0,00017099759466767
;; --blad rzedu 10^(-2)
;;
;; 0.023     -> 0.28474762814675436
;; kalk      -> 0.28438669798515655
;; --blad rzedu 10^(-4)
;;
;; 123 456 789  -> 497.93385921822545
;; kalk         -> 497.93385921817447
;; --blad rzedu 10^(-10)
;;
;; 1 498 722 156 665  -> 11443.890912118894
;; kalk               -> 11443.890912118894
;; --blad rzedu < 10^(-12)