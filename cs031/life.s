####################################
#                                  #
#  life.s                          #
#  CS 31						   #
#  SeungJi Lee					   #
#  sl136	                       #
#                                  #
####################################



.data

    GENERATIONS         = 5         # number of generations to render
    ROWS                = 8         # width of the board (no. of cells)
    COLUMNS             = 10        # height of the board (no. of cells)
    ARRAY_SIZE          = 80        # area of board (must be ROWS * COLUMNS,
                                    # rounded up to a multiple of the word size)
                                    
row:		.word	ROWS
col:		.word	COLUMNS
arraySize:	.word	ARRAY_SIZE
generation:	.word	GENERATIONS
env1:		.byte	0:ARRAY_SIZE
env2:		.byte	0:ARRAY_SIZE
space:		.asciiz	" "
enter:		.asciiz	"\n"
begin:		.asciiz	"---\n"
end:		.asciiz	"===\n"

.text

# ****************************************************************************
# Subroutine: main
# Initializes the data structures and runs the main loop.
#
# Params: <none>
# Returns: <none>
# Register Usage:
# 		$s0 - row, row index
# 		$s1 - col, column index
# 		$s2 - ROWS
# 		$s3 - COLUMNS
# 		$s4 - addressCur, address of the current board
# 		$s5 - addressNext, address of the next board
# 		$s6 - genNum, number of generations
# 		$s7 - genIndex, generation index
# 		$t0 - tempAdd, temporary holder of calculated address and for swapping
# ****************************************************************************
main:
		la	$s4, env1				# addressCur = env1;
		la	$s5, env2				# addressNext = env2;
		move $a0, $s4				# add_cells_for_testing(addressCur); //initialize the env array
		jal add_cells_for_testing	# 
		
		lw	$a1, row
		lw	$a2, col
		jal	print2DArray			# print intial environment;		

		li	$s0, 0					# row = 0;
		li	$s1, 0					# col = 0;
		lw	$s2, row				# ROWS = Array_ROWS;
		lw	$s3, col				# COLS = Array_COLS;
		li	$s7, 0					# genIndex = 0;	
		lw	$s6, generation			# genNum = generations
		
fGen:	bge	$s7, $s6, endfG			# while( genIndex < genNum){
		li	$s0, 0					# row = 0;
	for1:	
		bge	$s0, $s2, endf1			# while( row < ROWS ){
		li	$s1, 0					#	col = 0;
		for2:
		bge	$s1, $s3, endf2			#	while( col < COLS ){
		move $a0, $s4				#		
		move $a1, $s1				#
		move $a2, $s0				#
		move $a3, $s5				#
		jal	getNextGen				#		v0 = getNextGen(addressCur, col, row, addressNext);
		add	$s1, $s1, 1				#		col++;
		j	for2					#	}
		endf2:
		add	$s0, $s0, 1				#	row++;
		j	for1					# }
	endf1:
		move $a0, $s5				#
		move $a1, $s2				#
		move $a2, $s3				#
		jal	print2DArray			#	print2DArray(arrayNext, ROWS, COLS);
		move $t0, $s4				#	tempAdd = addressCur;
		move $s4, $s5				#	addressCur = addressNext;
		move $s5, $t0				# 	addressNext = tempAdd;
		add	$s7, $s7, 1				#	genIndex++;
		j	fGen					# }
endfG:
		done

		



