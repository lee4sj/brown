.data
a: .word 12

.text
main:
li $a0, 12
li $v0, 1
syscall
jr $ra