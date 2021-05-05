;; Find min number in list
(defun find-min (xs min)
	(cond
		;; return current min if list is empty/ended
		((null xs) min)

		;; if list size == 1, return car xs
		;; ((= (find-size xs) 1)
		;;	(car xs)
		;; )
        
		;; update new min found
		((< (car xs) min) (find-min (cdr xs) (car xs)))
        
		;; if no new min found, keep the current min
		((or (> (car xs) min) (= (car xs) min))
			(find-min (cdr xs) min)
		)
	)
)

;; Find max number in list
(defun find-max (xs max)
	(cond 
		;; return current max if list is empty/ended
		((null xs) max)
        
		;; if new max found, update
		;; else, keep the current max
		
		;; if list size == 1, return car xs
		;; ((= (find-size xs) 1)
		;;	(car xs)
		;; )

		;; if new max found, update
		((> (car xs) max) (find-max (cdr xs) (car xs)))
		
		;; else, keep the current max 
		((or (< (car xs) max) (= (car xs) max))
			(find-max (cdr xs) max)
		)
	)
)

;; Find size of list (for mean calculation)
;; Source: 06-Lisp.pdf (page 30)
(defun find-size (xs)
	(if (null xs) 0
		(+ 1 (find-size (cdr xs)))
	)
)

;; Find sum of list (for mean calculation)
;; Source: 06-Lisp.pdf (page 30) - inspired from `length`
(defun find-sum (xs)
	(if (null xs) 0
		(+ (car xs) (find-sum (cdr xs)))
  	)
)


;; -------- MAIN FUNCTION TO RETURN MIN-MEAN-MAX --------
(defun min-mean-max (xs)
	(setq min (find-min (cdr xs) (car xs)))
	(setq mean (/ (find-sum xs) (find-size xs)))
	(setq max (find-max (cdr xs) (car xs)))
	(list min mean max)
)