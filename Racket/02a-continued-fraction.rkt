#lang racket

;; --- --- --- --- --- ---
;; Bartlomiej Grochowski
;; nr 300 951
;; Li2 zadanie 10
;; --- --- --- --- --- ---

(define (inc x) (+ x 1))
(define (square x) (* x x))

;; -- -- --
;; cw 10
;; -- -- --

(define (chain term-n term-d)

  ;; -- An --
  (define (A n)
    (let ((a-1 1) (a0 0))
    (define (iter i an an-1)
      (if (= i n)
          an
          (iter (inc i)
                (+ (* (term-d (inc i)) an) (* (term-n (inc i)) an-1))
                an)))
    (iter 1
          (+ (* (term-d 1) a0) (* (term-n 1) a-1))
          0)))

  ;; -- Bn --
  (define (B n)
    (let ((b-1 0) (b0 1))
    (define (iter i bn bn-1)
      (if (= i n)
          bn
          (iter (inc i)
                (+ (* (term-d (inc i)) bn) (* (term-n (inc i)) bn-1))
                bn)))
    (iter 1
          (+ (* (term-d 1) b0) (* (term-n 1) b-1))
          1)))

  ;; --- --- ---
  (define (function-ab k)
    [/ (A k) (B k)])

  (define (good-enough? f k)
    (let ((mistake 0.00001))
    (< ( abs ( - (f (+ k 1)) (f k))) mistake))) 

  (define (improve x)
    (if (good-enough? function-ab x)
        (function-ab x)
        (improve (+ x 1))))
  ;; --- --- ---

  (improve 1))

;; --- --- --- --- --- ---
;; testy - cw 10
;;
;; FI - zloty podzial
;; [chain (lambda (x) 1.0) (lambda (x) 1.0)]
;; wynik: 0.6180371352785146 , ok
;;
;; pierwiastek z 3
;; [+ 1 (chain (lambda (x) 2.0) (lambda (x) 2.0))]
;; wynik: 1.7320574162679425 , ok
;;
;; PI
;; [+ 3 (chain (lambda (x) (square (- (* 2 x) 1))) (lambda (x) 6.0))]
;; wynik: 3.1415877225444917 , ok
;;
;; stala Eulera e
;; [+ 2 (chain (lambda (x) [if (= x 1) 1.0 (- x 1)]) identity)]
;; wynik: 2.71828369389345
;; 

(define (atg-chain x)
  [chain (lambda (i) [if (= i 1.0) x [square(* (- i 1.0) x)]]) (lambda (i) (- (* 2.0 i) 1.0))])

;; (atan 2)        ->  1.1071487177940904
;; (atg-chain 2)   ->  1.1071558118862657
;; ok
;;
;; (atan -1)       -> -0.7853981633974483 (-PI/4)
;; (atg-chain -1)  -> -0.7854037267080746 (-PI/4)
;; ok
;;
;; (atan 0)        -> 0
;; (atg-chain 0)   -> 0
;;
;; (atan 10)       ->  1.4711276743037347
;; (atg-chain 10)  ->  1.47112229837035
;; ok