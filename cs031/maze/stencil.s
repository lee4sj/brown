    .data
#
# The Object class serves only as a base for other classes
#################################
object_size=4                	# public class Object {
__object_vtbl:	.word	0:1    	# 
    	                        #	public Object();
object_vtbl_equals=0            #	public boolean equals(Object o);
				                # }
#################################
#
# The Room class represents a room in the maze
#################################
room_size = 8									#	public class Room{
roomID = 4
__Room_vtbl:	.word	0:5						

room_vtbl_equals = object_vtbl_equals			#	public boolean equals(Object other)
room_vtbl_isGoal = 4							#	bool isGoal()
room_vtbl_getNeighbors = 8						# 	void getNeighbors()
room_vtbl_isSearched = 12						#	bool isSearched()
room_vtbl_movePascalHere = 16					#	movePascalHere()
#################################
# 
# 
# syscall numbers: 
#################################
SYSCALL_INIT_MAZE=30            #
SYSCALL_DEFEAT=31               #
SYSCALL_VICTORY=32              #
SYSCALL_IS_GOAL=33              #
SYSCALL_GET_NEIGHBORS=34        #
SYSCALL_MOVE_PASCAL=35          #
SYSCALL_IS_SEARCHED=36          #
#################################
# 
# 
# Support code queue stuff. ignore this. 
#################################
qsize=512                       #
q:				.word 0:qsize   #
qhead:			.word	0       #
qtail:			.word	0       #
space:			.asciiz	" "     #
newline:		.asciiz	"\n"    #
emptyq_error:	.asciiz	"Tried to dequeue from an empty queue\n"
fullq_error:	.asciiz "The queue is full. You're enqueuing way too many nodes\n"
#################################
#
# 
# Memory management data. Change values here as needed.
#################################
heap:		.word 	0:2400      #
free_lst:	.word	0           #
MEM_SIZE 			= 	800		# number of memory nodes
NODESIZE      		=   12      # sizeof node in bytes
#################################
#
# 
# Needed for getting neighbors
#############################
buff:	.word	0:4

#############################################
	.text                                   #
#############################################
#
# vtbl_init
# fills the object and room vtbls with the appropriate values
#############################################
vtbl_init:                                  #
	sub	$sp, $sp, 4							# start:
	sw	$ra, 4($sp)							# push $ra

    la	$t0, __object_vtbl                  #
    la	$t1, object_equals                  #
    sw	$t1, object_vtbl_equals($t0)        # object_vtbl[0] = object_equals;

	la	$t0, __Room_vtbl					#
	la	$t1, room_equals					
	sw	$t1, room_vtbl_equals($t0)			# room_vtbl[0] = room_equals;
	
	la	$t1, room_is_goal
	sw	$t1, room_vtbl_isGoal($t0)			# room_vtble[1] = room_is_goal;
	
	la	$t1, room_get_neighbors
	sw	$t1, room_vtbl_getNeighbors($t0)	# room_vtbl[2] = room_get_neighbors;
	
	la	$t1, room_is_searched
	sw	$t1, room_vtbl_isSearched($t0)		# room_vtbl[3] = room_is_searched;
	
	la	$t1, room_move_pascal_here
	sw	$t1, room_vtbl_movePascalHere($t0)	# room_vtbl[4] = room_get_move_pascal_here
	

	lw	$ra, 4($sp)							# pop $ra
	add	$sp, $sp, 4	
    jr $ra                                  # end:
#############################################
#
# 
# init_object(pointer)
# initializes the contents of the pointer to be an object
############################################# 
init_object:								# void init_object(void * o){
	jr $ra									# }
#############################################
# 
# Object::equals  
# compares two objects by simply comparing their pointers
#############################################
object_equals:								# public boolean Object::equals(Object o){
	seq $v0, $a0, $a1						# 	return this == o;
	jr	$ra	    							# }
#############################################
# 
# init_room
# initializes the contents of the pointer to be a room with id roomID
# input:
# 		$a0 - void* v, the pointer to this room object
# 		$a1 - int roomID
# returns: <none>
#############################################
init_room:                                  # public void init_room(void* v,int roomID){
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;
	
	sw	$a1, roomID($a0)					# 	this.roomID = roomID;
	
	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
	jr $ra									# }
