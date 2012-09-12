package comp;

import java.io.*;

/**
 * This class encapsulates the process of writing MIPS instructions to an output
 * string. Once this string is created, you may output the string to either a
 * file or the java console (stdout). <p>
 * 
 * To write the string to a file, use {@link #writeToFile(String)}. To write it
 * to the console, use {@link #writeToConsole()}. <p>
 * 
 * Each <code>gen...</code> method corresponds to writing exactly one
 * instruction to the output string. The arguments to the methods specify the
 * variable parts of the instructions. (The one-instruction rule is relaxed for
 * the {@link #genOutput()} instruction, which also writes the text "syscall" and then
 * writes three instructions that serve to print a newline after the output
 * integer.)
 *
 * @version Fall 2007
 * @author The TAs
 */
public class MIPSCodeGenerator {
	private String _codeString;
	private int _label;
    
	/** 
	 * Create an instance of <code>MIPSCodeGenerator</code>.
	 */
	public MIPSCodeGenerator() {
		_label = 0;
		_codeString = new String();
	}
    
	/**
	 * Returns a new unique label.
	 *
	 * @return A jump label.
	 */
	public int getNextLabel() {
		return _label++;
	}
    
	/**
	 * Translates from a Register to its register number.
	 */
	private int getRegNumber(Register reg) {
		switch(reg) {
		case V0: return 2;
		case V1: return 3;
            
		case A0: return 4;
		case A1: return 5;
		case A2: return 6;
		case A3: return 7;
            
		case T0: return 8;
		case T1: return 9;
		case T2: return 10;
		case T3: return 11;
		case T4: return 12;
		case T5: return 13;
		case T6: return 14;
		case T7: return 15;
		case T8: return 24;
		case T9: return 25;
            
		case S0: return 16;
		case S1: return 17;
		case S2: return 18;
		case S3: return 19;
		case S4: return 20;
		case S5: return 21;
		case S6: return 22;
		case S7: return 23;
		case S8: return 30;
            
		case SP: return 29;
		case RA: return 31;
		}
		return -1;
	}
    
	/** 
	 * Write a <code>print-int</code> syscall. You must move the argument into <code>$a0</code>;
	 * this method doesn't do it for you.
	 */
	public void genOutput()
	{
		_codeString +=
  		"\tli\t$v0, 1\n" +
  		"\tsyscall\n" +
  		"\tli\t$v0, 11\n" +
  		"\tli\t$a0, 10\n" +
  		"\tsyscall\n";
	}
	 
	/** 
	 * Write a read-int syscall. Puts the read integer into <code>$v0</code>.
	 */
	public void genInput()
	{
		_codeString +=
  		"\tli\t$v0, 5\n" +
  		"\tsyscall\n";
	}
	 
	/** 
	 * Write <code>.data</code> line.
	 */
	public void genData()
	{
		_codeString += ".data\n\n";
	}
	 
	/** 
	 * Write <code>.text</code> line.
	 */
	public void genText()
	{
		_codeString += 	
  		"\n.text\n\n";
	}
	 
	/** 
	 * Write <code>main:</code> line, as well as code to save registers on the stack
	 */
	public void genMain()
	{
		_codeString += 	
  		"main:\n" +
		"\tsub $sp, $sp, 40\n" +
		"\tsw  $ra, 40($sp)\n" +
		"\tsw  $s0, 36($sp)\n" +
		"\tsw  $s1, 32($sp)\n" +
		"\tsw  $s2, 28($sp)\n" +
		"\tsw  $s3, 24($sp)\n" +
		"\tsw  $s4, 20($sp)\n" +
		"\tsw  $s5, 16($sp)\n" +
		"\tsw  $s6, 12($sp)\n" +
		"\tsw  $s7, 8($sp)\n" +
		"\tsw  $s8, 4($sp)\n\n";
	}
	 
	/** 
	 * Write code to return from main (restores registers and jumps to $ra)
	 */
	public void genReturnFromMain()
	{
		_codeString += 	
		"\tlw  $s8, 4($sp)\n" +
		"\tlw  $s7, 8($sp)\n" +
		"\tlw  $s6, 12($sp)\n" +
		"\tlw  $s5, 16($sp)\n" +
		"\tlw  $s4, 20($sp)\n" +
		"\tlw  $s3, 24($sp)\n" +
		"\tlw  $s2, 28($sp)\n" +
		"\tlw  $s1, 32($sp)\n" +
		"\tlw  $s0, 36($sp)\n" +
		"\tlw  $ra, 40($sp)\n" +
		"\tadd $sp, $sp, 40\n" +
		"\tjr $ra\n\n";
	}
	 
