;; MAIN MATCH FUNCTION
(defun match (pattern assertion)
	(cond
		;; If pattern is the same with assertion, return True
		((equal pattern assertion) t)
		
		;; If the first atom in pattern and assertion matches,
		;; recurse to check if they match for the rest of the atoms
		((equal (car pattern) (car assertion)) (match (cdr pattern) (cdr assertion)))

		;; If pattern or assertion is empty, return NIL
		((or (null pattern) (null assertion)) nil)


	;; ----------------------- SPECIAL CASES ---------------------- ;;
		
		;; If pattern has "?" or "!", recurse the rest of the pattern and assertion 
		;; to find if there is any atom that matches after the '?' character
		;; NOTE: There should be 5 test cases related to stand-alone "?" or "!" --> use OR
		((or (equal (car pattern) '?) (equal (car pattern) '!))
			(or (match (cdr pattern) (cdr assertion)) 
				(match pattern (cdr assertion))
			)
		)
	)
)