#############################################
#
# Room::equals
# compares two Rooms by comparing their roomID
# input:
# 		$a0 - void* v, the pointer to this room object
# 		$a1 - void* v2, the pointer to the room object to be compared
# returns:
# 		$v0 - 1 if two Rooms have same roomID, 0 otherwise;
# Registar Usage:
# 		$t1 - this.roomID
# 		$t2 - o.roomID
############################################# 
room_equals:								# public boolean Room::equals(Room o){
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;
		
	lw	$t1, roomID($a0)					# get this.roomID
	lw	$t2, roomID($a1)					# get o.roomID
	seq	$v0, $t1, $t2						# return this.roomID == o.roomID;

	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
	jr $ra									# }
############################################# 
# 
# Room::isGoal
# checks to see if this room is the goal
# input:
# 		$a0 - void* v, the pointer to this room object
# returns:
# 		$v0 - 0 if this room is not a goal room, 1 otherwise
# Register Usage:
############################################# 
room_is_goal:								# public boolean Room::isGoal();
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;

	lw	$a0, roomID($a0)
	li	$v0, SYSCALL_IS_GOAL
	syscall									# return syscall(IS_GOAL, this.roomID);

	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
    jr $ra									# }
#############################################
# 
# Room::getNeighbors
# returns the rooms's neighbors
# input:
# 		$a0 - void* v, the pointer to this room object
# 		$a1 - int[] buf, 
# returns: <none> (however, the input buf will now contain neighbors of this.room)
# Register Usage:
#############################################
room_get_neighbors:							# public void Room::getNeighbors(int[] buf);
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;

	lw	$a0, roomID($a0)
	li	$v0, SYSCALL_GET_NEIGHBORS
	syscall									# syscall(GET_NEIGHBORS, this.roomID, buf);

	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
    jr $ra									# }
#############################################
# 
# Room::isSearched
# check if this room has been searched already
# input:
# 		$a0 - void* v, the pointer to this room object
# returns: 
# 		$v0 - 1 if the room has already been visited via move_pascal, 0 otherwise
#############################################
room_is_searched:							# public boolean Room::isSearched();
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;
	
	lw	$a0, roomID($a0)
	li	$v0, SYSCALL_IS_SEARCHED
	syscall									# return syscall(IS_SEARCHED, this.roomID);

	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
    jr $ra									# }
#############################################
# 
# Room::movePascalHere
# moves Pascal to this room in the GUI
# input:
# 		$a0 - void* v, the pointer to this room object
# returns: <none>
#############################################
room_move_pascal_here:						# public void Room::movePascalHere(){
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;
	
	lw	$a0, roomID($a0)					
	li	$v0, SYSCALL_MOVE_PASCAL
	syscall									# syscall(MOVE_PASCAL, this.roomID);

	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
    jr	$ra									# }
#############################################
# 
# SUPPORT CODE QUEUE. DON'T WORRY ABOUT THIS
# enq
#############################################
enq:										# void enq(Room * item){
	lw $t0, qhead							#     
	li $t1, qsize                           #     if(qhead
	sub $t1, $t1, 1
	mul $t1, $t1, 4
	bne $t0,$t1, enq_not_full               #     if(qhead > (qsize-1)*4){
	la $a0, fullq_error                     # 		print fullq_error
	li $v0, 4                               #
	syscall                                 #
	jr $ra									# 		exit
enq_not_full:                               #     }
	sw $a0, q($t0)							#     queue[queue_head]=item;
	add $t0, $t0, 4							#     queue_head++;
	sw $t0, qhead							#
	jr $ra									# }
#############################################
# 
# deq
#############################################
deq:										# Room* deq(){
	lw $t0, qtail							#   
	lw $t1, qhead                           #
	bne $t0, $t1, deq_not_empty             # 	if(qtail == qhead){
	la $a0, emptyq_error                    # 		print emptyq_error
	li $v0, 4                               #
	syscall                                 #
	jr $ra									# 		exit
deq_not_empty:                              # 	}	
	add $t2, $t0, 4							#
	sw $t2, qtail							# 	queue_tail++
	lw $v0, q($t0)							#   return queue[queue_tail-1];
	jr $ra									# }
#############################################
# 
# qcontains
#############################################
qcontains:									# int qcontains(MyObject *other){
	sub	$sp, $sp, 20						#
    sw	$ra, 4($sp)							#
    sw	$s0, 8($sp)							#
    sw	$s1, 12($sp)						#
    sw	$s2, 16($sp)						#
    sw	$s3, 20($sp)						#
    move $s0, $a0							#
    jal emptyq								#
    move $t0, $v0                           #
    li $v0, 0                           	#
    bnez $t0, qcontains_exit				#	if(!emptyq()){
    lw $s1, qtail							#
    lw $s2, qhead							#