	/** 
	 * Write a label directive, with the letter L in front (so passing in
	 * 2 generates this line <code>
	 * l2:
	 * </code>
	 * 
	 * @param label An integer that distinguishes the label from others in the
	 * program.
	 */
	public void genLabel( int label )
	{
		_codeString += 	
  		"l" +
  		label +
  		": \n";
	}
	 
	/** 
	 * Write a <code>.word</code> directive that can be used to store a
	 * variable's value. Tack an underscore onto the end of the identifier so
	 * that variable names like "<code>j</code>" or "<code>bge</code>" won't be
	 * interpreted as instructions.
	 * 
	 * @param id Label for the word declaration.
	 */
	public void genDecl( String id )
	{
		_codeString +=
  		id + "_" +
  		":\t.word 0\n";
	}
	 
	/** 
	 * Write a .word directive that can be used to store an array of
	 * values. Tack an underscore onto the end of the identifier so that
	 * variable names like "<code>j</code>" or "<code>bge</code>" won't be
	 * interpreted as instructions.
	 * 
	 * @param size Number of words in the array.
	 * @param id Label for the array.
	 */
	public void genArrayDecl( int size, String id )
	{
		_codeString += 	
  		id + "_" +
  		":\t.word\t0:" +
  		( size ) + "\n";
	}

	/** 
	 * Pushes the contents of the register to the stack.
	 * 
	 * @param reg Register whose value should be pushed.
	 */
	public void genPush(Register reg)
	{
		_codeString +=
			"\tsub\t$sp, $sp, 4\n"+
			"\tsw\t$" + getRegNumber(reg) + ", 4($sp)\n";
	}

	/**
	 * Pops the stack into the given register.
	 * 
	 * @param reg Register to pop into.
	 */
	public void genPop(Register reg)
	{
		_codeString +=
			"\tlw\t$" + getRegNumber(reg) + ", 4($sp)\n" +
			"\tadd\t$sp, $sp, 4\n";
	}
	 
	/** 
	 * Write an sw instruction in the format:
     * <pre>
     *   sw $9, label
	 * </pre>
	 * 
	 * @param label Label identifying a variable.
	 * @param reg Register whose value should be stored.
	 */
	public void genStore( String label, Register reg )
	{
		_codeString += 
  		"\tsw\t$" +
  		getRegNumber(reg) +
  		", " +
  		label + "_\n";
	}
	 
	/** 
	 * Write an sw instruction in the format <code>
	 * sw $9, label($11)
	 * </code>
	 * Adds the value of the label to the value in the register
	 * given by index and stores the value in the register give
	 * by reg in the word at that computed address.
	 * @param id Label or constant offset for the immediate-register mode
	 * @param reg Register whose value should be stored
	 * @param index Variable offset or index for the immediate-register mode
	 */
	public void genStoreIndirect( String id, Register reg, Register index )
	{
		_codeString +=
  		"\tsw\t$" +
  		getRegNumber(reg) +
  		", " +
  		id + "_" +
  		"($" +
  		getRegNumber(index) +
  		")\n";
	}
	 
	/** 
	 * Write an lw instruction in the format<code>
	 * lw $9, label
	 * </code>
	 * @param id Label identifying a variable
	 * @param reg Register that will be changed
	 */
	public void genLoad( String id, Register reg )
	{
		_codeString +=
  		"\tlw\t$" +
  		getRegNumber(reg) +
  		", " +
  		id + "_\n";
	}
	 
	/** 
	 * Write an li instruction.
	 * @param reg Register that will be changed
	 * @param val Value to be put in the register
	 */
	public void genLoadI( Register reg, int val )
	{
		_codeString +=
  		"\tli\t$" +
  		getRegNumber(reg) +
  		", " +
  		val + "\n";
	}
	 
	/** 
	 * Write an lw instruction in the format <code>
	 * lw $9, label($11)
	 * </code>
	 * Adds the value of the label to the value in the register
	 * given by index, loads the word at that address and
	 * stores it in the register given by reg.
	 * @param id Label or constant offset for the immediate-register mode
	 * @param reg Register that will be changed
	 * @param index Variable offset or index for the immediate-register mode
	 */
	public void genLoadIndirect( String id, Register reg, Register index )
	{
		_codeString += 
  		"\tlw\t$" +
  		getRegNumber(reg) +
  		", " +
  		id + "_" +
  		"($" +
  		getRegNumber(index) +
  		")\n";
	}
	 
