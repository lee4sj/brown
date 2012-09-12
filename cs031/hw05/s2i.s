		.data
thestring: 	.asciiz "2147483647"	#the string to convert
theint: 	.word 	0		#integer value of the string
enter:		.asciiz	"\n"
errMessage:	.asciiz "Wrong input! The string is not an integer!\n"

		.text
__start:
# String to Integer
# Convert string(thestring) to an integer(theint)
# Note: If the integer in the string is greater than the largerst word, (2^31)-1, then the program will overflow.
# $s0 - currChar, current character bit
# $s1 - curr, the index of the current bit in the string, starting from the left
# $s7 - tempInt, temporary holder for an integer used for conversion of the string to the integer

		la 	$a0,thestring		#print the original string
		li	$v0, 4
		syscall
		
		la 	$a0,enter			#print "\n"
		li	$v0, 4
		syscall

		li	$s1, 0				# curr := current index
		lb  $s0, thestring($s0)	# currChar := the byte of current index of the content of thestring (i.e. chars[curr])
		li 	$s2, 0				# $s2 := NULL
		li 	$s3, 48				# $s3 := 48
		li	$s4, 57				# $s4 := 57

while: 	beq $s0, $s2, end		# while( currChar != NULL){
								# 	//error checking: if the charater is not an integer
		blt $s0, $s3, err		# 	if( currChar < 48 ) print error message;
		bgt $s0, $s4, err		# 	else if( currChar > 57 ) print error message;
								# 	else{
								# 	//converting the char to int
		sub $s0, $s0, 48		# 	currChar := currChar - 48
		lw	$s7, theint			# 	tempInt := theint (current value)
		mul $s7, $s7, 10		# 	tempInt := 10*tempInt + currChar
		add $s7, $s7, $s0		# 
		sw	$s7, theint			# 	theint := tempInt
		
		add $s1, $s1, 1			# 	curr = curr + 1
		lb  $s0, thestring($s1) # 	currChar := chars[curr]}
		j	while				#}

err:	la	$a0, errMessage		#print error message
		li	$v0, 4
		j	print
		
end:	lw 	$a0, theint			#print the integer
		li	$v0, 1
		j	print
		
print: 	syscall
		done