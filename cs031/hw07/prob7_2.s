		.data
row:		.word	9
col:		.word	9
size:		.word	81
array:		.word	0:81
enter:		.ascii	"\n"
space:		.ascii	"\t"


		.text
# Homework 7.2 

__start:
# Problem 7.2 (a)
# $s0 - counter
# $s1 - size
# $s2 - temporary holder for a returned value
# $s3 - col
# $t0 - address, temporary holder for address indexed at the counter


		li	$s0, 0				# counter = 0;
		lw	$s1, size			# size = arraySize;
		lw	$s3, col			# col = arrayCol;

for2:	bge	$s0, $s1, endF2		# while(counter < size){
		rem $a0, $s0, $s3		#	input1 = counter mod col;
		sub $a2, $s0, $a0		#	input3 = counter - input1;
		div	$a2, $a2, $s3		#	input3 = input3/col;
		add $a2, $a2, 1			#	input3++;
		add	$a0, $a0, 1			#	input1++;
		move $a1, $a0			#	input2 = input1;
		jal mulp				# 	v = mulp(input1, input2,input3);
		move $s2, $v0			#	temp = v;
		move $a0, $s0			#	input1 = counter;
		move $a1, $s0			#	input2 = counter;
		li	$a2, 4				#	input3 = 4;
		jal mulp				#	v = mulp(input1,input2,input3);
		move $t0, $v0			#	address = v; //counter*4
		sw	$s2, array($t0)		#	array[address] = temp;
		add $s0, $s0, 1			# 	counter++;
		j	for2				#}
endF2:
		jal printArray
		done



# Problem 7.2 (a)
# mulp: takes 3 parameters, $a0, $a1, $a2
# 		and returns $a0 * $a2 in $v0
# $a0 - number to added to
# $a1 - number to add
# $a2 - number of times to add
# $t1 - a temporary holder of 1;
# returns $v0
mulp:	
		sub	$sp, $sp, 4
		sw	$ra, 4($sp)		#push ra
		li	$t0, 1			# temp = 1
		
		ble $a2,$t0, endif1	# if ( $a2 > 1)
		sub	$a2, $a2, 1		#	$a2--;
		add	$a0, $a0, $a1	#	$a0 += $a1;
		jal	mulp			#	mulp($a0, $a1, $a2);
endif1:
		move $v0, $a0
		lw	$ra, 4($sp)		#pop ra
		add	$sp, $sp, 4
		jr	$ra				#jump back		
		


# Problem 7.2 (b)
# printArray
# prints the array stored at array
# 
# $s0 - counter
# $s1 - size, the size of the array
# $s2 - col
# $s3 - tmep, temporary holder for a returned value
# $t0 - address, temporary holder for address indexed at the counter
# $t1 - temp, used for finding whether it is the end of the row
printArray:
		sub	$sp, $sp, 20
		sw	$ra, 20($sp)		# push ra
		sw	$s3, 16($sp)		# save $s3
		sw	$s2, 12($sp)		# save $s2
		sw	$s1, 8($sp)			# save $s1
		sw	$s0, 4($sp)			# save $s0
				
		
		li	$s0, 0				# couter = 0;
		lw	$s1, size			# size = arraySize;
		lw	$s2, col			# col = arrayCol;

for:	bge	$s0, $s1, endF1		# while(counter < size)
		rem	$t1, $s0, $s2		#	temp = counter mod col

		bnez $t1, endif2		# 	if(temp == 0)
		la	$a0, enter			#		print("\n")
		li	$v0, 4
		syscall
endif2:	
		move $s3, $v0			#	temp = v;
		move $a0, $s0			#	input1 = counter;
		move $a1, $s0			#	input2 = counter;
		li	$a2, 4				#	input3 = 4;
		jal mulp				#	v = mulp(input1,input2,input3);
		move $t0, $v0			#	address = v; //counter*4
		lw	$a0, array($t0)		#	print(array[counter]);
		li	$v0, 1
		syscall
		la	$a0, space			#	print ("\t");
		li	$v0, 4
		syscall

		add	$s0, $s0, 1			# counter++;
		j	for					# }
endF1:	
		lw	$s0, 4($sp)			# restore $s0
		lw	$s1, 8($sp)			# restore $s1
		lw	$s2, 12($sp)		# restore $s2
		sw	$s3, 16($sp)		# restore $s3
		lw	$ra, 20($sp)		# pop ra
		add	$sp, $sp, 20
		jr	$ra					# jump back!






	