	/** 
	 * Write a move instruction.
	 * @param reg1 Destination register
	 * @param reg2 Source register
	 */
	public void genMove( Register reg1, Register reg2 )
	{
		_codeString +=
  		"\tmove\t" +
  		"$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) + "\n";
	}
	 
	/** 
	 * Write an add instruction in the format<code>
	 * add $8, $8, $9
	 * @param reg1 Source and destination register
	 * @param reg2 Source register
	 */
	public void genPlus( Register reg1, Register reg2 )
	{
		_codeString += 
  		"\tadd\t" +
  		"$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) + "\n";
	}
	 
	/** 
	 * Write a sub instruction in the format <code>
	 * sub $8, $8, $9
	 * </code>
	 * @param reg1 Minuend and destination register
	 * @param reg2 Subtrahend source register
	 */
	public void genMinus( Register reg1, Register reg2 )
	{
		_codeString +=
  		"\tsub\t" +
  		"$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) + "\n";
	}
	 
	/** 
	 * Write a mul instruction in the format <code>
	 * mul $8, $8, $9
	 * </code>
	 * @param reg1 Source and destination register
	 * @param reg2 Source register
	 */
	public void genTimes( Register reg1, Register reg2 )
	{
		_codeString += 
  		"\tmul\t" +
  		"$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) + "\n";
	}
	
	/** 
	 * Write a div instruction in the format <code>
	 * div $8, $8, $9
	 * </code>
	 * @param reg1 Source and destination register
	 * @param reg2 Source register
	 */
	public void genDivide( Register reg1, Register reg2 ) 
	{
		_codeString +=
		"\tdiv\t" +
		"$" +
		getRegNumber(reg1) +
		", $" +
		getRegNumber(reg1) +
		", $" +
		getRegNumber(reg2) + "\n";
	}

	/** 
	 * Write a rem instruction in the format <code>
	 * rem $8, $8, $9
	 * </code>
	 * @param reg1 Source and destination register
	 * @param reg2 Source register
	 */
	public void genRemainder( Register reg1, Register reg2 )
	{
		_codeString +=
		"\trem\t" +
		"$" +
		getRegNumber(reg1) +
		", $" +
		getRegNumber(reg1) +
		", $" +
		getRegNumber(reg2) + "\n";
	}
	 
	/** p 
	 * Write a j instruction, with the label specified by a number as in genLabel(.).
	 * @param label Unique identifier for the label
	 */
	public void genJump( int label )
	{
		_codeString +=
  		"\tj\tl" +
  		label + "\n";
	}
	
	/** 
	 * Write a beqz instruction, with the label specified by a number as in genLabel(.).
	 * @param reg Register
	 * @param label Unique identifier for the label
	 */
	public void genJumpEqualToZero( Register reg, int label )
	{
		_codeString +=
  		"\tbeqz\t$" +
		getRegNumber(reg) +
		", l" +
		label + "\n";
	}
	 
	/** 
	 * Write a bnez instruction, with the label specified by a number as in genLabel(.).
	 * @param reg Register
	 * @param label Unique identifier for the label
	 */
	public void genJumpNotEqualToZero( Register reg, int label )
	{
		_codeString +=
  		"\tbnez\t$" +
		getRegNumber(reg) +
		", l" +
		label + "\n";
	}
	 
	/** 
	 * Write a seq instruction, that stores the result in the first register
	 * @param reg1 Left-hand source and destination register
	 * @param reg2 Right-hand source register
	 */
	public void genStoreEq( Register reg1, Register reg2)
	{
		_codeString +=
  		"\tseq\t" +
  		"$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) + "\n";
	}
	 
	/** 
	 * Write a sge instruction, that stores the result in the first register
	 * @param reg1 Left-hand source and destination register
	 * @param reg2 Right-hand source register
	 */
	public void genStoreGreaterEq( Register reg1, Register reg2)
	{
		_codeString +=
  		"\tsge\t" +
  		"$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) + "\n";
	}
	 
	/** 
	 * Write a sgt instruction, that stores the result in the first register
	 * @param reg1 Left-hand source and destination register
	 * @param reg2 Right-hand source register
	 */
	public void genStoreGreaterThan( Register reg1, Register reg2)
	{
		_codeString +=
  		"\tsgt\t" +
  		"$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) + "\n";
	}
	 
