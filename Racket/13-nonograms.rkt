#lang racket

(require racklog)

;; transpozycja tablicy zakodowanej jako lista list
(define (transpose xss)
  (cond [(null? xss) xss]
        ((null? (car xss)) (transpose (cdr xss)))
        [else (cons (map car xss)
                    (transpose (map cdr xss)))]))

;; procedura pomocnicza
;; tworzy listę n-elementową zawierającą wyniki n-krotnego
;; wywołania procedury f
(define (repeat-fn n f)
  (if (eq? 0 n) null
      (cons (f) (repeat-fn (- n 1) f))))

;; tworzy tablicę n na m elementów, zawierającą świeże
;; zmienne logiczne
(define (make-rect n m)
  (repeat-fn m (lambda () (repeat-fn n _))))


;; ===== DODANE =====
;; predykat binarny
;; (%row-ok xs ys) oznacza, że xs opisuje wiersz (lub kolumnę) ys

; k (pierwszy element listy opisujacej wiersz) traktujemy
; jako etykiete opisujaca ile pozostalo '* do przeczytania
; k<0 ma sens - to oznacza ze k znakow temu byla ostatnia gwiazdka

; chcemy, zeby insider zaczynal przeszukiwania z k = -1 na poczatku
; bo bedziemy umieli prawidlowo obsluzyc '_ oraz '* na poczatku wiersza
(define %row-ok-insider 
  (%rel (xs ys k n)
        
        ; gdy wyczerpia sie obie listy i k nie jest dodatnie
        ; (czyli nie przewidujemy juz zadnych '*) to jest ok
        [((cons k null) null) 
         (%<= k 0)]           

        ; jesli natrafimy na gwiazdke i k jest ujemne to przechodzimy
        ; do nastepnej liczby z listy i obslugujemy ta dwiazdke
        ; (jesli k = 0 to mamy sytuacje np: (%row-ok '(2 1) '(_ * * * _)
        ; ktora jest oczywiscie zla
        [((cons k xs) (cons '* ys)) 
         (%< k 0)                  
         (%row-ok-insider xs (cons '* ys))]

        ; jesli mamy gwiazdke i k>0 to zmniejszamy k o jeden
        ; i obslugujemy reszte listy znakow
        [((cons k xs) (cons '* ys))
         (%> k 0)
         (%is n (- k 1))
         (%row-ok-insider (cons n xs) ys)]

        ; jesli k<=0 i natrafilismy na '_ to zmniejszamy k
        ; i obslugujemy reszte listy
        [((cons k xs) (cons '_ ys))
         (%<= k 0)
         (%is n (- k 1))
         (%row-ok-insider (cons n xs) ys)]

        ))

; niektore obrazki logiczne opisuja pusty wiersz jako 0, a inne jako nic,
; dlatego chcemy obsluzyc zarowno: (%row-ok '(0) '(_ _)) jak (%row-ok '() '(_ _))
(define %row-ok
  (%rel (xs ys)
        [((cons 0 xs) ys)
         (%row-ok-insider (cons -1 xs) ys)]
        [(xs ys)
         (%row-ok-insider (cons -1 xs) ys)] ; zaczynamy z -1 na poczatku
        ))

; dostaje tablice i sprawdza, czy wszystkie
; wiersze spelniaja predykat %row-ok
; uzycie: (%all-ok board rows'-numbers)
(define %all-ok
  (%rel (xs ys xss yss)
        [(null null)]
        [((cons xs xss) (cons ys yss))
         (%row-ok xs ys)
         (%all-ok xss yss)]))
  
;; funkcja rozwiązująca zagadkę
(define (solve rows cols)
  (define board (make-rect (length cols) (length rows)))
  (define tboard (transpose board))
  (define ret (%which (xss yss) 
                      (%= xss board)
                      (%= yss tboard)
                      (%all-ok rows xss)   ; wszystkie wiersze okej
                      (%all-ok cols yss))) ; wszystkie kolumny okej
                 
  (and ret (cdar ret)))

;; testy
(equal? (solve '((2) (1) (1)) '((1 1) (2)))
        '((* *)
          (_ *)
          (* _)))

(equal? (solve '((2) (2 1) (1 1) (2)) '((2) (2 1) (1 1) (2)))
        '((_ * * _)
          (* * _ *)
          (* _ _ *)
          (_ * * _)))


; TEST INICJALY
;B
(equal? (solve '((3) (1 1) (4) (1 1) (4)) '((5) (1 1 1) (3 1) (3)))
        '((* * * _)
          (* _ * _)
          (* * * *)
          (* _ _ *)
          (* * * *)))
;G
(equal? (solve '((4) (1) (1 2) (1 1) (4)) '((5) (1 1) (1 1 1) (1 3)))
        '((* * * *)
          (* _ _ _)
          (* _ * *)
          (* _ _ *)
          (* * * *)))

; TEST 3
(equal? (solve '((2) (1 1) (1 2) (1 1) (0)) '((4) (1) (0) (3) (1)))
        '((* * _ _ _)
          (* _ _ * _)
          (* _ _ * *)
          (* _ _ * _)
          (_ _ _ _ _)))


; TEST jakies przyklady z internetu
(equal? (solve '((2) (3 1) (4) (4) (1 1)) '((1) (5) (4) (2) (4)))
        '((_ * * _ _)
          (* * * _ *)
          (_ * * * *)
          (_ * * * *)
          (_ * _ _ *)))

(equal? (solve '((3) (5) (1 1) (1 1) (2 2)) '((1 1) (5) (2) (5) (1 1)))
        '((_ * * * _)
          (* * * * *)
          (_ * _ * _)
          (_ * _ * _)
          (* * _ * *)))