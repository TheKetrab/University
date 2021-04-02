#lang racket

(define (const? t)
  (number? t))

(define (binop? t)
  (and (list? t)
       (= (length t) 3)
       (member (car t) '(+ - * /))))

(define (binop-op e)
  (car e))

(define (binop-left e)
  (cadr e))

(define (binop-right e)
  (caddr e))

(define (binop-cons op l r)
  (list op l r))

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

(define (hole? t)
  (eq? t 'hole))

(define (arith/let/holes? t)
  (or (hole? t)
      (const? t)
      (and (binop? t)
           (arith/let/holes? (binop-left  t))
           (arith/let/holes? (binop-right t)))
      (and (let? t)
           (arith/let/holes? (let-expr t))
           (arith/let/holes? (let-def-expr (let-def t))))
      (var? t)))

(define (num-of-holes t)
  (cond [(hole? t) 1]
        [(const? t) 0]
        [(binop? t)
         (+ (num-of-holes (binop-left  t))
            (num-of-holes (binop-right t)))]
        [(let? t)
         (+ (num-of-holes (let-expr t))
            (num-of-holes (let-def-expr (let-def t))))]
        [(var? t) 0]))

(define (arith/let/hole-expr? t)
  (and (arith/let/holes? t)
       (= (num-of-holes t) 1)))

(define (hole-context e)

  ; ----- ----- -----
  ; odp trzymamy w liscie (#t/#f , lista zmiennych)
  ; #t - znaleziono juz dziure, #f - jeszcze nie
  (define (answer pred env)
    (list pred env))
  (define (answer-true? a)
    (eq? #t (car a)))
  (define (answer-env a)
    (cadr a))

  ; ----- ----- -----
  ; problemem jest to, ze wyrazenie czasem jest
  ; para, a czasem nie. dlatego trzeba sprawdzac
  ; zarowno (xxx? (car e)), jak i (xxx? e)
  ; --> procedura zwraca obiekt answer
  (define (seek-hole-expr e ans)
    (cond
      [(answer-true? ans) ans]
      [(null? e) ans]
      ; ---
      [(hole? e) (answer #t (answer-env ans))]
      [(const? e) ans]
      [(binop? e)
       (seek-hole-binop e ans)]
      [(let? e)
       (seek-hole-let e ans)]
      [(var? e) ans]
      ; ---
      [(hole? (car e)) (answer #t (answer-env ans))]
      [(const? (car e)) (seek-hole-expr (cdr e) ans)]
      [(binop? (car e))
       (seek-hole-expr (cdr e)
                       (seek-hole-binop (car e) ans))]
      [(let? (car e))
       (seek-hole-expr (cdr e)
                       (seek-hole-let (car e) ans))]
      [(var? (car e)) (seek-hole-expr (cdr e) ans)]))

  ; ----- ----- -----
  ; jesli udalo sie znalezc answer-true
  ; w lewym lub prawym poddrzewie, to zwroci
  ; odp, wpp. zwroci to answer, ktore dostal
  (define (seek-hole-binop e ans)
    (let ((l-ans (seek-hole-expr (binop-left e) ans)))
      (if (answer-true? l-ans)
          l-ans
          (let ((r-ans (seek-hole-expr (binop-right e) ans)))
            (if (answer-true? r-ans)
                r-ans
                ans)))))
  
  ; ----- ----- -----
  ; jesli sie udalo znalezc dziure to zwroci
  ; gotowe answer-true, wpp. zwroci to, co dostal
  (define (seek-hole-let e ans)
    (let ((d-ans (seek-hole-let-def (let-def e) ans)))
      (if (answer-true? d-ans)
          d-ans          
          ; teraz szukamy z nowym srodowiskiem ( d-ans )
          (let ((e-ans (seek-hole-expr (let-expr e) d-ans)))  
            (if (answer-true? e-ans)
                e-ans
                ans)))))

  ; ----- ----- -----
  ; jesli ktoras czesc let-def jest dziura
  ; to zwroci answer-true z tym srodowiskiem,
  ; ktore juz ma. wpp. doda zmienna do srodowiska
  ; (bez powrotrzen) i zwroci takie srodowisko
  (define (seek-hole-let-def e ans)
    (let ((lde (let-def-expr e))
          (ldv (let-def-var e)))
      (cond
        [(hole? ldv) (answer #t (answer-env ans))]
        [(hole? lde) (answer #t (answer-env ans))]
        [else
         (let ((new-answer (seek-hole-expr lde ans)))
           (if (answer-true? new-answer)
               new-answer
               (answer #f (cons ldv
                                (remove* (list ldv) (answer-env ans))))))])))

  (let ((result (seek-hole-expr e (answer #f '()))))
    (if (answer-true? result)
        (answer-env result)
        (error "hole not found"))))

(define (test)

  ; ----- ----- -----
  (define (included? xs ys)
    (cond [(null? xs) #t]
          [(is-member? (car xs) ys)
           (included? (cdr xs) ys)]
          [else #f]))

  (define (is-member? x xs)
    (cond [(null? xs) #f]
          [(eq? x (car xs)) #t]
          [else (is-member? x (cdr xs))]))

  ; ----- ----- -----
  ; hole-context podaje dobry wynik, jesli
  ; zwraca liste z tymi samymi zmiennymi
  ; (niekoniecznie w tej samej kolejnosci)
  ; korzystajac z tego, ze zmienne na listach
  ; sie nie powtarzaja, wystarczy sprawdzic,
  ; czy listy maja taka sama dlugosc i czy
  ; wszystkie zmienne z cont znajduja sie
  ; gdzies na liscie good
  (define (good-context? cont good)
    (and (= (length cont) (length good))
         (included? cont good)))

  ; ----- TESTY -----
  (and
   (good-context? (hole-context '( let (x 3) ( let (y 7) (+ x hole ))))
                  '(y x))
   (good-context? (hole-context '((let (x 3) hole)))
                  '(x))
   (good-context? (hole-context '((let (x 3) (let (y 2) (+ x hole)))))
                  '(x y))
   (good-context? (hole-context '(+ 3 hole))
                  '())
   (good-context? (hole-context '(let (x 3) (let (y 7) (+ x hole ))))
                  '(y x))
   (good-context? (hole-context '(let (x 3) (let (y hole ) (+ x 3))))
                  '(x))
   (good-context? (hole-context '(let (x hole) (let (y 7) (+ x 3))))
                  '())
   (good-context? (hole-context '(let (piesek 1)
                   (let (kotek 7)
                      (let (piesek 9)
                         (let (chomik 5)
                            hole)))))
                  '( chomik piesek kotek ))
   (good-context? (hole-context '(+ (let (x 4) 5) hole))
                  '())
   (good-context? (hole-context '(+ (let (x (+ 3 4)) (let (y (+ 4 (* hole 2))) y)) 13))
                  '(x))
   (good-context? (hole-context '(+ (let (x (+ 3 4)) (let (y (+ 4 (* x 2))) hole)) 13))
                  '(x y))
  ))


; --- ---
(test)

