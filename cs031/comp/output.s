.data

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

	li	$8, 2
	sub	$sp, $sp, 4
	sw	$8, 4($sp)
	li	$8, 1
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
	beqz	$8, l0
l0: 
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


