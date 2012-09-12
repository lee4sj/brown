# ************************************************************
# PROG: Homework 8.1
# NAME: SeungJi Lee
# ACCT: 
# DATE: 11/2/10
# 
# 
# ************************************************************
.data

#The offset of the VTBL in an object
VTBL_POINTER_OFFSET  = 0 
#The offset of the favorite number variable in an object
FAVNUM_OFFSET = 4
#The total size of the object
OBJECT_SIZE = 8

# Each method should be found at the following
# offset in the VTBL
OFFSET_getFavoriteNumber = 0
OFFSET_tellJoke = 4
OFFSET_tellPunchline = 8
OFFSET_tellWhatChanges = 12
OFFSET_tellWhatChangesNot = 16


#Test objects
test_haxor: .byte 0:OBJECT_SIZE
test_belgian_haxor: .byte 0:OBJECT_SIZE
test_theorist: .byte 0:OBJECT_SIZE

#Strings that will be printed
haxor_joke: .asciiz "Why do computer scientists get Halloween and Christmas confused?\n"
haxor_punchline: .asciiz "Because oct 31 = dec 25\n"
belgian_haxor_joke: .asciiz "Who is following me?\n"
belgian_haxor_punchline: .asciiz "Oh gosh...\n"
theorist_joke: .asciiz "What did NP say to the circuit-sat?\n"
theorist_punchline: .asciiz "You complete me.\n"
uhramm: .asciiz "uhramm\n"
uhromm: .asciiz "uhromm\n"

#Declare your vtbls here
# YOUR CODE HERE
theorist_vtbl: .word 0:3
haxor_vtbl: .word 0:5
belgianhaxor_vtbl: .word 0:5


.text

#The following code tests your VTBLs. DO NOT MODIFY IT.
# also, this is intentionally obfuscated, and thus 
#     not a model of good MIPS
main:

sub $sp, $sp, 8
sw $ra, 4($sp)
sw $s0, 8($sp)

jal vtbl_init

la $a0, test_theorist
jal new_theorist
la $a0, test_theorist        
jal init_theorist
la $s0, test_theorist
add $s0, $s0, VTBL_POINTER_OFFSET

lw $t0, ($s0)
add $t0, $t0, OFFSET_getFavoriteNumber
lw $t1, ($t0)
la $a0, test_theorist
jalr $t1, $ra

move $a0, $v0
li $v0, 1
syscall

li $a0, 10
li $v0, 11
syscall


lw $t0, ($s0)
add $t0, $t0, OFFSET_tellJoke
lw $t1, ($t0)
la $a0, test_theorist
jalr $t1, $ra

lw $t0, ($s0)
add $t0, $t0, OFFSET_tellPunchline
lw $t1, ($t0)
la $a0, test_theorist
jalr $t1, $ra

la $a0, test_haxor
jal new_haxor
la $a0, test_haxor
jal init_haxor
la $s0, test_haxor
add $s0, $s0, VTBL_POINTER_OFFSET

lw $t0, ($s0)
add $t0, $t0, OFFSET_getFavoriteNumber
lw $t1, ($t0)
la $a0, test_haxor
jalr $t1, $ra
move $a0, $v0
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

lw $t0, ($s0)
add $t0, $t0, OFFSET_tellJoke
lw $t1, ($t0)
la $a0, test_haxor
jalr $t1, $ra

lw $t0, ($s0)
add $t0, $t0, OFFSET_tellPunchline
lw $t1, ($t0)
la $a0, test_haxor
jalr $t1, $ra

lw $t0, ($s0)
add $t0, $t0, OFFSET_tellWhatChanges
lw $t1, ($t0)
la $a0, test_haxor
jalr $t1, $ra

lw $t0, ($s0)
add $t0, $t0, OFFSET_tellWhatChangesNot
lw $t1, ($t0)
la $a0, test_haxor
jalr $t1, $ra

la $a0, test_belgian_haxor
jal new_belgian_haxor
la $a0, test_belgian_haxor
jal init_belgian_haxor
la $s0, test_belgian_haxor
add $s0, $s0, VTBL_POINTER_OFFSET

lw $t0, ($s0)
add $t0, $t0, OFFSET_getFavoriteNumber
lw $t1, ($t0)
la $a0, test_belgian_haxor
jalr $t1, $ra
move $a0, $v0
li $v0, 1
syscall
li $a0, 10
li $v0, 11
syscall

lw $t0, ($s0)
add $t0, $t0, OFFSET_tellJoke
lw $t1, ($t0)
la $a0, test_belgian_haxor
jalr $t1, $ra

lw $t0, ($s0)
add $t0, $t0, OFFSET_tellPunchline
lw $t1, ($t0)
la $a0, test_belgian_haxor
jalr $t1, $ra

lw $t0, ($s0)
add $t0, $t0, OFFSET_tellWhatChanges
lw $t1, ($t0)
la $a0, test_belgian_haxor
jalr $t1, $ra