# ****************************************************************************
# Subroutine: add_cells_for_testing (board)
# Initializes the env array
# 
# Param:
# 		&a0 = address of the board
# 
# Returns: <none>
# Register Table:
# 		$s
# 
# ****************************************************************************
add_cells_for_testing:
    	sub	$sp, $sp, 4				# start:
		sw	$ra, 4($sp)				# push $ra
		
		li	$a1, 0
		li	$a2, 0
		jal add_live_cell			# add_live_cell(board, 0, 0)
		li	$a1, 1
		li	$a2, 0
		jal add_live_cell			# add_live_cell(board, 1, 0)
		li	$a1, 0
		li	$a2, 1
		jal add_live_cell			# add_live_cell(board, 0, 1)
		li	$a1, 0
		li	$a2, 2
		jal add_live_cell			# add_live_cell(board, 0, 2)
		li	$a1, 1
		li	$a2, 1
		jal add_live_cell			# add_live_cell(board, 1, 1)
    	
		lw	$ra, 4($sp)				# pop $ra
		add	$sp, $sp, 4				#
		jr	$ra						# end:


# ****************************************************************************
# Subroutine: add_live_cell (board, x, y)
# Sets the state of the cell at location (x,y) in the given board to "Live".
#
# Params:  
#          $a0 = address of the board
#          $a1 = x (0-based index)
#          $a2 = y (0-based index)
# Returns: <none>
# Register Usage:
# 		$t0 - temporary holder for addressing
# 		$t1 - col, Array_Col
# 		$t2 - 1
#
# ****************************************************************************
add_live_cell:
		sub	$sp, $sp, 4				# start:
		sw	$ra, 4($sp)				# push $ra
	
		lw	$t1, col				# col = Array_Col;
		li	$t2, 1					# $t2 = 1;
		mul	$t0, $a2, $t1			# temp = y*col;
		add	$t0, $t0, $a1			# temp += x;
		add $t0, $a0, $t0			# temp = addressBoard + temp;
		sb	$t2, ($t0)				# board[y][x] = 1;
	
		lw	$ra, 4($sp)				# pop $ra
		add	$sp, $sp, 4				#
		jr	$ra						# end:



# ****************************************************************************
# Subroutine: countNeighbors (board, x, y)
# Count the number of alive neighbor cells at the location (x, y) in the given board
# 
# Params:
# 			$a0 = address of the board
# 			$a1 = x (0-based index)
# 			$a2 = y (0-based index)
# 
# Reterns: 
# 			$v0 = integer, number of the neighbors of the cell located at (x, y)
# Register Usage:
# 		$s0 - counter, counter for neighbors
# 		$s1 - used for if statement, and for addressing(temp)
# 		$s2 - used for if statement, and for addressing(tempVal)
# 		$t0 - 0
# 		$t1 - rowIndex, index for row
# 		$t2 - colIndex, index for column
# 		$t3 - 1
# 		$t4 - row, to be looked up
# 		$t5 - col, to be looked up
# 		$t6 - arrayRow, number of rows in the array
# 		$t7 - arrayCol, number of cols in the array
# 		
# 
# ****************************************************************************
countNeighbors:
		sub	$sp, $sp, 16			# start:
		sw	$ra, 4($sp)				# push $ra
		sw	$s0, 8($sp)				# push $s0
		sw	$s1, 12($sp)			# push $s1
		sw	$s2, 16($sp)			# push $s2
		
		li	$s0, 0					# counter = 0;
		li	$t1, -1					# rowIndex = -1;
		li	$t2, -1					# colIndex = -1;
		li	$t3, 1					# $t3 = 1
		li	$t0, 0					# $t0 = 0
		lw	$t6, row				# arrayRow = ArrayRow
		lw	$t7, col				# arrayCol = ArrayCol
		
