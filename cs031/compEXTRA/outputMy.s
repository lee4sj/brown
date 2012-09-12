.data

a_:	.word 0
b_:	.word	0:10
c_:	.word	0:11
array_:	.word	0:10
arrayPointer_:	.word 0
counter_:	.word 0

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

	la	$8, b_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sw	$8, a_
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
	la	$8, b_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 13
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$8, a_
	sw	$9, ($8)
	sw	$8, a_
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
	lw	$8, a_
	lw	$9, ($8)
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
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sw	$9, b_($8)
	li	$8, 4
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
	lw	$8, a_
	lw	$9, ($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	la	$9, c_
	add	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sw	$9, b_($8)
	li	$8, 2
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
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	la	$9, c_
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
	li	$8, 3
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	lw	$10, b_($8)
	sw	$9, ($10)
	sw	$10, b_($8)
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, c_($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 4
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sw	$9, c_($8)
	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, b_($8)
	lw	$8, ($9)
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
	sw	$8, counter_
l0: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, counter_
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
	beqz	$8, l1
	lw	$8, counter_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, counter_
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	lw	$9, 4($sp)
	add	$sp, $sp, 4
	sw	$9, array_($8)
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, counter_
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
	sw	$8, counter_
	j	l0
l1: 
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	li	$10, 4
	mul	$8, $8, $10
	la	$9, array_
	add	$9, $9, $8
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sw	$8, arrayPointer_
	li	$8, 0
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, 4($sp)
	add	$sp, $sp, 4
	sw	$8, counter_
l2: 
	li	$8, 10
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, counter_
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
	beqz	$8, l3
	lw	$8, arrayPointer_
	lw	$9, ($8)
	sub	$sp, $sp, 4
	sw	$9, 4($sp)
	lw	$4, 4($sp)
	add	$sp, $sp, 4
	li	$v0, 1
	syscall
	li	$v0, 11
	li	$a0, 10
	syscall
	li	$8, 4
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, arrayPointer_
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
	sw	$8, arrayPointer_
	li	$8, 1
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	lw	$8, counter_
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
	sw	$8, counter_
	j	l2
l3: 
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