lw $t0, ($s0)
add $t0, $t0, OFFSET_tellWhatChangesNot
lw $t1, ($t0)
la $a0, test_belgian_haxor
jalr $t1, $ra

lw $s0, 8($sp)
lw $ra, 4($sp)
jr $ra

# The next eight subroutines take care of printing out text.
# Your VTBLs will contain pointers to these subroutines.
# Again, you will NOT be modifying these methods.

say_uhramm:
li $v0, 4
la $a0, uhramm
syscall
jr $ra

say_uhromm:
li $v0, 4
la $a0, uhromm
syscall
jr $ra

say_haxor_joke:
li $v0, 4
la $a0, haxor_joke
syscall
jr $ra


say_haxor_punchline:
li $v0, 4
la $a0, haxor_punchline
syscall
jr $ra

say_theorist_joke:
li $v0, 4
la $a0, theorist_joke
syscall
jr $ra

say_theorist_punchline:
li $v0, 4
la $a0, theorist_punchline
syscall
jr $ra

say_belgian_haxor_punchline:
li $v0, 4
la $a0, belgian_haxor_punchline
syscall
jr $ra

say_belgian_haxor_joke:
li $v0, 4
la $a0, belgian_haxor_joke
syscall
jr $ra

# End printing subroutines

# Below is what you should fill in

# ****************************************************************************
# Method: cs_get_favorite_number
#   Returns the ComputerScientist object's favorite number.
#
# Params:  $a0 - a pointer to the receiving object (aka this)
# Returns: $v0 - the computer scientist's favorite number
#
# Register Usage:
#   
#
# ****************************************************************************
cs_get_favorite_number:
										#start:
		lw	$v0, FAVNUM_OFFSET($a0)		#return this._favoriteNumber;
		jr	$ra							#end:
		

# ****************************************************************************
# Subroutine: vtbl_init
#   Initializes all the vtbl's
#
# Params:  <none>
# Returns: <none>
#
# Register Usage:
#   	$t0 - temporary holder for address of the subroutine
#		$t1 - theorist, theorist_vtbl
# 		$t2 - haxor, haxor_vtbl
# 		$t3 - belgianhaxor, belgianhaxor_vtbl
# ****************************************************************************
vtbl_init:
												# start:
		la	$t1, theorist_vtbl					# theorist = theorist_vtbl
		la	$t2, haxor_vtbl						# haxor = haxor_vtbl	
		la	$t3, belgianhaxor_vtbl				# belgianhaxor = belgianhaxor_vtbl

		la	$t0, cs_get_favorite_number			# temp = address to cs_get_favorite_number
		sw 	$t0, OFFSET_getFavoriteNumber($t1)	# &(theorist.getFavoriteNumber) = temp
		sw	$t0, OFFSET_getFavoriteNumber($t2)	# &(haxor.getFavoriteNumber) = temp
		sw	$t0, OFFSET_getFavoriteNumber($t3)	# &(belgianhaxor.getFavoriteNumber) = temp
		
		la	$t0, say_theorist_joke				# temp = address to say_theorist_joke
		sw	$t0, OFFSET_tellJoke($t1)			# &(theorist.tellJoke) = temp
		la	$t0, say_haxor_joke					# temp = address to say_haxor_joke
		sw	$t0, OFFSET_tellJoke($t2)			# &(haxor.tellJoke) = temp\		
		la	$t0, say_belgian_haxor_joke			# temp = address to say_belgian_haxor_joke
		sw	$t0, OFFSET_tellJoke($t3)			# &(belgianhaxor.tellJoke) = temp
		
		la	$t0, say_theorist_punchline			# temp = address to say_theorist_punchline
		sw	$t0, OFFSET_tellPunchline($t1)		# &(theorist.tellPunchline) = temp
		la	$t0, say_haxor_punchline			# temp = address to say_haxor_punchline
		sw	$t0, OFFSET_tellPunchline($t2)		# &(haxor.tellPunchline) = temp
		la	$t0, say_belgian_haxor_punchline	# temp = address to say_belgian_haxor_punchline
		sw	$t0, OFFSET_tellPunchline($t3)		# &(belgianhaxor.tellPunchline) = temp
		
		la	$t0, say_uhramm						# temp = address to say_uhramm
		sw	$t0, OFFSET_tellWhatChanges($t2)	# &(haxor.tellWhatChanges) = temp
		
		la	$t0, say_uhromm						# temp = address to say_uhromm
		sw	$t0, OFFSET_tellWhatChangesNot($t2)	# &(haxor.tellWhatChangesNot) = temp
		sw	$t0, OFFSET_tellWhatChanges($t3)	# &(belgianhaxor.tellWhatChanges) = temp
		sw	$t0, OFFSET_tellWhatChangesNot($t3)	# &(belgianhaxor.tellWhatChangesNot) = temp
		
		jr	$ra									# end:


