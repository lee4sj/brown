RISC
SeungJi Lee (sl136)

README
***************************************************

***BUGS:
1) When converting 8-bit input of LIM instruction to 16-bit,
I accidently added filler bits to lower-order bits instead of higher-order bits.

***Control Wire: the numbers indicate the index of the bit that has to be 1. Other bits are 0.
1) ADD = 0, 1, 9
2) CMP = 2, 9
3) SUB = 1, 9
4) MOV = 3, 9
5) LIM = 4, 9
6) LW = 7, 8, 9
7) SW = 6, 8
8) JGZ = 5

	Each instruction has a unique control wire that must be 1 to enable the value to go through the bus(i.e. to enable the tri-state), and 2 through 7 are assigned to them.
	Also 9 represents a wire that will enable writing to registers. So this is a write control wire for registers.
	Also 8 is a sel wire for RAM.
	Ld wire for RAM is simply the same as a control wire assigned to LW instruction (7).

***Test Cases: (test image is riscTest.img in the risc folder)
LIM r0, -4			r0 = -4
LIM r1, 0			r1 = 0
LIM r3, 3			r3 = 3
LIM r5, -1			r5 = -1
LIM r6, 1			r6 = 1

ADD r0, r0, r1			r0 = -4
ADD r0, r5, r5			r0 = -2
ADD r0, r0, r5			r0 = -3
ADD r0, r0, r3			r0 = 0
ADD r0, r3, r6			r0 = 4

SUB r0, r5, r5			r0 = 0
SUB r0, r6, r5			r0 = 2
SUB r0, r6, r6			r0 = 0
SUB r0, r5, r6			r0 = -2

MOV r0, r3			r0 = 3
MOV r0, r0			r0 = 3

SW r6, r3			RAM(1) = 3
SW r0, r5			RAM(3) = -1
SW r0, r6			RAM(3) = 1

LW r0, r0			r0 = 1
LW r0, r6			r0 = 3

JGZ r5, r3			This should not be executed
JGZ r0, r3			same
LIM r0, 10			This should not be executed
LIM r0, 9			This should not be executed
LIM r0, 8			r0 = 8

CMP r0, r0, r3			r0 = 0
CMP r0, r1, r2			r0 = 1
CMP r0, r6, r3			r0 = 0
CMP r0, r6, r6			r0 = 1



	LIM instruction: negative, positive and zero values are tested.
	ADD instruction: pairs of (-, -), (+, +), (+, -) are tested.
	SUB instruction: pairs of (-, -), (+, +), (+, -) are tested.
	MOV instruction: from a register to a different register and to the same register are tested.
	SW instruction: overwriting and regular saving are tested.
	LW instruction: loading from the RAM is tested.
	JGZ instruction: unsatisfied jump(no jumping occurs) and satisfied jump(jumping occurs) are tested.
	CMP instruction: comparing two same values and different values are tested.




