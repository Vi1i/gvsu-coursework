li $t1, 20
jal HERE
add $t1, $t2, $t2

HERE:
	li $t2, 5
	jr $ra
