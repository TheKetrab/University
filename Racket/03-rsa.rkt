#lang racket

;;;; fast modular exponentiation. From the textbook, section 1.2

(define (expmod b e m)
  (cond ((zero? e) 1)
        ((even? e)
         (remainder (square (expmod b (/ e 2) m))
                    m))
        (else
         (remainder (* b (expmod b (- e 1) m))
                    m))))

(define (square x) (* x x))


;;; An RSA key consists of a modulus and an exponent.

(define make-key cons)
(define key-modulus car)
(define key-exponent cdr)

(define (RSA-transform number key)
  (expmod number (key-exponent key) (key-modulus key)))


;;;; generating RSA keys

;;; To choose a prime, we start searching at a random odd number in a
;;; specifed range

(define (choose-prime smallest range)
  (let ((start (+ smallest (choose-random range))))
    (search-for-prime (if (even? start) (+ start 1) start))))

(define (search-for-prime guess)
  (if (fast-prime? guess 2)
      guess
      (search-for-prime (+ guess 2))))

;;; The following procedure picks a random number in a given range,
;;; but makes sure that the specified range is not too big for
;;; Scheme's RANDOM primitive.

(define choose-random
  ;; restriction of Scheme RANDOM primitive
  (let ((max-random-number (expt 10 18))) 
    (lambda (n)
      (random (floor (min n max-random-number))))))


;;; The Fermat test for primality, from the texbook section 1.2.6

(define (fermat-test n)
    (let ((a (choose-random n)))
      (= (expmod a n n) a)))

(define (fast-prime? n times)
    (cond ((zero? times) true)
          ((fermat-test n) (fast-prime? n (- times 1)))
          (else false)))


;;; RSA key pairs are pairs of keys

(define make-key-pair cons)
(define key-pair-public car)
(define key-pair-private cdr)

;;; generate an RSA key pair (k1, k2).  This has the property that
;;; transforming by k1 and transforming by k2 are inverse operations.
;;; Thus, we can use one key as the public key andone as the private key.

(define (generate-RSA-key-pair)
  (let ((size (expt 2 14)))
    ;; we choose p and q in the range from 2^14 to 2^15.  This insures
    ;; that the pq will be in the range 2^28 to 2^30, which is large
    ;; enough to encode four characters per number.
    (let ((p (choose-prime size size))
          (q (choose-prime size size)))
    (if (= p q)       ;check that we haven't chosen the same prime twice
        (generate-RSA-key-pair)     ;(VERY unlikely)
        (let ((n (* p q))
              (m (* (- p 1) (- q 1))))
          (let ((e (select-exponent m)))
            (let ((d (invert-modulo e m)))
              (make-key-pair (make-key n e) (make-key n d)))))))))


;;; The RSA exponent can be any random number relatively prime to m

(define (select-exponent m)
  (let ((try (choose-random m)))
    (if (= (gcd try m) 1)
        try
        (select-exponent m))))


;;; Invert e modulo m

(define (invert-modulo e m)
  (if (= (gcd e m) 1)
      (let ((y (cdr (solve-ax+by=1 m e))))
        (modulo y m))                   ;just in case y was negative
      (error "gcd not 1" e m)))


;; ----- ----- ----- ----- -----

;;; solve ax+by=1
;;; The idea is to let a=bq+r and solve bx+ry=1 recursively

