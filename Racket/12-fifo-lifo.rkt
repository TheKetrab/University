#lang racket

;; sygnatura: grafy
(define-signature graph^
  ((contracted
    [graph        (-> list? (listof edge?) graph?)]
    [graph?       (-> any/c boolean?)]
    [graph-nodes  (-> graph? list?)]
    [graph-edges  (-> graph? (listof edge?))]
    [edge         (-> any/c any/c edge?)]
    [edge?        (-> any/c boolean?)]
    [edge-start   (-> edge? any/c)]
    [edge-end     (-> edge? any/c)]
    [has-node?    (-> graph? any/c boolean?)]
    [outnodes     (-> graph? any/c list?)]
    [remove-node  (-> graph? any/c graph?)]
    )))

;; prosta implementacja grafów
(define-unit simple-graph@
  (import)
  (export graph^)

  (define (graph? g)
    (and (list? g)
         (eq? (length g) 3)
         (eq? (car g) 'graph)))

  (define (edge? e)
    (and (list? e)
         (eq? (length e) 3)
         (eq? (car e) 'edge)))

  (define (graph-nodes g) (cadr g))

  (define (graph-edges g) (caddr g))

  (define (graph n e) (list 'graph n e))

  (define (edge n1 n2) (list 'edge n1 n2))

  (define (edge-start e) (cadr e))

  (define (edge-end e) (caddr e))

  (define (has-node? g n) (not (not (member n (graph-nodes g)))))
  
  (define (outnodes g n)
    (filter-map
     (lambda (e)
       (and (eq? (edge-start e) n)
            (edge-end e)))
     (graph-edges g)))

  (define (remove-node g n)
    (graph
     (remove n (graph-nodes g))
     (filter
      (lambda (e)
        (not (eq? (edge-start e) n)))
      (graph-edges g)))))

;; sygnatura dla struktury danych
(define-signature bag^
  ((contracted
    [bag?       (-> any/c boolean?)]
    [bag-empty? (-> bag? boolean?)]
    [empty-bag  (and/c bag? bag-empty?)]
    [bag-insert (-> bag? any/c (and/c bag? (not/c bag-empty?)))]
    [bag-peek   (-> (and/c bag? (not/c bag-empty?)) any/c)]
    [bag-remove (-> (and/c bag? (not/c bag-empty?)) bag?)]
    [bag-list (-> bag? (listof any/c))]   ; DODANE
    [bag-cons (-> (listof any/c) bag?)]))) ; DODANE


; DODANE
;; struktura danych - stos
; stos jest zaimplementowany na dwuelementowej liscie,
; w ktorej pierwszy element to etykieta, a drugi to lista,
; ktorej pierwszy element nazywamy wierzcholkiem (peek)
(define-unit bag-stack@
  (import)
  (export bag^)

  (define (bag? b)
    (and (list? b)
         (eq? (length b) 2)
         (eq? (car b) 'bag-stack)))

  (define (bag-empty? b)
    (and (eq? (first b) 'bag-stack)
         (eq? (second b) '())))
  
  (define empty-bag
    (list 'bag-stack '()))

  (define (bag-insert b v)
    (list 'bag-stack (cons v (second b))))

  (define (bag-peek b)
    (car (second b)))

  (define (bag-remove b)
    (list 'bag-stack (cdr (second b))))

  ; bedziemy wyswietlac w odwrotnej kolejnosci niz wrzucilismy:
  ; (display (insert 1, insert 2, insert 3)) --> '(3 2 1)
  ; czyli pierwszy wynikowej listy element to wierzcholek
  (define (bag-list b)
    (second b))

  (define (bag-cons xs)
    (list 'bag-stack xs))
   
)
; ----- ----- ----- -----

; DODANE
;; struktura danych - kolejka FIFO
; kolejka to trzyelementowa lista, gdzie pierwszy element
; to etykieta, drugi to lista 'do dodawania elementow',
; a trzeci to lista 'do usuwania elementow'.
(define-unit bag-fifo@
  (import)
  (export bag^)
  
  (define (bag? b)
    (and (list? b)
         (eq? (length b) 3)
         (eq? (car b) 'bag-fifo)))

  ; puste <=> obie listy sa puste
  (define (bag-empty? b)
    (and (eq? (first b) 'bag-fifo)
         (eq? (second b) '())
         (eq? (third b) '())))
    
  (define empty-bag
    (list 'bag-fifo '() '()))

  (define (bag-insert b v)
    (list 'bag-fifo
          (cons v (second b))
          (third b)))

  ; ta procedura ma zwrocic wartosc a nie nowa strukture bag, dlatego
  ; nie odwracamy list w wewnetrznej reprezentacji fifo, za to:
  ;  a) jesli lista wyjsciowa jest pusta, to ostatni element wejsciowej,
  ;  b) wpp. pierwszy element listy wyjsciowej
  (define (bag-peek b)
    (if (eq? (third b) '())
        (last (second b))
        (car (third b))))

  ; jesli lista wyjsciowa jest pusta, to tworzymy nowa kolejke
  ; z pusta lista wejsciowa i odwrocona lista wejsciowa bez
  ; pierwszego elementu (oczywiscie cdr moze zwroci nam nulla
  ; w przypadku listy jednoelementowej - czyli wtedy obie listy
  ; beda puste). wpp usuwamy z listy wyjsciowej pierwszy element
  (define (bag-remove b)
    (if (eq? (third b) '())
        (list 'bag-fifo '() (cdr (reverse (second b))))
        (list 'bag-fifo (second b) (cdr (third b)))))

  ; podobnie jak przy stosie chcemy utrzymac odwrotna kolejnosc,
  ; z tym ze teraz wierzcholek to ostatni element wynikowej listy.
  (define (bag-list b)
    (append (second b) (reverse (third b))))

  (define (bag-cons xs)
    (list 'bag-fifo xs '()))

  
)
; ----- ----- ----- -----



;; sygnatura dla przeszukiwania grafu
(define-signature graph-search^
  (search))

;; implementacja przeszukiwania grafu
;; uzależniona od implementacji grafu i struktury danych
(define-unit/contract graph-search@
  (import bag^ graph^)
  (export (graph-search^
           [search (-> graph? any/c (listof any/c))]))
  (define (search g n)
    (define (it g b l)
      (cond
        [(bag-empty? b) (reverse l)]
        [(has-node? g (bag-peek b))
         (it (remove-node g (bag-peek b))
             (foldl
              (lambda (n1 b1) (bag-insert b1 n1))
              (bag-remove b)
              (outnodes g (bag-peek b)))
             (cons (bag-peek b) l))]
        [else (it g (bag-remove b) l)]))
    (it g (bag-insert empty-bag n) '()))
  )

;; otwarcie komponentu grafu
(define-values/invoke-unit/infer simple-graph@)

;; graf testowy
(define test-graph
  (graph
   (list 1 2 3 4)
   (list (edge 1 3)
         (edge 1 2)
         (edge 2 4))))

(define my-graph-1
  (graph
   (list 1 2 3 4 5 6)
   (list (edge 1 6)
         (edge 2 1)
         (edge 2 3)
         (edge 3 4)
         (edge 4 3)
         (edge 4 6)
         (edge 6 5))))

(define my-graph-2
  (graph
   (list 1 2 3 4 5)
   (list (edge 1 2)
         (edge 2 3)
         (edge 3 4)
         (edge 4 5)
         (edge 5 1))))

(define my-graph-3
  (graph
   (list 'd 'c 'b 'a 4 3 2 1)
   (list (edge 1 'd)
         (edge 1 2)
         (edge 2 3)
         (edge 3 4)
         (edge 'a 'b)
         (edge 'b 'c)
         (edge 'c 4)
         (edge 'd 'a))))


(define my-graph-4
  (graph
   (list 1 2 3 4 5 6 7 8 9 10)
   (list (edge 1 7)
         (edge 2 3)
         (edge 3 4)
         (edge 5 1)
         (edge 5 2)
         (edge 5 6)
         (edge 6 2)
         (edge 6 10)
         (edge 8 6)
         (edge 9 6)
         (edge 10 3))))



;; DODANE: otwieramy raz stos, raz kolejke wykomentowujac
;  w zaleznosci od tego, z czego chcemy korzystac
;; otwarcie komponentu stosu
(define-values/invoke-unit/infer bag-stack@)
;; opcja 2: otwarcie komponentu kolejki
;(define-values/invoke-unit/infer bag-fifo@)

;; testy w Quickchecku
(require quickcheck)

;; test przykładowy: jeśli do pustej struktury dodamy element
;; i od razu go usuniemy, wynikowa struktura jest pusta
(quickcheck
 (property ([s arbitrary-symbol])
           (bag-empty? (bag-remove (bag-insert empty-bag s)))))




; DODANE: moje testy
;; test dlugosci: jeśli do struktury dodamy jakis element
;; to dlugosc listy elementow powinna zwiekszyc sie o jeden
(quickcheck
 (property ([s arbitrary-symbol]
            [l (arbitrary-list arbitrary-integer)]
            [i arbitrary-integer])
           (let* ((test-bag (bag-cons l))
                  (l1 (length (bag-list test-bag)))
                  (l2 (length (bag-list (bag-insert test-bag i)))))
             (= (+ l1 1) l2))))

#|
;; TESTY DLA FIFO
;; test wierzcholka dla fifo: peek to zawsze pierwszy dodany element
; ktory jeszcze nie zostal zabrany przez remove
(define (add-elems-to-bag bag xs)
  (if (null? xs)
      bag
      (add-elems-to-bag (bag-insert bag (car xs)) (cdr xs))))

(quickcheck
 (property ([l (arbitrary-list arbitrary-integer)]
            [i arbitrary-integer])
           (let* ((l (cons i l)) ; zeby lista nie byla pusta
                  (elem (first l))
                  (test-bag (add-elems-to-bag empty-bag l)))
             (eq? (bag-peek test-bag) elem))))


;; test dla fifo: jesli mamy pusta liste wyjsciowa i usuniemy cos, to:
; a) lista wejsciowa jest pusta
; b) dlugosc listy wyjsciowej jest o jeden krotsza
;    niz byla przed usunieciem lista wejsciowa
; c) listy wyjsciowa teraz i wejsciowa przedtem odwrocona
;    bez pierwszego sa rowne
(quickcheck
 (property ([l (arbitrary-list arbitrary-integer)]
            [x arbitrary-integer])
           (let* ((lst (cons x l)) ; chcemy, zeby lista nigdy nie byla pusta
                  (test-bag (bag-cons lst))
                  (removed-test-bag (bag-remove test-bag)))
             (and [eq? (second removed-test-bag) '()]
                  [= (length (second test-bag))
                     (+ (length (third removed-test-bag)) 1)]
                  [equal? (cdr (reverse (second test-bag)))
                        (third removed-test-bag)]))))
             
;; test dla fifo: dlugosc listy calej struktury (bag-list)
; to suma dlugosci listy wejsciowej i listy wyjsciowej
(quickcheck
 (property ([in (arbitrary-list arbitrary-integer)]
            [out (arbitrary-list arbitrary-integer)])
           (let ((fif (list 'bag-fifo in out)))
             (= (length (bag-list fif))
                (+ (length in) (length out))))))
|#

;#|
;; TESTY DLA STACK
;; test dla stack: jesli dodamy do stosu jakis element,
; to owczesny wierzcholek bedzie teraz drugim elementem listy
(quickcheck
 (property ([l (arbitrary-list arbitrary-integer)]
            [x arbitrary-integer]
            [y arbitrary-integer])
           (let* ((lst (cons x l)) ; chcemy, zeby to byla niepusta lista
                  (stk (bag-cons lst))
                  (p (bag-peek stk))
                  (new-stk (bag-insert stk y)))
             (eq? p (second (second new-stk))))))

;; test wierzcholka dla stack: peek to zawsze ostatnio dodany element
(define (add-elems-to-bag bag xs)
  (if (null? xs)
      bag
      (add-elems-to-bag (bag-insert bag (car xs)) (cdr xs))))

(quickcheck
 (property ([l (arbitrary-list arbitrary-integer)]
            [i arbitrary-integer])
           (let* ((l (cons i l)) ; zeby lista nie byla pusta
                  (elem (last l))
                  (test-bag (add-elems-to-bag empty-bag l)))
             (eq? (bag-peek test-bag) elem))))



;|#


;; otwarcie komponentu przeszukiwania
(define-values/invoke-unit/infer graph-search@)

;; uruchomienie przeszukiwania na przykładowym grafie
(search test-graph 1)

; PRZESZUKIWANIE W MOICH GRAFACH
; (tym sposobem (wszerz / w glab), ktorego 'bag'
; nie jest wykomentowany

; (search my-graph-1 3)
; --> '(3 4 6 5)

;(search my-graph-2 4)
; --> '(4 5 1 2 3)

; (search my-graph-3 'd)
; --> '(d a b c 4)

; (search my-graph-4 5)
; --> '(5 6 10 3 4 2 1 7)