qcontains_loop:								# 		for (int i = queue_tail; i<queue_head; i++) {
	lw $t0, q($s1)							#
	lw $t1, 0($t0)							#
	lw $t1, object_vtbl_equals($t1)			#
	move $a0, $t0							#
	move $a1, $s0							#
	jalr $t1, $ra							#
	beqz $v0, qcontains_not_found			# 			if (queue[i]->equals(other)) {
	j qcontains_exit						# 				return 1;
qcontains_not_found:						#			}
	add $s1, $s1, 4							#
	xor $t0, $s2, $s1						#
	bnez $t0, qcontains_loop				# 		}
    										# 		return 0;
qcontains_exit:								#	}
	lw      $s3, 20($sp)					#
    lw      $s2, 16($sp)					#
    lw      $s1, 12($sp)					#
    lw      $s0, 8($sp)						#
    lw      $ra, 4($sp)						#
    add     $sp, $sp, 20					#
    jr      $ra								# }
#############################################
# 
# emptyq
#############################################
emptyq:										# int emptyq(){
	lw $t0, qhead							# 
	lw $t1,	qtail							# 
	seq $v0, $t0, $t1						#	return queue_head==queue_tail;
	jr $ra									# }
#############################################
# 
# printq
# prints the contents of the queue for debugging
#############################################
printq:										#
	sub	$sp, $sp, 20						#
    sw	$ra, 4($sp)							#
    sw	$s0, 8($sp)							#
    sw	$s1, 12($sp)						#
    sw	$s2, 16($sp)						#
    sw	$s3, 20($sp)						#
    move $s0, $a0							#
    lw $s1, qtail							#
    lw $s2, qhead							#
printq_loop:								# 	for (int i = qtail; i<qhead; i++) {
	lw $a0, q($s1)							#
	li $v0, 1								#
	syscall									#		print q[i];
	la $a0, space							#
	li $v0, 4								#
	syscall									#		print " ";
	add $s1, $s1, 4							#
	xor $t0, $s2, $s1						#
	bnez $t0, printq_loop					# 	}
	la $a0, newline							#
	li $v0, 4								#
	syscall									#		print "\n";
	lw      $s3, 20($sp)					#
    lw      $s2, 16($sp)					#
    lw      $s1, 12($sp)					#
    lw      $s0, 8($sp)						#
    lw      $ra, 4($sp)						#
    add     $sp, $sp, 20					#
    jr      $ra								# }
#############################################
# END SUPPORT CODE QUEUE
# 
# init_heap
# turns the heap into a linked list
# input: <none>
# returns: <none>
# Register Usage:
# 		$t0 - index
# 		$t1 - memSize
# 		$t2 - offset, offset for the heap address
# 		$t3 - nextNodeAdd, the address of the nextNode
# 		$t4 - temp
#############################################
init_heap:                                  # void init_heap(void){
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;
	
	li	$t0, 0								# index = 0;
	li	$t1, MEM_SIZE						# memSize = MEM_SIZE;
	sub	$t1, $t1, 1							# memSize--;
	
	la	$t4, heap							# temp = &heap;
	sw	$t4, free_lst						# free_lst = temp;

while1:
	bge	$t0, $t1, endW1						# while( index < memSize){
	mul $t2, $t0, NODESIZE					# offset = index * NODESIZE;
	add	$t2, $t2, room_size					# offset += room_size;
	
	add	$t3, $t0, 1							# nextNodeAdd = 1 + index;
	mul	$t3, $t3, NODESIZE					# nextNodeAdd *= NODESIZE;
	
	la	$t4, heap							# temp = &heap;
	add	$t3, $t3, $t4						# nextNodeAdd += temp;
	sw	$t3, heap($t2)						# heap.getNode(index).setNextNode(heap.getNode(index+1));
	
	add	$t0, $t0, 1							# index ++;
	j	while1								# }
endW1:

	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
	jr $ra									# }
#############################################
# 
# new_room
# take a node off the free list, set the VTBL, and return the room
# input: <none>
# returns:
# 		$v0 - Room* room, a pointer to a Room object
# Register Usage:
# 		$t1 - temp
#############################################
new_room:	                          		# Room new_room(){
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;
	
	lw	$v0, free_lst						# room = free_lst.pop();
	lw	$t1, room_size($v0)					# 
	sw	$t1, free_lst						#
	
	la	$t1, __Room_vtbl					#
	sw	$t1, 0($v0)							# room.vtbl = __Room_vtbl; //setVTBL for the room object


											# return room;
	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
	jr $ra                                  # }
