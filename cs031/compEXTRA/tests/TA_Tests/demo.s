.data

b_:	.word	0:10
c_:	.word 0
a_:	.word 0

.text

main:
	sub $sp, $sp, 40
	sw  $ra, 40($sp)
	sw  $s0, 36($sp)
	sw  $s1, 32($sp)
	sw  $s2, 28($sp)
	sw  $s3, 24($sp)
	sw  $s4, 20($sp)
	sw  $s5, 16($sp)
	sw  $s6, 12($sp)
	sw  $s7, 8($sp)
	sw  $s8, 4($sp)

	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sw	$8, a_
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	sw	$9, b_($8)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	beq	$8, $9, l2
	li	$10, 0
l2: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l1
	j	l0
l1: 
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
l0: 
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 4
	mul	$8, $8, $9
	lw	$9, b_($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	beq	$8, $9, l6
	li	$10, 0
l6: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l4
	j	l5
l4: 
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	j	l3
l5: 
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	sw	$9, b_($8)
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 4
	mul	$8, $8, $9
	lw	$9, b_($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
l3: 
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 5
	syscall
	li	$9, 4
	mul	$8, $8, $9
	sw	$2, b_($8)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 4
	mul	$8, $8, $9
	lw	$9, b_($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$v0, 5
	syscall
	sw	$2, a_
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sw	$8, a_
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sw	$8, c_
l9: 
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	blt	$8, $9, l13
	li	$10, 0
l13: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 0
	bnez	$8, l11
	j	l10
l11: 
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	beq	$8, $9, l14
	li	$10, 0
l14: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 0
	bnez	$8, l12
	j	l10
l12: 
	li	$9, 1
l10: 
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l8
	j	l7
l8: 
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	add	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sw	$8, a_
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	blt	$8, $9, l20
	li	$10, 0
l20: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 0
	bnez	$8, l18
	j	l17
l18: 
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	beq	$8, $9, l22
	li	$10, 0
l22: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 0
	bnez	$8, l21
	li	$9, 1
l21: 
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 0
	bnez	$8, l19
	j	l17
l19: 
	li	$9, 1
l17: 
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l16
	j	l15
l16: 
	li	$8, 10000000
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
l15: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	blt	$8, $9, l28
	li	$10, 0
l28: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 0
	bnez	$8, l26
	j	l25
l26: 
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	beq	$8, $9, l29
	li	$10, 0
l29: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 0
	bnez	$8, l27
	j	l25
l27: 
	li	$9, 1
l25: 
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l24
	j	l23
l24: 
	li	$8, 12222222
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
l23: 
	j	l9
l7: 
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	beq	$8, $9, l33
	li	$10, 0
l33: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 1
	bnez	$8, l32
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	ble	$8, $9, l34
	li	$10, 0
l34: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 1
	bnez	$8, l32
	li	$9, 0
l32: 
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l31
	j	l30
l31: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
l30: 
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	beq	$8, $9, l38
	li	$10, 0
l38: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 1
	bnez	$8, l37
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	ble	$8, $9, l39
	li	$10, 0
l39: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 1
	bnez	$8, l37
	li	$9, 0
l37: 
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l36
	j	l35
l36: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
l35: 
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	blt	$8, $9, l43
	li	$10, 0
l43: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 1
	bnez	$8, l42
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	ble	$8, $9, l44
	li	$10, 0
l44: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 1
	bnez	$8, l42
	li	$9, 0
l42: 
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l41
	j	l40
l41: 
	li	$8, 11
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
l40: 
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	ble	$8, $9, l48
	li	$10, 0
l48: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 1
	bnez	$8, l47
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	blt	$8, $9, l49
	li	$10, 0
l49: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 1
	bnez	$8, l47
	li	$9, 0
l47: 
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l46
	j	l45
l46: 
	li	$8, 12
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
l45: 
	li	$8, 5
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 1
	blt	$8, $9, l52
	li	$10, 0
l52: 
	sub	$sp, $sp, 4
	sw	$10, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l51
	j	l50
l51: 
	li	$8, 13
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
l50: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	rem	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	add	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 144444
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 144441
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sub	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	mul	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 55
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 11
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	div	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 36
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sw	$8, a_
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 6
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	sw	$9, b_($8)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 6
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	div	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 7
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	mul	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 4
	mul	$8, $8, $9
	lw	$9, b_($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	li	$8, 6
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	mul	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	div	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 4
	mul	$8, $8, $9
	lw	$9, b_($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	add	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 36
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sub	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	add	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	lw  $s8, 4($sp)
	lw  $s7, 8($sp)
	lw  $s6, 12($sp)
	lw  $s5, 16($sp)
	lw  $s4, 20($sp)
	lw  $s3, 24($sp)
	lw  $s2, 28($sp)
	lw  $s1, 32($sp)
	lw  $s0, 36($sp)
	lw  $ra, 40($sp)
	add $sp, $sp, 40
	jr $ra