for3:	bgt	$t1, $t3, endf3			# while( rowIndex <= 1 ){
		add	$t4, $a2, $t1			#	row = y + rowIndex;
		li	$t2, -1					#	colIndex = -1;
	for4:
		bgt	$t2, $t3, endf4			#	while( colIndex <= 1 ){
		add	$t5, $a1, $t2			#	col = x + colIndex;
		if1:
		blt	$t4, $t0, endif1		#	if( row >= 0 &&
		blt	$t5, $t0, endif1		#		col >= 0 &&
		bge	$t4, $t6, endif1		#		row < arrayRow &&
		bge $t5, $t7, endif1		#		col < arrayCol &&
		seq	$s1, $t4, $a2			#
		seq	$s2, $t5, $a1			#
		add	$s1, $s1, $s2			#
		bgt	$s1, $t3, endif1		#		!(row == y && col == x)){
		
		mul $s1, $t4, $t7			#		temp = row * arrayCol;
		add	$s1, $s1, $t5			#		temp += col;
		add $s1, $s1, $a0			#		temp = addressBoard + temp;
		lb	$s2, ($s1)				#		tempVal = board[row][col];
		add	$s0, $s0, $s2			#		counter += tempVal;
		endif1:
		add	$t2, $t2, 1				#		colIndex++;
		j	for4					#	}
	endf4:
		add	$t1, $t1, 1				# 	rowIndex++;
		j	for3					# }
endf3:
		move $v0, $s0				# return counter;
	
		lw	$s2, 16($sp)			# pop $s2
		lw	$s1, 12($sp)			# pop $s1
		lw	$s0, 8($sp)				# pop $s0
		lw	$ra, 4($sp)				# pop $ra
		add	$sp, $sp, 16			#
		jr	$ra						# end:


# ****************************************************************************
# Subroutine: getNextGen (boardCurr, x, y, boardNext)
# Given the current-generation board, (x, y) coordinate, and the next-generation board,
#  decide whether the cell is alive or dead in the next generation, and store it to the location.
# 
# Params:
# 			$a0 = address of the currents board
# 			$a1 = x, column index
# 			$a2 = y, row index
# 			$a3 = address of the next generation board
# 
# Returns: <none>
# Register Usage:
# 			$s0 - x coordinate
# 			$s1 - y coordinate
# 			$s2 - current board address
# 			$s3 - next board address
# 			$t0 - temp, address index for looking up the cell located at (x, y) in the board
# 			$t1 - col, number of cols in the board
# 			$t2 - cellVal, value of the cell
# 			$t3 - 0
# 			$t4 - 1
# 			$t5 - 2
# 			$t6 - 3
# 			$t7 - number of neighbors
# ****************************************************************************
getNextGen:
		sub	$sp, $sp, 20			# start:
		sw	$ra, 4($sp)				# push $ra
		sw	$s0, 8($sp)				# push $s0
		sw	$s1, 12($sp)			# push $s1
		sw	$s2, 16($sp)			# push $s2
		sw	$s3, 20($sp)			# push $s3
		
		move $s0, $a1				# x = input_x;
		move $s1, $a2				# y = input_y;
		move $s2, $a0				# currBoard = input_currBoard;
		move $s3, $a3				# nexBoard = input_nextBoard;
		jal	countNeighbors			# v0 = countNeighbors(currBoard, x, y);
		move $t7, $v0				# numNeighbors = v0
		lw	$t1, col				# row = Array_Col;
		li	$t3, 0					# $t3 = 0;
		li	$t4, 1					# $t4 = 1;
		li	$t5, 2					# $t5 = 2;
		li	$t6, 3					# $t6 = 3;
		
		mul	$t0, $s1, $t1			# temp = y*col;
		add	$t0, $t0, $s0			# temp += x;
		add $t0, $t0, $s2			# temp += currBoard;
		lb	$t2, ($t0)				# cellVal = board[y][x];
		
		sub $t0, $t0, $s2			# temp -= currBoard;
		add $t0, $t0, $s3			# temp += nextBoard
		
gNGif1:	bne	$t2, $t3, egNGif1		# if(cellVal == 0){
	gNGif2:
		bne	$t7, $t6, else1			#	if(numNeighbors == 3)
		sb	$t4, ($t0)				#		board[y][x] = 1;
		j	egNGif2
		else1:						#	else
		sb	$t3, ($t0)				#		board[y][x] = 0;
	egNGif2:
		j	egNGif3					# }
