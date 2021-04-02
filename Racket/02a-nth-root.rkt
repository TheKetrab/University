#lang racket

;; Bartlomiej Grochowski
;; nr 300 951
;; Li2 zadanie 11

(define (dist x y) (abs (- x y)))
(define (square x) (* x x))
(define (cube x) (* x x x))

(define (close-enough? x y)
  (< (dist x y) 0.000001))

;; zad 3
(define (compose f g)
  (lambda (x)
    (f (g x))))

(define (repeated p n )
    (if (= n 0)
        identity
        (compose p (repeated p (- n 1)))))

;; z wykladu
(define (fix-point f x0)
  (let ((x1 (f x0)))
    (if (close-enough? x0 x1)
        x0
        (fix-point f x1))))

(define (average-damp f)
  (lambda (x) (/ (+ x (f x)) 2)))


;; --- --- ---

(define (rt2-ad x)
  (fix-point ((repeated average-damp 1)
              (lambda (y) (/ x (expt y 1)))) 1.0))
(define (rt3-ad x)
  (fix-point ((repeated average-damp 1)
              (lambda (y) (/ x (expt y 2)))) 1.0))
(define (rt4-ad x)
  (fix-point ((repeated average-damp 2)
              (lambda (y) (/ x (expt y 3)))) 1.0))
(define (rt5-ad x)
  (fix-point ((repeated average-damp 2)
              (lambda (y) (/ x (expt y 4)))) 1.0))
(define (rt6-ad x)
  (fix-point ((repeated average-damp 2)
              (lambda (y) (/ x (expt y 5)))) 1.0))
(define (rt7-ad x)
  (fix-point ((repeated average-damp 2)
              (lambda (y) (/ x (expt y 6)))) 1.0))
(define (rt8-ad x)
  (fix-point ((repeated average-damp 3)
              (lambda (y) (/ x (expt y 7)))) 1.0))
(define (rt9-ad x)
  (fix-point ((repeated average-damp 3)
              (lambda (y) (/ x (expt y 8)))) 1.0))
(define (rt10-ad x)
  (fix-point ((repeated average-damp 3)
              (lambda (y) (/ x (expt y 9)))) 1.0))
(define (rt16-ad x)
  (fix-point ((repeated average-damp 4)
              (lambda (y) (/ x (expt y 15)))) 1.0))
(define (rt32-ad x)
  (fix-point ((repeated average-damp 5)
              (lambda (y) (/ x (expt y 31)))) 1.0))

;; --- --- --- --- --- --- --- --- --- ---
;; testy, ile razy trzeba skladac funkcje
;; average-damp, zeby bylo zbiezne.
;; (na podstawie tych testow dobralem X
;; w repeated avg-damp X dla powyzszych funkcji)
;; --- --- --- --- --- --- --- --- --- ---
;;(rt2-ad 16)
;;(rt2-ad 1)
;;(rt2-ad 36)
;;(rt3-ad 64)
;;(rt3-ad 27)
;;(rt4-ad 16)
;;(rt4-ad 81)
;;(rt5-ad 32)
;;(rt6-ad 546)
;;(rt7-ad 74)
;;(rt8-ad 764)
;;(rt9-ad 54)
;;(rt10-ad 65)
;;(rt16-ad 623)
;;(rt32-ad 11)
;; otrzymalem ze [log2(x)]

;; --- pierwiastek n-tego stopnia ---
(define (log2 x)
  (/ (log x) (log 2)))

(define (nth-root x n)
  (if (= n 0) 1
      (fix-point ((repeated average-damp [floor (log2 n)])
              (lambda (y) (/ x (expt y (- n 1))))) 1.0)))

;; --- --- --- --- ---
;; TESTY
;; --- --- --- --- ---
;;
;; (nth-root 10 2)   -> 3.162277665175675
;; kalkulator   -> 3.162277660168379 , ok
;;
;; (nth-root 277 6)  -> 2.553171004490982
;; kalkulator   -> 2.553171344656243 , ok
;;
;; (nth-root 26 11)  -> 1.3447259916704395
;; kalkulator   -> 1.3447264296899535 , ok
;;
;; (nth-root 100 66) -> 1.072266933373375
;; kalkulator   -> 1.072267222010323 , ok
;;
;; (nth-root 33.75 42) -> 1.0873956055503275
;; kalkulator     -> 1.0873953484408290 , ok
;;
;; (nth-root -22 33) -> -1.0981952958458203
;; kalkulator   -> -1,0981950331485254 , ok
;;
;; (nth-root 55555 0) -> 1
;; kalkulator    -> 1