# ****************************************************************************
# Method: new_theorist
#   Construct the given object by filling in the vtbls
#
# Params:  $a0 - a pointer to the object to construct
# Returns: <none>
#
# Register Usage:
#   	$t0 - temp, temporary holder for address
#
# ****************************************************************************
new_theorist:
											# start:
		la	$t0, theorist_vtbl				# temp = &(theorist_vtbl);
		sw	$t0, VTBL_POINTER_OFFSET($a0)	# this.vtbl_pointer = temp;
		jr	$ra								# end:
        
        
# ****************************************************************************
# Method: new_haxor
#   Construct the given object by filling in the vtbls
#
# Params:  $a0 - a pointer to the object to construct
# Returns: <none>
#
# Register Usage:
#   	$t0 - temp, temporary holder for address
#
# ****************************************************************************
new_haxor:
											# start:
		la	$t0, haxor_vtbl					# temp = $(haxor_vtbl);
		sw	$t0, VTBL_POINTER_OFFSET($a0)	# this.vtbl_pointer = temp;
		jr	$ra								# end:
        
        
# ****************************************************************************
# Method: new_belgian_haxor
#   Construct the given object by filling in the vtbls
#
# Params:  $a0 - a pointer to the object to construct
# Returns: <none>
#
# Register Usage:
#   	$t0 - temp, temporary holder for address
#
# ****************************************************************************
new_belgian_haxor:
		# start:
		la	$t0, belgianhaxor_vtbl			# temp = $(belgianhaxor_vtbl);
		sw	$t0, VTBL_POINTER_OFFSET($a0)	# this.vtbl_pointer = temp;
		jr	$ra								# end:
        
        
# ****************************************************************************
# Method: init_computer_scientist
#   Initializes a new ComputerScientist instance, then tells a joke and 
# 	its punchline.
#
# Params:  $a0 - a pointer to the object to initialize 
# 		   $a1 - favorite number
# Returns: <none>
#
# Register Usage:
#   		$t0 - temp, temporary holder
# 			$s0 - a pointer to the object to initialize
#
# ****************************************************************************
init_computer_scientist:
		sub	$sp, $sp, 8						# start:
		sw	$ra, 4($sp)						# push $ra
		sw	$s0, 8($sp)						# push $s0

		sw	$a1, FAVNUM_OFFSET($a0)			# this.favoriteNumber = favorite number;
		move $s0, $a0						# $(this) = $(input_pointer);

		lw	$t0, VTBL_POINTER_OFFSET($s0)	# temp = this.vtbl_pointer;
		lw	$t0, OFFSET_tellJoke($t0)		# temp = &(temp.tellJoke());
		jalr $t0, $ra						# this.tellJoke();
		
		lw	$t0, VTBL_POINTER_OFFSET($s0)	# temp = this.vtbl_pointer;
		lw	$t0, OFFSET_tellPunchline($t0)	# temp = &(temp.tellPunchline());
		jalr $t0, $ra						# this.tellPunchline();
		
		lw	$s0, 8($sp)						# pop $s0
		lw	$ra, 4($sp)						# pop $ra
		add	$sp, $sp, 8
		jr	$ra								# end:
		


# ****************************************************************************
# Method: init_theorist
#   Initializes the given Theorist object
#
# Params:  $a0 - a pointer to the object to initialize
# Returns: <none>
#
# Register Usage:
#   	
#
# ****************************************************************************
init_theorist:
		sub	$sp, $sp, 4						# start:
		sw	$ra, 4($sp)						# push $ra
		
		li	$a1, 51
		jal	init_computer_scientist			# super(51);

		lw	$ra, 4($sp)						# pop $ra
		add	$sp, $sp, 4
		jr	$ra								# end:

# ****************************************************************************
# Method: init_haxor
#   Initializes the given Haxor object
#
# Params:  $a0 - a pointer to the object to initialize
# Returns: <none>
#
# Register Usage:
#   	$t0 - temp, temporary holder
# 		$s0 - pointer to the object to initialize
#
# ****************************************************************************
init_haxor:
		sub	$sp, $sp, 4						# start:
		sw	$ra, 4($sp)						# push $ra
		
		li	$a1, 31337
		jal	init_computer_scientist			# super(31337);

		lw	$ra, 4($sp)						# pop $ra
		add	$sp, $sp, 4
		jr	$ra								# end:


# ****************************************************************************
# Method: init_belgian_haxor
#   Initializes the given BelgianHaxor object
#
# Params:  $a0 - a pointer to the object to initialize
# Returns: <none>
#
# Register Usage:
# 		
#
# ****************************************************************************
init_belgian_haxor:
		sub	$sp, $sp, 4						# start:
		sw	$ra, 4($sp)						# push $ra
		
		jal	init_haxor						# super();

		lw	$ra, 4($sp)						# pop $ra
		add	$sp, $sp, 4
		jr	$ra								# end:


