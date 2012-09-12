.data
a: .word 12

.text
main:
lw $a0, a
li $v0, 1
syscall
jr $ra