#############################################
# 
# delete_room
# find the node for a Room object and add it back to the free list
# input:
# 		$a0 - Room* room, a pointer to the Room we are deallocating
# returns: <none>
# Register Usage:
# 		$t0 - temp
#############################################
delete_room:                              # void delete_room(Room r){
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;
	
											# //free_lst.push(room);
	lw	$t0, free_lst						# temp = free_lst;
	sw	$t0, room_size($a0)					# room.nextNode = temp;
	sw	$a0, free_lst						# free_lst = room;

	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
	jr $ra                                  # }
#############################################
# main
# runs the maze solver
# Register Usage:
# 		$s0 - room
# 		$s1 - 1
# 		$s2 - 4
# 		$s3 - index
# 		$s4 - temp
# 		$s5 - room2
# 		$s6 - temp2
#############################################
main:
#******Maze solver algorithm goes here******#
	sub	$sp, $sp, 4							#
	sw	$ra, 4($sp)							# push $ra;

	jal	init_heap							# init_heap();
	jal	vtbl_init							# init_vtbl();
	
	li	$v0, SYSCALL_INIT_MAZE
	syscall									# startID = init_maze();
	move $s4, $v0							# temp = startID;
	
	jal	new_room
	move $a0, $v0
	move $a1, $s4
	jal	init_room							# v0 = new Room(temp);
	
	move $a0, $v0
	jal enq									# enq(v0);
	
	li	$s1, 1								# $s1 = 1;
	li	$s2, 4								# $s2 = 4;


while2:
	jal	emptyq								#
	beq	$v0, $s1, endW2						# while(!emptyq()){
	jal	deq									# 	v0 = deq();
	move $s0, $v0							# 	room = v0;
	move $a0, $v0							
	jal	room_move_pascal_here				# 	room.movePascalHere();
	
	move $a0, $s0
	jal room_is_goal					
	move $s4, $v0							# 	temp = room.is_goal();
	
	if1:
	beqz $s4, endIf1						# 	if(temp){
	li	$v0, SYSCALL_VICTORY
	syscall									# 		victory();
	
		while3:
	jal	emptyq								#
	beq	$v0, $s1, endW3						# 		while(!emptyq()){
	jal	deq									
	move $s4, $v0							#			temp = deq();
	move $a0, $s4
	jal	delete_room							#			delete(temp);
	j	while3								#		}
		endW3:
	move $a0, $s0
	jal	delete_room							#	delete(room);
	j	halt								#	halt;
	endIf1:									#	}

	move $a0, $s0							
	la	$a1, buff							
	jal	room_get_neighbors					#	buff = room.getNeighbors();
	
	li	$s3, 0								# 	index = 0;
	for1:
	bge	$s3, $s2, endF1						#	while(index < 4){
	mul	$s4, $s3, 4							#		temp = index * 4;
	lw	$s4, buff($s4)						#		temp = buff[index];
		if2:
	beqz $s4, endIf2						#		if(temp){
	jal	new_room
	move $a0, $v0
	move $a1, $s4
	jal	init_room							#			v0 = new Room(temp);
	move $s5, $v0							#			room2 = v0;
	
			if3:
	move $a0, $s5
	jal	room_is_searched					
	move $s4, $v0							#			temp = room2.isSearched();
	
	move $a0, $s5
	jal	qcontains							#			
	move $s6, $v0							#			temp2 = qcontains(room2);
	
	beq	$s4, $s1, else3						#			if(!temp &&
	beq	$s6, $s1, else3						#			   !temp2)
	
	move $a0, $s5
	jal enq									# 				enq(room2);
	j	endIf3								#			}
			else3:							
	move $a0, $s5
	jal delete_room
			endIf3:
		endIf2:
	add	$s3, $s3, 1							#		index++;
	j	for1								#	}
	endF1:
	
	move $a0, $s0
	jal delete_room							# delete(room);
	j	while2								# }	
endW2:
	
	li	$v0, SYSCALL_DEFEAT
	syscall									# defeat();
	
halt:
	
	lw	$ra, 4($sp)							# pop $ra;
	add	$sp, $sp, 4
	jr $ra
