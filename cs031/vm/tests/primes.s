                .data
                NBR_PRIMES = 4096
primes:         .word   0:NBR_PRIMES 
newline:        .asciiz "\n"            # Newline character, for convenience

                .text
main:
# Register usage:
#  $s0 - primes_left (number of primes left to find - starts at NBR_PRIMES and decrements until zero)
#  $s1 - n (next number to try - starts at 2 and increments by 1)
#  $s2 - i (loop counter)
#  $s3 - prime (pointer into primes array)
#################################################
        sub     $sp,    $sp,    20      	#
        sw      $ra,    20($sp)          	#
        sw      $s3,    16($sp)          	#
        sw      $s2,    12($sp)          	#
        sw      $s1,    8($sp)          	#
        sw      $s0,    4($sp)          	#
                                        	#
        li      $s0,    NBR_PRIMES              # int primes_left = NBR_PRIMES;
	li	$s1,	2			# int n = 2;
                                        	#
outer_begin:					#
	beqz	$s0,	outer_end		# while (primes_left > 0) {
	li	$s2,	NBR_PRIMES		#	int i = NBR_PRIMES;
	la	$s3,	primes			#	int* prime = primes;
inner_begin:					#
	beqz	$s2,	inner_end		# 	while (i > 0) {
	lw	$t0,	($s3)			#		int p = *prime;
	bnez	$t0,	ifelse			#		if (p == 0) {	// New prime found
	sw	$s1,	($s3)			#			*prime = n;
	sub	$s0,	$s0,	1		#			--primes_left;
						#
	j	inner_end			#			break;
ifelse:						#		}
	rem	$t1,	$s1,	$t0		#
	bnez	$t1,	if_end			#		else if (n % p == 0) {
						#			// n is divisible by this prime => n can't be prime
	j	inner_end			#			break;
if_end:						#		}
	add	$s3,	$s3,	4		#		++prime;
	sub	$s2,	$s2,	1		#		--i;
	j	inner_begin			#
inner_end:					#	}
	add	$s1,	$s1,	1		#	++n;
	j	outer_begin			#
outer_end:					# }

        move	$a0,    $s1			#
        li      $v0,    1               	#
        syscall                         	#			print_int(n);
						#
        la      $a0,    newline         	#
        li      $v0,    4               	#
        syscall                         	#			print_string("\n");

        lw      $s0,    4($sp)          	#
        lw      $s1,    8($sp)          	#
        lw      $s2,    12($sp)          	#
        lw      $s3,    16($sp)          	#
        lw      $ra,    20($sp)          	#
        add     $sp,    $sp,    20      	#
        jr      $ra                     	# return;

