.data
a: .word 12

.text
main:
li	$s0, 3
sw	$s0, a
lw $a0, a
li $v0, 1
syscall
jr $ra