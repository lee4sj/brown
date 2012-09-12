.data

a_:	.word 0
b_:	.word	0:10
c_:	.word 0

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
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sw	$9, b_($8)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	seq	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l0
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
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, b_($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	seq	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l1
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
	j	l2
l1: 
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sw	$9, b_($8)
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
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
l2: 
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$v0, 5
	syscall
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 4
	mul	$8, $8, $9
	sw	$2, b_($8)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
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
l3: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	slt	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l5
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	seq	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
l5: 
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l4
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
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	add	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sw	$8, a_
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sgt	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l6
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	seq	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$9, 0
	seq	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
l6: 
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l7
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
l7: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sgt	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l8
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	seq	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
l8: 
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l9
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
l9: 
	j	l3
l4: 
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	seq	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l10
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sle	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
l10: 
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l11
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
l11: 
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	seq	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l12
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sle	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
l12: 
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l13
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
l13: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	slt	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l14
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sge	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
l14: 
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l15
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
l15: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sle	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	bnez	$8, l16
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, c_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sgt	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
l16: 
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l17
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
l17: 
	li	$8, 5
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sgt	$8, $8, $9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	beqz	$8, l18
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
l18: 
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	rem	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
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
	add	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 144441
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 144444
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sub	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
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
	mul	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 11
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 55
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	div	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
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
	li	$8, 6
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sw	$9, b_($8)
	li	$8, 6
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	div	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 6
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, b_($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	mul	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	li	$8, 7
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	mul	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	div	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
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
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, b_($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	add	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
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
	lw	$8, a_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sub	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	li	$8, 9
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	add	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
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


