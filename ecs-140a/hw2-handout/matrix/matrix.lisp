;; Helper function `dot-product` for matrix multiplication
;; Source: Piazza @246 + https://rosettacode.org/wiki/Dot_product#Common_Lisp
(defun dot-product (mat1 mat2)
    (mapcar #'(lambda (x) (apply '+ (mapcar #'* mat1 x))) mat2)
)

;; -------------------- ESSENTIAL FUNCTIONS ----------------------- ;;

;; FUNCTION TO ADD 2 MATRICES
;; Source: Piazza @183 (for mapcar lambda)
(defun matrix-add (mat1 mat2)
	(mapcar #'(lambda (a b) (mapcar '+ a b)) mat1 mat2)
)

;; FUNCTION TO TRANSPOSE MATRIX (SWAP ROL - COL)
;; Source: https://riptutorial.com/common-lisp/example/21146/examples-of-mapcar
(defun matrix-transpose (mat)
	(apply #'mapcar #'list mat)
)

;; FUNCTION TO MULTIPLY 2 MATRICES
;; NOTE: Use `matrix-transpose` to make it easier (Source: Piazza @229)
(defun matrix-multiply (mat1 mat2)
    (mapcar #'(lambda (x) (dot-product x (matrix-transpose mat2))) mat1)
)




;; TODO: This source use &rest (not covered in class) --> Redo `matrix-multiply`

;; FUNCTION TO MULTIPLY MATRICES
;; Source: https://rosettacode.org/wiki/Matrix_multiplication#Common_Lisp
;;(defun matrix-multiply (mat1 mat2)
;;	(mapcar (lambda (row) (apply #'mapcar (lambda (&rest column) (apply #'+ (mapcar #'* row column))) mat2)) mat1)
;;)