;; NOTE: `append` usage is allowed (Piazza @221)

;; Get list of numbers smaller than n
(defun find-smaller (n xs)
	(cond
		;; if xs is null/end of list, return nil
		((null xs) nil)
		
		;; if current item in xs smaller than n, append it to new list of xs
		;; NOTE: Use recursion to do the same thing for the rest of xs
		((< (car xs) n) 
			(append (list (car xs)) (find-smaller n (cdr xs)))
		)
		
		;; else, continue to find smaller num with the rest of xs and return when done
		(t (find-smaller n (cdr xs)))
	)
)

;; Get list of numbers larger than n
(defun find-larger (n xs)
	(cond
		;; if xs is null/end of list, return nil
		((null xs) nil)
		
		;; if current item in xs larger than n, append it to xs
		((>= (car xs) n) 
			(append (list (car xs)) (find-larger n (cdr xs)))
		)
		
		;; else, continue to find larger num with the rest of the list and return when done
		(t (find-larger n (cdr xs)))
	)
)

;; PIVOT FUNCTION TO DISPLAY 2 SPLIT LISTS
(defun pivot (n xs)
	(list (find-smaller n xs) (find-larger n xs))
)

;; QUICKSORT FUNCTION TO PERFORM QUICK SORTING
(defun quicksort (xs)
	(if (not (null xs))
		(append
			(quicksort (find-smaller (car xs) (cdr xs)))
			(cons (car xs) nil)
			(quicksort (find-larger (car xs) (cdr xs)))
		)
	)
)