egNGif1:
gNGif3:	bne	$t2, $t4, egNGif3		# else if(cellVal == 1){
	gNGif4:
		blt	$t7, $t5, do1			#	if( numNeighbors < 2 ||
		bgt	$t7, $t6, do1			#		numNeighbors > 3)
		j	else2
		do1:
		sb	$t3, ($t0)				#		board[y][x] = 0;
		j	egNGif4
		else2:						#	else
		sb	$t4, ($t0)				#		board[y][x] = 1;
	egNGif4:
egNGif3:							# }

		lw	$s3, 20($sp)			# pop $s3
		lw	$s2, 16($sp)			# pop $s2
		lw	$s1, 12($sp)			# pop $s1
		lw	$s0, 8($sp)				# pop $s0
		lw	$ra, 4($sp)				# pop $ra
		add	$sp, $sp, 20			#
		jr	$ra						# end:


# ****************************************************************************
# Subroutine: print2DArray(array, row, col)
# Given a 2-dimensional array and number of rows and columns, 
# print2DArray prints the elements of the array
# 
# Params:
# 			$a0 = address of the array
# 			$a1 = number of rows
# 			$a2 = number of columns
# 
# Returns: <none>
# Register Usage:
# 		$t0 - rowIndex
# 		$t1 - colIndex
# 		$t2 - temporary holder for addressing into the array
# 		$t3 - val, the value of the array at rowIndex and colIndex
# 		$t4 - temp2, temporary holder for deciding whether to put "\n"
# 		$s0 - array, address of the array
# 		$s1 - row, number of rows
# 		$s2 - col, number of cols
# 
# ****************************************************************************
print2DArray:

		sub	$sp, $sp, 16			# start:
		sw	$ra, 4($sp)				# push $ra
		sw	$s0, 8($sp)				# push $s0
		sw	$s1, 12($sp)			# push $s1
		sw	$s2, 16($sp)			# push $s2
		
		li	$t0, 0					# rowIndex = 0;
		li	$t1, 0					# colIndex = 0;
		move $s0, $a0				# array = input_array;
		move $s1, $a1				# row = input_row;
		move $s2, $a2				# col = input_col;
		
		la	$a0, begin				#
		li	$v0, 4
		syscall						# print "---"
		
prfor1:	bge	$t0, $s1, eprfor1		# while(rowIndex < Array_Row){
		li	$t1, 0					# colIndex = 0;
prfor2:	bge	$t1, $s2, eprfor2		#	while(colIndex < Array_Col){
		mul	$t2, $t0, $s2			#		temp = rowIndex * Array_Col;
		add	$t2, $t2, $t1			#		temp += colIndex;
		add	$t2, $t2, $s0			#		temp += addressArray;
		
		
		lb 	$a0, ($t2)				#		val = array[rowIndex][colIndex];
		li	$v0, 1
		syscall						#		print val;
		
		la	$a0, space				
		li	$v0, 4
		syscall						#	print ("\t");
		
		rem	$t4, $t1, $s2			#		temp2 = colIndex mod Array_Col;
		add	$t4, $t4, 1				#		temp2++;
		
	prif1:
		bne	$t4, $s2, eprif1		#		if(temp2 == Array_Col){
		la	$a0, enter
		li	$v0, 4
		syscall						#			print line;
	eprif1:							#		}
		add	$t1, $t1, 1				#		colIndex++;
		j	prfor2					#	}
eprfor2:
		add	$t0, $t0, 1				#	rowIndex++;
		j	prfor1					# }
eprfor1:

		la	$a0, end				#
		li	$v0, 4					#
		syscall						# print "==="

		lw	$s2, 16($sp)			# pop $s2
		lw	$s1, 12($sp)			# pop $s1
		lw	$s0, 8($sp)				# pop $s0
		lw	$ra, 4($sp)				# pop $ra
		add	$sp, $sp, 16			#
		jr	$ra						# end:
		