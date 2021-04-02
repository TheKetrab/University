#lang racket

;;; ---------------------------------------------------------------------------
;;; Prosty system obiektowy z dziedziczeniem

(define (ask object message . args)
  (let ((method (get-method object message)))
    (if (method? method)
        (apply method (cons object args))
        (error "Brak metody" message (cadr method)))))

(define (get-method object message)
  (object message))

(define (no-method name)
  (list 'no-method name))

(define (method? x)
  (not (no-method? x)))

(define (no-method? x)
  (if (pair? x)
      (eq? (car x) 'no-method)
      false))

; ===== ===== DODANE ===== =====

; klasa zmiennych - chcemy zapisywac sobie czy juz cos sie stalo
(define (make-vars)
  (let ([var-list '()])
    (lambda (message)
      (cond [(eq? message 'true?)
             (lambda (var)
               (not (eq? #f (member var var-list))))]
            [(eq? message 'false?)
             (lambda (var)
               (eq? #f (member var var-list)))]
            [(eq? message 'add)
             (lambda (var)
               (set! var-list (cons var var-list)))]
            [(eq? message 'remove)
             (lambda (var)
               (set! var-list (remove* (list var) var-list)))]
            [(eq? message 'var-list) var-list]
            [else (error "unknow-method" message)]))))

(define vars (make-vars)) ; <-- obiekt zmiennych



; klasa dziennika
; log to dwie listy: modyfikowalna (obecne) i zwykla (ukonczone),
; gdzie w obecnych przechowujemy nazwe zadania i wpisy,
; w ukonczonych przechowujemy tytuly ukonczonych zadan
; (po to, zeby gracz wiedzial, co ma aktualnie robic)

(define (make-log)

  ; metody prywatne
  ; ===== ===== ===== ===== ===== =====

  ; podobne do member tylko ze na mconsach
  ; i jak znajdzie to zwraca true
  (define (m-member? lst item)
    (cond
      [(null? lst) #f]
      [(eq? (mcar lst) item) #t]
      [else (m-member? (mcdr lst) item)]))

  ; dodaje na koniec listy modyfikowalnej item
  (define (add-at-the-end! lst item)
    (define (helper l)
      (if (null? (mcdr l))
          (begin (set-mcdr! l (mcons item null)) lst)
          (helper (mcdr l))))
    (if (null? lst) ; jesli jest nullem, to tworzy liste
        (mcons item null)
        (helper lst)))

  ; szuka czesci listy ktora jest odpowiednio etykietowana
  ; [pierwszy element] i dodaje item na koniec znalezionej listy
  (define (add-to-list-with-label! lst label item)
    (define (helper l)
      (cond [(null? l) (error "not found")]
            [(eq? (mcar (mcar l)) label)
             (begin (add-at-the-end! (mcar l) item) lst)]
            [else (helper (mcdr l))]))
    (helper lst))

  ; usuwa odpowiednio etykietowana liste, mamy dwa przypadki:
  ; a) pierwszy element trzeba usunac
  ; b) n-ty element trzeba usunac
  (define (remove-from-list-by-label lst label)
    (define (helper l)
      (cond [(null? l) null]
            [(null? (mcdr l)) null]
            [(eq? (mcar (mcar (mcdr l))) label) ; patrzymy na etykiete nastepnego elementu
             (begin (set-mcdr! l (mcdr (mcdr l))) lst)] ; i przepinamy wskaznik - zapominamy o tym pomiedzy
            [else (helper (mcdr l))]))
    (cond [(null? lst) null]
          [(eq? (mcar (mcar lst)) label)
           (mcdr lst)]
          [else (helper lst)]))
  
  ; wyswietlanie dziennika zadan
  (define (display-log-list lst)
    (define (display-quest entries)
      (unless (null? entries)
        (begin
          (display "> ")
          (display (mcar entries))
          (newline)
          (display-quest (mcdr entries)))))
    (unless (null? lst)
      (begin
        (display "----- ----- -----")
        (newline)
        (display "ZADANIE: ")             ; wyswietlamy napis ZADANIE
        (display (mcar (mcar lst)))       ; po czym dobieramy sie do pierwszego elementu
        (newline)                         ; w srodku czyli nazwy zadania, potem wyswietlamy
        (display-quest (mcdr (mcar lst))) ; wyswietlac wpisy w zadaniu od drugiego elementu
        (display-log-list (mcdr lst)))))  ; [bo pierwszy to nazwa]


  ; pola
  ; ===== ===== ===== ===== ===== =====
  (let ([actual '()]
        [done '()])

    
  ; metody publiczne
  ; ===== ===== ===== ===== ===== =====
    (lambda (message)
      (cond [(eq? message 'actual) actual]
	    [(eq? message 'done) done]
            [(eq? message 'add-quest)
             (lambda (title entry)
               (and
                (display "--> Nowy wpis w dzienniku: ")
                (display title)
                (newline)
                (set! actual (add-at-the-end! actual (mcons title (mcons entry null))))))]
           
            [(eq? message 'add-entry)
             (lambda (title entry)
               ; szuka odpowiedniej listy (zadania) i tam dodaje wpis
               (begin (display "--> Nowy wpis w dzienniku: ")
                      (display title)
                      (newline)
                      (add-to-list-with-label! actual title entry)
                      (void)))]
            [(eq? message 'set-success)
             (lambda (title)
               (begin
                 (display "--> Ukończono zadanie: ")
                 (display title)
                 (newline)
                 (set! actual (remove-from-list-by-label actual title))
                 (set! done (mcons title done))))]
            [(eq? message 'quest-done?)
             (lambda (title)
               (m-member? done title))]
            [(eq? message 'display)
             (display-log-list actual)]
	    [else 'unknow-method message]))))

(define log (make-log)) ; <-- obiekt log

; nazwy zadan
(define zdac-egzamin "Zdać egzamin")
(define papierosy-dla-palacza "Papierosy dla palacza")
(define egzaminator-oszust "Egzaminator oszust")
(define przeczytac-ksiege-wiedzy "Przeczytać księgę wiedzy")
(define szybkie-pioro-na-egzamin "Szybkie pióro na egzamin")


; ===== ===== ====== ===== =====






;;; ----------------------------------------------------------------------------
;;; Osoby, miejsca i rzeczy są nazwanymi obiektami

(define (make-named-object name)
  (lambda (message) 
    (cond ((eq? message 'name) (lambda (self) name))
	  (else (no-method name)))))

;;; Osoby i rzeczy są mobilne, ich miejsce może ulec zmianie

(define (make-mobile-object name location)
  (let ((named-obj (make-named-object name)))
    (lambda (message)
      (cond ((eq? message 'place)    (lambda (self) location))
	    ((eq? message 'install)
	     (lambda (self)
	       (ask location 'add-thing self)))
	    ;; Poniższa metoda nie powinna być wołana przez użytkownika
	    ;; Zobacz change-place
	    ((eq? message 'set-place)
	     (lambda (self new-place)
	       (set! location new-place)
	       'place-set))
	    (else (get-method named-obj message))))))

(define (make&install-mobile-object name place)
  (let ((mobile-obj (make-mobile-object name place)))
    (ask mobile-obj 'install)
    mobile-obj))

;;; Rzecz to coś, co może mieć właściciela

(define (make-thing name birthplace)
  (let ((owner     'nobody)
	(mobile-obj (make-mobile-object name birthplace)))
    (lambda (message)
      (cond ((eq? message 'owner)    (lambda (self) owner))
	    ((eq? message 'ownable?) (lambda (self) true))
	    ((eq? message 'owned?)
	     (lambda (self)
	       (not (eq? owner 'nobody))))
            
	    ;; Poniższa metoda nie powinna być wołana przez użytkownika
	    ;; Zobacz take i lose
	    ((eq? message 'set-owner)
	     (lambda (self new-owner)
	       (set! owner new-owner)
	       'owner-set))
	    (else (get-method mobile-obj message))))))

(define (make&install-thing name birthplace)	
  (let ((thing  (make-thing name birthplace)))
    (ask thing 'install)
    thing))

;;; Implementacja miejsc

(define (make-place name)
  (let ((neighbor-map '())		
	(things       '())
	(named-obj (make-named-object name)))
    (lambda (message)
      (cond ((eq? message 'things) (lambda (self) things))
	    ((eq? message 'neighbors)
	     (lambda (self) (map cdr neighbor-map)))
	    ((eq? message 'exits)
	     (lambda (self) (map car neighbor-map)))
	    ((eq? message 'neighbor-towards)
	     (lambda (self direction)
	       (let ((places (assq direction neighbor-map)))
		 (if places
		     (cdr places)
		     false))))
            ((eq? message 'add-neighbor)
             (lambda (self direction new-neighbor)
               (cond ((assq direction neighbor-map)
                      (display-message (list "Kierunek już przypisany"
					      direction name))
		      false)
                     (else
                      (set! neighbor-map
                            (cons (cons direction new-neighbor) neighbor-map))
		      true))))
	    ((eq? message 'accept-person?)
	     (lambda (self person)
	       true))
 
	    ;; Poniższe metody nie powinny być wołane przez użytkownika
	    ;; Zobacz change-place
            ((eq? message 'add-thing)
             (lambda (self new-thing)
               (cond ((memq new-thing things)
                      (display-message (list (ask new-thing 'name)
					     "już jest w" name))
		      false)
                     (else (set! things (cons new-thing things))
			   true))))
            ((eq? message 'del-thing)
             (lambda (self thing)
               (cond ((not (memq thing things))
                      (display-message (list (ask thing 'name)
					     "nie jest w" name))
		      false)
                     (else (set! things (delq thing things))
			   true))))

            (else (get-method named-obj message))))))

;;; ----------------------------------------------------------------------------
;;; Implementacja osób

(define (make-person name birthplace threshold)
  (let ((possessions '())
	(mobile-obj  (make-mobile-object name birthplace)))
    (lambda (message)
      (cond ((eq? message 'person?)     (lambda (self) true))
	    ((eq? message 'possessions) (lambda (self) possessions))
	    ((eq? message 'list-possessions)
	     (lambda (self)
	       (ask self 'say
		    (cons "Mam"
			  (if (null? possessions)
			      '("nic")
			      (map (lambda (p) (ask p 'name))
				      possessions))))
	       possessions))
	    ((eq? message 'say)
	     (lambda (self list-of-stuff)
	       (display-message
		 (append (list "W miejscu" (ask (ask self 'place) 'name)
			       ":"  name "mówi --")
			 (if (null? list-of-stuff)
			     '("Nieważne.")
			     list-of-stuff)))
	       'said))
	    ((eq? message 'have-fit)
	     (lambda (self)
	       (ask self 'say '("Jestem zły!!!"))
	       'I-feel-better-now))
	    ((eq? message 'look-around)
	     (lambda (self)
	       (let ((other-things
		       (map (lambda (thing) (ask thing 'name))
                               (delq self
                                     (ask (ask self 'place)
                                          'things)))))
                 (ask self 'say (cons "Widzę" (if (null? other-things)
						  '("nic")
						  other-things)))
		 other-things)))

	    ((eq? message 'take)
	     (lambda (self thing)
	       (cond ((memq thing possessions)
		      (ask self 'say
			   (list "Już mam" (ask thing 'name)))
		      true)
		     ((and (let ((things-at-place (ask (ask self 'place) 'things)))
			     (memq thing things-at-place))
			   (is-a thing 'ownable?))
		      (if (ask thing 'owned?)
			  (let ((owner (ask thing 'owner)))
			    (ask owner 'lose thing)
			    (ask owner 'have-fit))
			  'unowned)

		      (ask thing 'set-owner self)
		      (set! possessions (cons thing possessions))
		      (ask self 'say
			   (list "Biorę" (ask thing 'name)))
		      true)
		     (else
		      (display-message
		       (list "Nie możesz wziąć" (ask thing 'name)))
		      false))))
	    ((eq? message 'lose)
	     (lambda (self thing)
	       (cond ((eq? self (ask thing 'owner))
		      (set! possessions (delq thing possessions))
		      (ask thing 'set-owner 'nobody) 
		      (ask self 'say
			   (list "Tracę" (ask thing 'name)))
		      true)
		     (else
		      (display-message (list name "nie ma"
					     (ask thing 'name)))
		      false))))
	    ((eq? message 'move)
	     (lambda (self)
               (cond ((and (> threshold 0) (= (random threshold) 0))
		      (ask self 'act)
		      true))))
	    ((eq? message 'act)
	     (lambda (self)
	       (let ((new-place (random-neighbor (ask self 'place))))
		 (if new-place
		     (ask self 'move-to new-place)
		     false))))

	    ((eq? message 'move-to)
	     (lambda (self new-place)
	       (let ((old-place (ask self 'place)))
		 (cond ((eq? new-place old-place)
			(display-message (list name "już jest w"
					       (ask new-place 'name)))
			false)
		       ((ask new-place 'accept-person? self)
			(change-place self new-place)
			(for-each (lambda (p) (change-place p new-place))
				  possessions)
			(display-message
			  (list name "idzie z"    (ask old-place 'name)
				     "do"         (ask new-place 'name)))
			(greet-people self (other-people-at-place self new-place))
			true)
		       (else
			(display-message (list name "nie może iść do"
					       (ask new-place 'name))))))))
	    ((eq? message 'go)
	     (lambda (self direction)
	       (let ((old-place (ask self 'place)))
		 (let ((new-place (ask old-place 'neighbor-towards direction)))
		   (cond (new-place
			  (ask self 'move-to new-place))
			 (else
			  (display-message (list "Nie możesz pójść" direction
						 "z" (ask old-place 'name)))
			  false))))))
	    ((eq? message 'install)
	     (lambda (self)
	       (add-to-clock-list self)
	       ((get-method mobile-obj 'install) self)))

            ; ===== ===== DODANE ===== =====

            ; czasami chcemy powiedziec cos specjalnego do kogos
            [(eq? message 'talk-special)
             (lambda (self who sentence)
               (if (eq? (ask who 'place)
                        (ask student 'place))
                   ; jesli ten ktos jest w tym samym miejscu, to ok
                   (cond
                     [(and (eq? (ask who 'name) 'wch)
                           ((vars 'true?) 'student-bedzie-zgadywal))
                      (and
                       (if (or (equal? sentence "continuum")
                               (equal? sentence "Continuum")
                               (equal? sentence "CONTINUUM"))
                           (and
                            (display-dia student sentence)
                            (display-dia wch "Świetnie!")
                            ((vars 'remove) 'student-bedzie-zgadywal)
                            ((vars 'add) 'wch-correct-answer))
                           
                           (and
                            (display-dia student sentence)
                            (display-dia wch "To nie jest prawidłowa odpowiedź"))))]
                     ; przypadek domyslny - rozmowca nas zignoruje: "Dobra, dobra."
                     [else (and
                            (display-dia student sentence)
                            (display-dia who "Dobra, dobra."))])
                           
                   ; jesli tego kogos nie ma w poblizu
                   (display "NIE MOŻESZ MÓWIĆ DO KOGOŚ, KTO JEST DALEKO.")))]

            
            ; osoby moga uzywac niektore itemy
            [(eq? message 'use)
             (lambda (self item)
               (if (eq? (ask item 'owner) self)

                   ; jesli self ma ten przedmiot, to moze go uzyc
                   (cond
                     [(eq? (ask item 'name) 'ksiega-wiedzy)
                      (and
                       (display "AAA! To bardzo mądre.")
                       (when ((vars 'false?) 'ksiega-wiedzy-przeczytana)
                         (and
                          ((vars 'add) 'ksiega-wiedzy-przeczytana)
                          ((log 'set-success) przeczytac-ksiege-wiedzy))))]
                     ; normalnie wiekszosci rzeczy nie da sie uzyc
                     [else (display "tej rzeczy nie można użyć")])

                   ; jesli nie ma
                   (display "NIE MOZNA UŻYWAĆ CUDZEJ RZECZY")))]

            

            ; dialogi - niektore osoby maja dialogi /fabula/
            ((eq? message 'dialoge)
             (lambda (self)
               
               (if (eq? (ask self 'place)
                        (ask student 'place))
                   ; w zaleznosci z kim rozmawiamy i KIEDY,
                   ; duzo zalezy od stanu gry - zmiennych vars
                   (cond
                     ; jesli gadamy z...
                     [(eq? (ask self 'name) 'ref)
                      (cond [((vars 'false?) 'talked-to-ref-first-time)
                             (and
                              (display-dia student "Hej, Ref, ty już zdałeś egzamin z metod.")
                              (display-dia student "Czy możesz podać kilka rad, jak to zrobić?")
                              (display-dia self "Oczywiście! Są dwie możliwości - nauczyć się lub przekabacic egzaminatora")
                              (display-dia self "Jeśli chcesz się nauczyć powienieneś udać się do dabi")
                              (display-dia self "Jeśli wolisz oszukiwać udaj się do palących studentów przed instytutem")
                              (display-dia self "Gdy będziesz gotowy, udaj się do sali 25, tam fsieczkowski czeka na ciebie z arkuszem")
                              (display-dia self "Powodzenia!")
                              ((vars 'add) 'talked-to-ref-first-time)
                              ((log 'add-quest)
                               zdac-egzamin
                               "Muszę zdać egzamin, są dwa sposoby: nauczyć się albo oszukiwać. W zalezności, co wybieram, muszę udać się do dabi albo palących studentów przed instytutem. Gdy będę gotowy mogę pójść do fsieczkowski i rozpocząć egzamin."))]
                            [else (display-default-dia self)])]

                     [(eq? (ask self 'name) 'fsieczkowski)
                      (and
                       (display-dia student "Chcę podejść do egzaminu.")
                       (display-dia self "W porządku, masz tu kartkę.")
                       (display-dia student "[ ... STUDENT PISZE ... ]")
                       (display-dia egzaminator "[ ... OCENIANIE ... ]")
                       (if ((vars 'true?) 'uda-ci-sie-zdac-egzamin)
                           (and ; then
                            (display-dia self "Moje gratulacje, udało ci się zdać egzamin!")
                            ((log 'set-success) zdac-egzamin)
                            ((vars 'add) 'game-success)
                            (display "WYGRAŁEŚ !!!")
                            )
                           (and ; else
                            (display-dia self "Oblałeś egzamin, widzimy się za rok...")
                            ((vars 'add) 'game-fail)
                            (display "PRZEGRAŁEŚ !!!")
                            ))
                       ((vars 'add) 'end-of-the-game))]
                     [(eq? (ask self 'name) 'palacz1)
                      (cond [((vars 'false?) 'talked-to-palacz1-first-time)
                             (and
                              (display-dia student "Hej, Ref przysłał mnie do ciebie.")
                              (display-dia student "Co muszę zrobić, żeby zdać egzamin nie ucząc się?")
                              (display-dia self "Odpoczywać, jak ja!")
                              (display-dia self "Wiesz co, konczą mi się papierosy... Przynieś mi ich trochę.")
                              (display-dia self "Wtedy podzielę się z tobą swoim doświadczeniem.")
                              (display-dia student "Gdzie znajdę papierosy?")
                              (display-dia self "Widziałem jak leżały w plastycznej... Pospiesz się!")
                              ((vars 'add) 'talked-to-palacz1-first-time)
                              ((log 'add-quest)
                               papierosy-dla-palacza
                               "Muszę przynieść palaczowi papierosy. Podobno są w plastycznej."))]
                            [(eq? (ask papierosy 'owner) student)
                             (and
                              (display-dia student "Mam papierosy.")
                              (ask student 'lose papierosy)
                              (ask palacz1 'take papierosy)
                              (display-dia palacz1 "Świetnie, teraz powiem ci, co wiem.")
                              (display-dia palacz1 "Udaj się do biblioteki dziennikarzy.")
                              (display-dia palacz1 "Tam znajdziesz egzaminatora, którego da się przekupić.")
                              (display-dia student "Oby mi się udało...")
                              ((log 'set-success) papierosy-dla-palacza)
                              ((log 'add-entry)
                               zdac-egzamin
                               "Palacz poradził mi, abym udał się do egzaminatora w bibliotece dziennikarzy.")
                              )]
                                                          
                            [else (display-default-dia self)])]

                     [(eq? (ask self 'name) 'egzaminator)
                      (cond [(and ((log 'quest-done?) papierosy-dla-palacza)
                                 ((vars 'false?) 'talked-to-egz-first-time))
                             (and
                              (display-dia student "Hej, podobno możesz mi pomóc zdać egzamin.")
                              (display-dia self "Jasne, zawsze pomagam studentom, jeśli tylko oni pomagają mi.")
                              (display-dia self "Podejdź bliżej.")
                              (display-dia self "[SZEPCZE] Przynieś mi słój pacholskiego, bardzo chciałbym go mieć.")
                              (display-dia self "[SZEPCZE] Wtedy postaram się o to, żebyś zdał egzamin.")
                              (display-dia self "Powodzenia!")
                              ((vars 'add) 'talked-to-egz-first-time)
                              ((log 'add-quest)
                               egzaminator-oszust
                               "To niebywałe, że ten egzaminator oszukuje! Ależ szczęście mi się przytrafiło, pojawiła się rzeczywista szansa na to, że zdam egzamin. Musze tylko mu przynieść słój pacholskiego.")
                              )]
                            [(and (eq? (ask słój-pacholskiego 'owner) student)
                                  ((vars' true?) 'talked-to-egz-first-time))
                             (and
                              (display-dia student "Mam to, o co prosiłeś.")
                              (ask student 'lose słój-pacholskiego)
                              (ask egzaminator 'take słój-pacholskiego)
                              (display-dia self "Dzięki!")
                              (display-dia student "To co z tym egzaminem?")
                              (display-dia self "Tak, jak mówiłem. Już ja się postaram, żeby twoja praca trafiła w moje ręce.")
                              (display-dia self "Możesz iść pisać egzamin.")
                              ((vars 'add) 'uda-ci-sie-zdac-egzamin)
                              ((log 'set-success) egzaminator-oszust)
                              ((log 'add-entry)
                               zdac-egzamin
                               "Udało mi się dogadać z egzaminatorem. Teraz mogę iść napisać egzamin i być pewien, że go zdam")
                              )]
                            [else (display-default-dia self)])]

                     [(eq? (ask self 'name) 'dabi)
                      (cond [((vars 'false?) 'talked-to-dabi-first-time)
                             (and
                              (display-dia student "Hej, prowadziłeś repetytorium. Pomóż mi jeszcze więcej.")
                              (display-dia self "A więc szukasz wiedzy? Znajdź księgę wiedzy w bibliotece dziennikarzy.")
                              (display-dia self "Przeczytaj ją i udaj się do wch.")
                              (display-dia self "Będziesz też potrzebował dobrego pióra aby szybciej żwawo pisać i nie tracić czasu.")
                              (display-dia self "Tak się składa, że wch ma takie pióro.")
                              (display-dia self "Gdy to załatwisz wróć do mnie, dostaniesz jeszcze jedną radę.")
                              ((vars 'add) 'talked-to-dabi-first-time)
                              ((log 'add-quest)
                               przeczytac-ksiege-wiedzy
                               "W bibliotece znajduje się księga wiedzy, muszę ją znaleźć i przeczytać.")
                              ((log 'add-quest)
                               szybkie-pioro-na-egzamin
                               "Będę potrzebował pióra które jest ultra szybkie. Ponoć wch ma taki egzemplarz.")
                              ((log 'add-entry)
                               zdac-egzamin
                               "Dabi polecił mi przeczytać księgę wiedzy i zdobyć szybkie pióro. Potem mam wrócić do niego.")
                              )]
                            [(and ((log 'quest-done?) przeczytac-ksiege-wiedzy)
                                  ((log 'quest-done?) szybkie-pioro-na-egzamin)
                                  ((vars 'false?) 'talked-to-dabi-second-time))
                             (and
                              (display-dia student "Wszystko gotowe!")
                              (display-dia self "Fantastycznie - w takim razie ostatnia rada.")
                              (display-dia self "Jak nie wiesz, jak coś udowodnić, próbuj przez indukcję.")
                              (display-dia self "Możesz iść do fsieczkowski i zdać egzamin. Powodzenia!")
                              ((vars 'add) 'uda-ci-sie-zdac-egzamin)
                              ((log 'add-entry)
                               zdac-egzamin
                               "Wszystko załatwione, czuję się dobrze przygotowany do egzaminu! Pora udać się do sali 25")
                              )]
                                                          
                            [else (display-default-dia self)])]

                      [(eq? (ask self 'name) 'wch)
                      (cond [((vars 'false?) 'talked-to-wch-first-time)
                             (and
                              (display-dia student "Podobno masz szybkie pióro?")
                              (display-dia self "Mam. Chcesz go, prawda?")
                              (display-dia self "Dam ci go pod jednym warunkiem. Odpowiesz prawidłowo na pytanie.")
                              (display-dia self "Ile jest wszystkich funkcji z N w N?")
                              ((vars 'add) 'talked-to-wch-first-time)
                              ((vars 'add) 'student-bedzie-zgadywal)
                              ((log 'add-entry)
                               szybkie-pioro-na-egzamin
                               "Wch da mi pióro, jeśli uzna, że jestem godzien. Muszę powiedzieć mu ile jest wszystkich funkcji z N w N. [Użyj: (talk-special-to wch \"odpowiedź\")]")
                              )]
                            [(and ((vars 'true?) 'wch-correct-answer)
                                  ((vars 'false?) 'talked-to-wch-second-time))
                             (and
                              (display-dia student "Dasz mi teraz pióro?")
                              (display-dia self "Tak, jest twoje, zasłużyłeś.")
                              ((vars 'add) 'talked-to-wch-second-time)
                              ((log 'set-success) szybkie-pioro-na-egzamin)
                              )]
                            
                                                          
                            [else (display-default-dia self)])]

                      ; jesli to ktos kto nie ma dialogow w ogole,
                      ; to zawsze wyswietlamy dialogi domyslne
                      [else (display-default-dia self)])

                   ; else
                   (display "NIE MA TAKIEJ OSOBY W POKOJU!"))))

            ; ===== ===== ====== ===== =====

             
	    (else (get-method mobile-obj message))))))
  
(define (make&install-person name birthplace threshold)
  (let ((person (make-person name birthplace threshold)))
    (ask person 'install)
    person))

;;; Łazik umie sam podnosić rzeczy

(define (make-rover name birthplace threshold)
  (let ((person (make-person name birthplace threshold)))
    (lambda (message)
      (cond ((eq? message 'act)
	     (lambda (self)
	       (let ((possessions (ask self 'possessions)))
                 (if (null? possessions)
                     (ask self 'grab-something)
                     (ask self 'lose (car possessions))))))
            ((eq? message 'grab-something)
	     (lambda (self)
	       (let* ((things (ask (ask self 'place) 'things))
                      (fthings
                       (filter (lambda (thing) (is-a thing 'ownable?))
                               things)))
		 (if (not (null? fthings))
		     (ask self 'take (pick-random fthings))
		     false))))
	    ((eq? message 'move-arm)
	     (lambda (self)
               (display-message (list name "rusza manipulatorem"))
	       '*bzzzzz*))
	    (else (get-method person message))))))

(define (make&install-rover name birthplace threshold)
  (let ((rover  (make-rover name birthplace threshold)))
    (ask rover 'install)
    rover))


; ===== ===== DODANE ===== =====
; dziedziczenie po klasie person
; palacz to osoba, ktora gdy kogos spotka
; pyta sie go czy chce bucha albo czy ma ogien
(define (make-smoker name birthplace threshold)
  (let ((person (make-person name birthplace threshold)))
    (lambda (message)
      (cond ((eq? message 'say)
	     (lambda (self list-of-stuff)
	       ( ask person 'say ( append (if (> (random 2) 0)
                                              '("Chcesz bucha?")
                                              '("Masz może ogień?"))
                                          list-of-stuff))))
            (else (get-method person message))))))

(define (make&install-smoker name birthplace threshold)
  (let ((smoker  (make-smoker name birthplace threshold)))
    (ask smoker 'install)
    smoker))

; ===== ===== ====== ===== =====

;;; --------------------------------------------------------------------------
;;; Obsługa zegara

(define *clock-list* '())
(define *the-time* 0)

(define (initialize-clock-list)
  (set! *clock-list* '())
  'initialized)

(define (add-to-clock-list person)
  (set! *clock-list* (cons person *clock-list*))
  'added)

(define (remove-from-clock-list person)
  (set! *clock-list* (delq person *clock-list*))
  'removed)

(define (clock)
  (newline)
  (display "---Tick---")
  (set! *the-time* (+ *the-time* 1))
  (for-each (lambda (person) (ask person 'move))
	    *clock-list*)
  'tick-tock)
	     

(define (current-time)
  *the-time*)

(define (run-clock n)
  (cond ((zero? n) 'done)
	(else (clock)
	      (run-clock (- n 1)))))

;;; --------------------------------------------------------------------------
;;; Różne procedury

(define (is-a object property)
  (let ((method (get-method object property)))
    (if (method? method)
	(ask object property)
	false)))

(define (change-place mobile-object new-place)
  (let ((old-place (ask mobile-object 'place)))
    (ask mobile-object 'set-place new-place)
    (ask old-place 'del-thing mobile-object))
  (ask new-place 'add-thing mobile-object)
  'place-changed)

(define (other-people-at-place person place)
  (filter (lambda (object)
	    (if (not (eq? object person))
		(is-a object 'person?)
		false))
	  (ask place 'things)))

(define (greet-people person people)
  (if (not (null? people))
      (ask person 'say
	   (cons "Cześć"
		 (map (lambda (p) (ask p 'name))
			 people)))
      'sure-is-lonely-in-here))

(define (display-message list-of-stuff)
  (newline)
  (for-each (lambda (s) (display s) (display " "))
	    list-of-stuff)
  'message-displayed)

(define (random-neighbor place)
  (pick-random (ask place 'neighbors)))

(define (filter predicate lst)
  (cond ((null? lst) '())
	((predicate (car lst))
	 (cons (car lst) (filter predicate (cdr lst))))
	(else (filter predicate (cdr lst)))))

(define (pick-random lst)
  (if (null? lst)
      false
      (list-ref lst (random (length lst)))))  ;; See manual for LIST-REF

(define (delq item lst)
  (cond ((null? lst) '())
	((eq? item (car lst)) (delq item (cdr lst)))
	(else (cons (car lst) (delq item (cdr lst))))))


; DODANE
(define (display-dia who what)
  (and (display "> ")
       (display (ask who 'name))
       (display ": ")
       (display what)
       (newline)))

; DODANE
(define (display-default-dia who)
  (and (display-dia student "Co tam?")
       (display-dia who "Nic nowego.")))

;;------------------------------------------
;; Od tego miejsca zaczyna się kod świata

(initialize-clock-list)

;; Tu definiujemy miejsca w naszym świecie
;;------------------------------------------

(define hol              (make-place 'hol))
(define piętro-wschód    (make-place 'piętro-wschód))
(define piętro-zachód    (make-place 'piętro-zachód))
(define ksi              (make-place 'ksi))
(define continuum        (make-place 'continuum))
(define plastyczna       (make-place 'plastyczna))
(define wielka-wschodnia (make-place 'wielka-wschodnia))
(define wielka-zachodnia (make-place 'wielka-zachodnia))
(define kameralna-wschodnia (make-place 'kameralna-wschodnia))
(define kameralna-zachodnia (make-place 'kameralna-zachodnia))
(define schody-parter    (make-place 'schody-parter))
(define schody-piętro    (make-place 'schody-piętro))

(define biblioteka-dziennikarzy    (make-place 'biblioteka-dziennikarzy)) ; DODANE
(define przed-ii    (make-place 'przed-ii)) ; DODANE

;; Połączenia między miejscami w świecie
;;------------------------------------------------------

(define (can-go from direction to)
  (ask from 'add-neighbor direction to))

(define (can-go-both-ways from direction reverse-direction to)
  (can-go from direction to)
  (can-go to reverse-direction from))

(can-go-both-ways schody-parter 'góra 'dół schody-piętro)
(can-go-both-ways hol 'zachód 'wschód wielka-zachodnia)
(can-go-both-ways hol 'wschód 'zachód wielka-wschodnia)
(can-go-both-ways hol 'południe 'północ schody-parter)
(can-go-both-ways piętro-wschód 'południe 'wschód schody-piętro)
(can-go-both-ways piętro-zachód 'południe 'zachód schody-piętro)
(can-go-both-ways piętro-wschód 'zachód 'wschód piętro-zachód)
(can-go-both-ways piętro-zachód 'północ 'południe kameralna-zachodnia)
(can-go-both-ways piętro-wschód 'północ 'południe kameralna-wschodnia)
(can-go-both-ways schody-parter 'wschód 'zachód plastyczna)
(can-go-both-ways hol 'północ 'południe ksi)
(can-go-both-ways piętro-zachód 'zachód 'wschód continuum)

(can-go-both-ways ksi 'północ 'południe przed-ii) ; DODANE
(can-go-both-ways schody-parter 'zachód 'wschód biblioteka-dziennikarzy) ; DODANE


;; Osoby dramatu
;;---------------------------------------

(define student   (make&install-person 'student hol 0))
(define fsieczkowski
  (make&install-person 'fsieczkowski wielka-wschodnia 0)) ; DODANE - 3 <-- 0, ma sie nie ruszac
(define mpirog    (make&install-person 'mpirog wielka-wschodnia 3))
(define mmaterzok (make&install-person 'mmaterzok wielka-wschodnia 3))
(define jma       (make&install-person 'jma piętro-wschód 2))
(define klo       (make&install-person 'klo kameralna-wschodnia 2))
(define ref       (make&install-person 'ref ksi 0))
(define aleph1    (make&install-rover 'aleph1 continuum 3))

(define słój-pacholskiego (make&install-thing 'słój-pacholskiego schody-piętro))
(define trytytki     (make&install-thing 'trytytki continuum))
(define cążki-boczne (make&install-thing 'cążki-boczne continuum))

(define dabi (make&install-person 'dabi schody-piętro 3)) ; DODANE
(define wch (make&install-person 'wch piętro-wschód 2)) ; DODANE
(define palacz1 (make&install-smoker 'palacz1 przed-ii 1)) ; DODANE
(define palacz2 (make&install-smoker 'palacz2 przed-ii 1)) ; DODANE
(define egzaminator (make&install-person 'egzaminator biblioteka-dziennikarzy 0)) ; DODANE

(define papierosy (make&install-thing 'papierosy continuum)) ; DODANE
(define ksiega-wiedzy (make&install-thing 'ksiega-wiedzy continuum)) ; DODANE
(define szybkie-pioro (make&install-thing 'magiczne-pioro continuum)) ; DODANE


;; Polecenia dla gracza
;;------------------------------------------------

(define *player-character* student)

(define (look-around)
  (ask *player-character* 'look-around)
  #t)

(define (go direction)
  (ask *player-character* 'go direction)
  (clock)
  #t)

(define (exits)
  (display-message (ask (ask *player-character* 'place) 'exits))
  #t)



; ===== ===== DODANE ===== =====

; teleportacje nie wywoluja (clock)
; powiedzmy, ze sa to kody/cheaty do gry :)
(define (tp-to-place place)
  (ask *player-character* 'move-to place)
  #t)

(define (tp-to object)
  (let ((place (ask object 'place)))
    (tp-to-place place)
    #t))

; gracz moze w kazdej chwili otworzyc dziennik
; (ksiazke w ktorej zapisuje zadania, co musi robic itp.)
(define (open-log)
  (log 'display))

; gracz moze nawiazac dialog z NPCem
(define (talk-to npc)
  (ask npc 'dialoge))

; gracz moze powiedziec cos innego niz gotowe dialogi
(define (talk-special-to who sentence)
  (ask student 'talk-special who sentence))
  
(define (use-item item)
  (ask student 'use item))

(define (take-item item)
  (ask student 'take item))

(define (drop-item item)
  (ask student 'lose item))

#|
; Sposob 1 - oszustwo
(tp-to ref)
(talk-to ref)
(tp-to palacz1)
(talk-to palacz1)
(tp-to papierosy)
(take-item papierosy)
(tp-to palacz1)
(talk-to palacz1)
(tp-to egzaminator)
(talk-to egzaminator)
(tp-to słój-pacholskiego)
(take-item słój-pacholskiego)
(tp-to egzaminator)
(talk-to egzaminator)
(tp-to fsieczkowski)
(talk-to fsieczkowski)
|#

#|
; Sposob 2 - nauka
(tp-to ref)
(talk-to ref)
(tp-to dabi)
(talk-to dabi)
(tp-to ksiega-wiedzy)
(take-item ksiega-wiedzy)
(use-item ksiega-wiedzy)
(tp-to wch)
(talk-to wch)
(talk-special-to wch "continuum")
(talk-to wch)
(tp-to dabi)
(talk-to dabi)
(tp-to fsieczkowski)
(talk-to fsieczkowski)
|#

; UWAGI
; gra jest bardzo bardzo prymitywna i niedopracowana.
; z pewnoscia zawiera mnostwo bugow, brak betatestow.
; dlatego kierujmy sie stara zasada i nie bugujmy gry na sile
; --> np oczywiscie gracz mimo skonczonej gry moze dalej chodzic po swiecie
;     i nie wiadomo jak bardzo bugowac dialogi
;
;
; FABULA
; celem gracza jest zdac egzamin u fsieczkowskiego
; gre mozna przejsc na dwa sposoby:
; a) oszukiwac
; b) nauczyc sie
; kroki, ktore nalezy wykonac zostaly podane powyzej
;
;
; WSKAZOWKI
; - przez cala gre mozna zagladac do dziennika zadan
;   aby nie pogubic sie i wiedziec co robic
;   komenda (open-log)
; - z uwagi na nieprzyjemne poruszanie sie po mapie (go direction)
;   poleca sie korzystanie z gry na kodach
;   a) (tp-to person)
;   b) (tp-to-place place)
;   jest szybciej, latwiej, oraz zadna z powyzszych procedur
;   nie wywoluje procedury (clock) - postacie sie nie przemieszczaja,
;   a lazik nie kradnie przedmiotow
;  
;
; CO ZOSTALO DODANE?
; wszystkie zmiany w kodzie (chyba) zostly opatrzone
; etykieta 'DODANE' , konkretniej:
; -> zmiana ; DODANE
; -> ===== DODANE =====
;          zmiana
;    ===== ====== =====
;
; dodano
; kilka osob,
; przedmiotow,
; miejsc,
; podklase osob [smoker],
; system zmiennych (stan swiata),
; system wpisow do dziennika (misji),
; kilka nowych polecen dla gracza,
; mozliwosc wpisywania z klawiatury, co chcemy powiedziec do kogos,
; mozliwosc uzywania przedmiotow,
; system dialogow
; i chyba tyle
;
;
; KOMENDY DLA GRACZA
; (look-around) -------------------- co jest w pokoju?
; (go direction) ------------------- idz w strone...
; (exits) -------------------------- w ktora strone mozesz isc?
; (tp-to-place place) -------------- teleport do miejsca
; (tp-to object) ------------------- teleport do obiektu (osoby, przedmiotu)
; (open-log) ----------------------- wypisuje stan dziennika zadan
; (talk-to npc) -------------------- nawiaz dialog
; (talk-special-to who sentence) --- powiedz cos specjalnego
; (use-item item) ------------------ uzyj przedmiotu
; (take-item item) ----------------- wez przedmiot
; (drop-item item) ----------------- wyrzuc przedmiot
