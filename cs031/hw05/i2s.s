		.data
int:	.word	2147483647	#the integer to convert
buffer: .byte	0:12		#the string buffer for the integer, 
							#the size of this array(each cell representing a digit in decimal) is strictly greater than the size of a word, 
							#thus will always hold 0 at the end.
enter:		.asciiz	"\n"

		.text
__start:		
# Integer to String
# convert an integer(int) to a String(buffer)
# $s0 -	currDigit, the value of the current digit
# $s1 - index, the index of the current digit
# $s7 - temp, temporary holder for an integer

		lw 	$a0,int				#print the integer
		li	$v0, 1
		syscall
		
		la 	$a0,enter			#print "\n"
		li	$v0, 4
		syscall

		lw	$s7, int			# temp := int
		li	$s1, 0				# index := current index
		li 	$s2, 0				# $s2 := NULL
		li 	$s3, 48				# $s3 := 48
		li	$s4, 10				# $s4 := 10
		
while1:	blt $s7, $s4, end1		# while(temp >= 10){
		add $s1, $s1, 1			# 	index++;
		div $s7, $s7, 10		# 	temp = temp/10
		j	while1				# }
end1:
		lw	$s7, int			# temp := int

while2:	blt $s1, $s2, end2		# while(index >= 0){
		rem $s0, $s7, 10		# 	currDigit = temp mod 10
								# 	//convert currDigit to String
		add	$s0, $s0, 48		# 	currDigit = currDigit + 48
		sb	$s0, buffer($s1)	# 	buffer[index] = currDigit
		
		div $s7, $s7, 10		# 	temp = temp / 10
		sub	$s1, $s1, 1			#	index--;
		j	while2				# }
end2:

	la	$a0, buffer
	li	$v0, 4
	syscall
	done