	/** 
	 * Write a slt instruction, that stores the result in the first register
	 * @param reg1 Left-hand source and destination register
	 * @param reg2 Right-hand source register
	 */
	public void genStoreLessThan( Register reg1, Register reg2)
	{
		_codeString +=
  		"\tslt\t" +
  		"$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) + "\n";
	}
	 
	/** 
	 * Write a sle instruction, that stores the result in the first register
	 * @param reg1 Left-hand source and destination register
	 * @param reg2 Right-hand source register
	 */
	public void genStoreLessEq( Register reg1, Register reg2)
	{
		_codeString +=
  		"\tsle\t" +
  		"$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) + "\n";
	}

	/** 
	 * Write a beq instruction, with the label specified by a number as in genLabel(.).
	 * @param reg1 Left-hand register
	 * @param reg2 Right-hand register
	 * @param label Unique identifier for the label
	 */
	public void genJumpEq( Register reg1, Register reg2, int label )
	{
		_codeString +=
  		"\tbeq\t$" +
		getRegNumber(reg1) +
		", $" + 
		getRegNumber(reg2) +
		", l" +
		label + "\n";
	}
	 
	/** 
	 * Write a bge instruction, with the label specified by a number as in genLabel(.).
	 * @param reg1 Left-hand register
	 * @param reg2 Right-hand register
	 * @param label Unique identifier for the label
	 */
	public void genJumpGreaterEq( Register reg1, Register reg2, int label )
	{
		_codeString +=
  		"\tbge\t$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) +
  		", l" +
  		label + "\n";
	}
	 
	/** 
	 * Write a ble instruction, with the label specified by a number as in genLabel(.).
	 * @param reg1 Left-hand register
	 * @param reg2 Right-hand register
	 * @param label Unique identifier for the label
	 */
	public void genJumpLessEq( Register reg1, Register reg2, int label )
	{
		_codeString += 
  		"\tble\t$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) +
  		", l" +
  		label + "\n";
	}
	 
	/** 
	 * Write a bgt instruction, with the label specified by a number as in genLabel(.).
	 * @param reg1 Left-hand register
	 * @param reg2 Right-hand register
	 * @param label Unique identifier for the label
	 */
	public void genJumpGreaterThan( Register reg1, Register reg2, int label )
	{
		_codeString += 
  		"\tbgt\t$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) +
  		", l" +
  		label + "\n";
	}
	 
	/** 
	 * Write a blt instruction, with the label specified by a number as in genLabel(.).
	 * @param reg1 Left-hand register
	 * @param reg2 Right-hand register
	 * @param label Unique identifier for the label
	 */
	public void genJumpLessThan( Register reg1, Register reg2, int label )
	{
		_codeString +=
  		"\tblt\t$" +
  		getRegNumber(reg1) +
  		", $" +
  		getRegNumber(reg2) +
  		", l" +
  		label + "\n";
	}
	 
	/** 
	 * Write a bnez instruction, with the label specified by a number as in genLabel(.).
	 * @param reg Register to check
	 * @param label Unique identifier for the label
	 */
	public void genJumpNotEqualZero( Register reg, int label )
	{
		_codeString +=
  		"\tbnez\t$" +
  		getRegNumber(reg) +
  		", l" +
  		label + "\n";
	}
	 
	/** 
	 * Write a comment introduced by a # sign.
	 * @param comment
	 */
	public void genComment( String comment )
	{
		_codeString += "# " + comment + "\n";
	}

	/**
	 * Write the _codeString to the console
	 */
	public void writeToConsole()
	{
		System.out.println(_codeString + "\n");
	}

	/**
	 * Write the _codeString to the file
	 */
	public void writeToFile(String filename)
	{
		try {
			FileWriter out = new FileWriter(filename);
			out.write(_codeString + "\n");
			out.close();
		}
		catch(java.io.IOException e)
		{
			System.err.println(e.getMessage() + "\n");
			e.printStackTrace(System.err);
			System.exit(1);
		}
	}		

	/**
	 * A demonstration of the use of this class.
	 */
	public static void main(String argv[])
	{
		MIPSCodeGenerator mcg = new MIPSCodeGenerator();
		mcg.genPush(Register.S0);
		mcg.genPop(Register.S0);
		mcg.writeToConsole();
		mcg.writeToFile("test");
	}
}
