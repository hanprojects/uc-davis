;; Source: Logic inspired from NFA assingment in Project 1 (Golang)

;; FUNCTION TO GET POSSIBLE TRANSITIONS FROM A START STATE
(defun get-transitions (transition start final input)
	(cond
		;; If no start states to choose, return nil
		((null start) nil)
		
		;; If transition from current start to final state is reachable, return true
		(t (or (reachable transition (car start) final input)
		
		;; Else, continue by recursing with remaining states
		(get-transitions transition (cdr start) final input)))
	)
)

;; FUNCTION TO CHECK IF THE TRANSITION WILL REACH ACCEPT STATE FROM START
(defun reachable (transition start final input)
	(cond
		;; if there is no input AND start == final, return true
		;; WARNING: Cannot use `and` for (null input)
		((null input) (if (equal start final) t))
		
		;; Get the transitions from the first input (start) with the rest of the input
		;; and return true if reached the final state
		;; NOTE: Use `funcall` as we wanna get the first state of the remaining states
		(t (get-transitions transition (funcall transition start (car input)) final (cdr input)))
	)
)