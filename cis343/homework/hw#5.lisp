;; 9 ;;
(defun compare (z y)
	(< (* 2 y) (+ 10 z)))

;; 10 ;;
(defun power-of-two (z)
	(if (= z 0) 1
		(* 2 (power-of-two (1- z)))))

;; 11 ;;
(defun num-even (l)
	(length (remove-if-not #'evenp l)))