(define (solve-ax+by=1 a b)
  (if (= a 1) ; czyli b=0
      (cons 1 0)
      (let ((r (remainder a b))  ; a=bq+r
            (q (floor (/ a b)))) ; bx'+ry'=1  | +bqy'
                                 ; bx'+ry'+bqy'=1+bqy'
                                 ;    ----->    (bq+r)y'+b(x'-qy')=1
                                 ;               ---- -  - ------
                                 ;                 a  x  b   y
          (let ((solved (solve-ax+by=1 b r)))
            (cons (cdr solved)                     ; <-   y'
                  (+ (car solved)                  ; <-   x'    | \
                     (* (- q) (cdr solved)))))))) ; <-   -q*y' | ---> (x'-qy')

;; testy:
;; (solve-ax+by=1 12 11)        ->  1 . -1 , ok
;; (solve-ax+by=1 7424 1903)    ->  -496 . 1935 , ok
;; (solve-ax+by=1 616 4803)     ->  538 . -69 , ok

;; ----- ----- ----- ----- -----


;;; Actual RSA encryption and decryption

(define (RSA-encrypt string key1)
  (RSA-convert-list (string->intlist string) key1))

(define (RSA-convert-list intlist key)
  (let ((n (key-modulus key)))
    (define (convert l sum)
      (if (null? l)
          '()
          (let ((x (RSA-transform (modulo (- (car l) sum) n)
                                  key)))
            (cons x (convert (cdr l) x)))))
    (convert intlist 0)))

(define (RSA-decrypt intlist key2)
  (intlist->string (RSA-unconvert-list intlist key2)))


;; ----- ----- ----- ----- -----

(define (RSA-unconvert-list intlist key)
  ;; enc: (x1 x2 x3 ... xn)
  ;; plaintext: (a1 a2 a3 ... an)
  ;; 
  ;; skoro uzywalismy poprzednich wynikow
  ;; do otrzymania kolejnych zaszyfrowanych
  ;; to zachodzi zaleznosc:
  ;;
  ;; [x(i)^d]%n = [a(i)-x(i-1)]%n
  ;; C = [a(i)-x(i-1)%n
  ;; a(i) = k*n + C + x(i-1)
  ;; doswiadczalnie otrzymalem, ze k = -1

  (let ((n (key-modulus key))
        (d (key-exponent key)))
      
    (define (convert l previous)
      (let ((prev (car l))
            (C (expmod (car l) d n)))
        (let ((ai (modulo (+ (* (- 1) n)
                                 C
                                 previous)
                          n)))
          (if (null? (cdr l))
              (list ai)
              (cons ai (convert (cdr l) prev))))))
    (convert intlist 0)))

;; testy:
;; test-key-pair1 '(123 11 66 453 1235)  ->  '(331143016 277390227 134700958 167445559 483008844) -> '(123 11 66 453 1235)
;; test-key-pair2 '(123 11 66 453 1235)  ->  '(258096661 804228917 334361167 319472583 172254028) -> '(123 11 66 453 1235)
;; test-key-pair3 '(123 11 66 453 1235)  ->  '(855 486 539 1327 323) -> '(123 11 66 453 1235)
;; ok
;; test-key-pair1 '(2 3 5 8)  ->  '(46673318 91668520 76842059 224658375) -> '(2 3 5 8)
;; test-key-pair2 '(2 3 5 8)  ->  '(20402285 122501233 182817700 364492885) -> '(2 3 5 8)
;; test-key-pair3 '(2 3 5 8)  ->  '(1752 583 1443 2217) -> '(2 3 5 8)
;; ok

;; ----- ----- ----- ----- -----


;;;; Digital signatures

;;; The following routine compresses a list of numbers to a single
;;; number for use in creating digital signatures.

(define (compress intlist)
  (define (add-loop l)
    (if (null? l)
        0
        (+ (car l) (add-loop (cdr l)))))
  (modulo (add-loop intlist) (expt 2 28)))


;;; Define the data structure that represents signed messages here

(define make-signed-message cons)
(define signature car)
(define message cdr)

;;; Encrypting and signing a message
(define (encrypt-and-sign message recipient-public-key sender-private-key)

  (let ((enc (RSA-encrypt message recipient-public-key)))        ; enc - zaszyfrowana wiadomosc
    (let ((hash (compress enc)))                                 ; hash - wynik funkcji hashujacej
      (let ((signature (RSA-transform hash sender-private-key))) ; podpis
        (cons signature enc)))))                                 ; para: podpis, wiadomosc

(define (authenticate-and-decrypt cyphertext recipient-private-key sender-public-key)
  ; Wspomagalem sie rysunkiem 3
  ; http://wazniak.mimuw.edu.pl/images/2/20/Bsi_05_wykl.pdf

  (let ((hash (compress (message cyphertext))))                                ; policz hasha uzywajac ustalonej funkcji skrotu
    (let ((my-check (RSA-transform (signature cyphertext) sender-public-key))) ; liczymy podpis na podstawie klucza publicznego

      (if (= my-check hash)                                                    ; jesli to, co ja sprawdzilem i hash sa takie same...
          (RSA-decrypt (message cyphertext) recipient-private-key)             ; odszyfruj tekst kluczem prywatnym
          (error "Podpis sie nie zgadza!")))))                                 ; blad wpp.


;;;; searching for divisors.

;;; The following procedure is very much like the find-divisor
;;; procedure of section 1.2 of the text, except that it increments
;;; the test divisor by 2 each time.  You should be careful to call
;;; it only with odd numbers n.

(define (smallest-divisor n)
  (find-divisor n 3))

(define (find-divisor n test-divisor)
  (cond ((> (square test-divisor) n) n)
        ((divides? test-divisor n) test-divisor)
        (else (find-divisor n (+ test-divisor 2)))))

(define (divides? a b)
  (= (remainder b a) 0))



;;;; converting between strings and numbers

;;; The following procedures are used to convert between strings, and
;;; lists of integers in the range 0 through 2^28.  You are not
;;; responsible for studying this code -- just use it.

;;; Convert a string into a list of integers, where each integer
;;; encodes a block of characters.  Pad the string with spaces if the
;;; length of the string is not a multiple of the blocksize.

(define (string->intlist string)
  (let ((blocksize 4))
    (let ((padded-string (pad-string string blocksize)))
      (let ((length (string-length padded-string)))
        (block-convert padded-string 0 length blocksize)))))

(define (block-convert string start-index end-index blocksize)
  (if (= start-index end-index)
      '()
      (let ((block-end (+ start-index blocksize)))
        (cons (charlist->integer
	       (string->list (substring string start-index block-end)))
              (block-convert string block-end end-index blocksize)))))

(define (pad-string string blocksize)
  (let ((rem (remainder (string-length string) blocksize)))
    (if (= rem 0)
        string
        (string-append string (make-string (- blocksize rem) #\Space)))))

;;; Encode a list of characters as a single number
;;; Each character gets converted to an ascii code between 0 and 127.
;;; Then the resulting number is c[0]+c[1]*128+c[2]*128^2,...

(define (charlist->integer charlist)
  (let ((n (char->integer (car charlist))))
    (if (null? (cdr charlist))
        n
        (+ n (* 128 (charlist->integer (cdr charlist)))))))

;;; Convert a list of integers to a string. (Inverse of
;;; string->intlist, except for the padding.)

(define (intlist->string intlist)
  (list->string
   (apply
    append
    (map integer->charlist intlist))))



;;; Decode an integer into a list of characters.  (This is essentially
;;; writing the integer in base 128, and converting each "digit" to a
;;; character.)

(define (integer->charlist integer)
  (if (< integer 128)
      (list (integer->char integer))
      (cons (integer->char (remainder integer 128))
            (integer->charlist (quotient integer 128)))))

;;;; Some initial test data

(define test-key-pair1
  (make-key-pair
   (make-key 816898139 180798509)
   (make-key 816898139 301956869)))

(define test-key-pair2
  (make-key-pair
   (make-key 513756253 416427023)
   (make-key 513756253 462557987)))

(define test-key-pair3
  (make-key-pair
   (make-key 3233 17)
   (make-key 3233 2753)))
  

;; ----- ----- ----- ----- -----

;; moj test
(define wiadomosc "idea RSA byla na KBK !")

;; 1 wysyla do 2
(define rcp-pub-k (key-pair-public test-key-pair1))
(define sen-pri-k (key-pair-private test-key-pair2))
(define rec-pri-k (key-pair-private test-key-pair1))
(define sen-pub-k (key-pair-public test-key-pair2))

(define zaszyfrowane (encrypt-and-sign wiadomosc  rcp-pub-k sen-pri-k))
(authenticate-and-decrypt zaszyfrowane rec-pri-k sen-pub-k)
