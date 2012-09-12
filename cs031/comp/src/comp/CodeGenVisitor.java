package comp;

import comp.nodes.*;
import java.util.ArrayList;

/**
 * Visit a parsed sytax tree from a {@link NodeProgram} and generate MIPS
 * code. It makes use of the {@link MIPSCodeGenerator} for the actual creation
 * of MIPS code.
 *
 * @version Fall 2007
 * @author The TAs
 */
public class CodeGenVisitor implements Visitor {
	
	private MIPSCodeGenerator _codegen;

    /**
     * Create a new <code>CodeGenVisitor</code>. Uses the given {@link
     * MIPSCodeGenerator} to help generate code.
	 * Declares the variables that need to be declared in the .data section of
	 * the MIPS code using the list passed in.
	 * The passed in list must hold all the variables that needs to be declared.
     *
     * @param codegen The <code>MIPSCodeGenerator</code> to use when outputting
	 * @param list	the list of variables that needs to be declared.
     * code.
     */
	public CodeGenVisitor(MIPSCodeGenerator codegen, ArrayList<DeclVar> list){
		_codegen = codegen;
        
        _codegen.genData();

        for(int a = 0; a < list.size(); a++){
            if(list.get(a).size == 1)
                _codegen.genDecl(list.get(a).name);
            else
                _codegen.genArrayDecl(list.get(a).size, list.get(a).name);
        }

        _codegen.genText();
	}
	
	private void checkAndVisit(Node node) throws VisitorException{
        if (node != null)
            node.visit(this);
        else throw new VisitorException("This is not supposed to happen, but a node is missing something");
	}


	/**
	 * Do nothing since declaration is handled in the construction!
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrDeclArray(NodeInstrDeclArray node) throws VisitorException {
		// Don't do anything since the declaration is handled in the beginning
	}


	/**
	 * Do nothing since declaration is handled in the construction!
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrDeclSingle(NodeInstrDeclSingle node) throws VisitorException {
		// Don't do anything since the declaration is handled in the beginning
	}


	/**
	 * Generate code for an array access.
	 * load the value in the array and push the value into the stack.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprArrayVar(NodeExprArrayVar node) throws VisitorException {

        checkAndVisit(node.index());

        _codegen.genPop(Register.T0);   //load index

		// convert the index to offset
		_codegen.genLoadI(Register.T2, 4);
		_codegen.genTimes(Register.T0, Register.T2);

        _codegen.genLoadIndirect(node.variable(), Register.T1, Register.T0); //load the value in the array at the index
        _codegen.genPush(Register.T1);   //push to the stack
	}

	/**
	 * Generate code for the immediate value
	 * li, load immediately, the value and push it into the stack
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprInt(NodeExprInt node) throws VisitorException {

        _codegen.genLoadI(Register.T0, node.value());       // load the value of the int
        _codegen.genPush(Register.T0);      //push to the stack
	}


	/**
	 * Generate code for subtraction
	 * Get the value of the lefthand and the righthand.
	 * Then, subtract the righthand from the lefthand, and push the result into the stack.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprMinus(NodeExprMinus node) throws VisitorException {

        //visit children
		checkAndVisit(node.rightChild());
		checkAndVisit(node.leftChild());
        

        //generate code
		_codegen.genPop(Register.T1);       //pop lefthand
        _codegen.genPop(Register.T0);       //pop righthand
        _codegen.genMinus(Register.T1, Register.T0);    // lefthand - righthand
        _codegen.genPush(Register.T1);      // push to the stack
	}


	/**
	 * Generate code for addition
	 * Get the value of the lefthand and the righthand.
	 * Then, add the righthand from the lefthand, and push the result into the stack.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprPlus(NodeExprPlus node) throws VisitorException {

        //visit children
        checkAndVisit(node.rightChild());
		checkAndVisit(node.leftChild());

        //generate code
		_codegen.genPop(Register.T1);       //pop lefthand
        _codegen.genPop(Register.T0);       //pop righthand
        _codegen.genPlus(Register.T1, Register.T0);    // lefthand - righthand
        _codegen.genPush(Register.T1);      // push to the stack
	}

	/**
	 * Generate code for multiplication
	 * Get the value of the lefthand and the righthand.
	 * Then, multiply the righthand from the lefthand, and push the result into the stack.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprTimes(NodeExprTimes node) throws VisitorException {
		//visit children
        checkAndVisit(node.rightChild());
		checkAndVisit(node.leftChild());

        //generate code
        _codegen.genPop(Register.T1);       //pop lefthand
		_codegen.genPop(Register.T0);       //pop righthand
        _codegen.genTimes(Register.T1, Register.T0);    // lefthand - righthand
        _codegen.genPush(Register.T1);      // push to the stack
	}

	/**
	 * Generate code for division
	 * Get the value of the lefthand and the righthand.
	 * Then, divide the righthand from the lefthand, and push the result into the stack.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprDivide(NodeExprDivide node) throws VisitorException {
		//visit children
        checkAndVisit(node.rightChild());
		checkAndVisit(node.leftChild());

        //generate code
        _codegen.genPop(Register.T1);       //pop lefthand
		_codegen.genPop(Register.T0);       //pop righthand
        _codegen.genDivide(Register.T1, Register.T0);    // lefthand - righthand
        _codegen.genPush(Register.T1);      // push to the stack
	}

	/**
	 * Generate code for modulus operation
	 * Get the value of the lefthand and the righthand.
	 * Then, result = lefthand mod righthand, and push the result into the stack.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprMod(NodeExprMod node) throws VisitorException {
		//visit children
        checkAndVisit(node.rightChild());
		checkAndVisit(node.leftChild());

        //generate code
        _codegen.genPop(Register.T1);       //pop lefthand
		_codegen.genPop(Register.T0);       //pop righthand
        _codegen.genRemainder(Register.T1, Register.T0);    // lefthand - righthand
        _codegen.genPush(Register.T1);      // push to the stack
	}

	/**
	 * Generate code for scalar variable access.
	 * Load the value stored in the scalar variable and push it into the stack.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprVar(NodeExprVar node) throws VisitorException {

        _codegen.genLoad(node.variable(), Register.T0);     //load the value
        _codegen.genPush(Register.T0);      // push to the stack
	}

	/**
	 * Generate code for array assignment.
	 * Get the index. Then store the value into the array at the index.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrAssignArray(NodeInstrAssignArray node) throws VisitorException {
		//visit child
        checkAndVisit(node.expression());
        checkAndVisit(node.index());

        //generate code
        _codegen.genPop(Register.T0);       // load the index
		
		// convert the index to offset
		_codegen.genLoadI(Register.T2, 4);
		_codegen.genTimes(Register.T0, Register.T2);

        _codegen.genPop(Register.T1);       // load the value of the expression to be assigned
        _codegen.genStoreIndirect(node.identifier(), Register.T1, Register.T0);		// store the value at the array at the index
	}

	/**
	 * Generate code for scalar assignment.
	 * Store the value into the scalar.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrAssignSingle(NodeInstrAssignSingle node) throws VisitorException {
		//visit child
        checkAndVisit(node.expression());

        //generate code
        _codegen.genPop(Register.T0);       // load the value of the expression to be assigned
        _codegen.genStore(node.identifier(), Register.T0);		//push to the stack
	}

	/**
	 * No code generated.
	 * However, visits its child node.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrBlock(NodeInstrBlock node) throws VisitorException {
		//visit child
		checkAndVisit(node.list());
	}

	/**
	 * Generate code for if-then statement.
	 * if(relation == true), do the instruction.
	 * Otherwise do nothing.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrIfthen(NodeInstrIfthen node) throws VisitorException {

		// visit the condition
		checkAndVisit(node.relation());

		//generate if code
		int endif = _codegen.getNextLabel();
		_codegen.genPop(Register.T0);							// pop the relation
		_codegen.genJumpEqualToZero(Register.T0, endif);		// if relation != false
		checkAndVisit(node.thenInstr());						// then, do theInstruction
		_codegen.genLabel(endif);								// generate endif label
		
	}

	/**
	 * Generate code for if-then-else statement.
	 * if(relation == true), do the if-instruction.
	 * otherwise, do the else-instruction.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrIfthenelse(NodeInstrIfthenelse node) throws VisitorException {
		// visit the condition
		checkAndVisit(node.relation());

		//generate if code
		int elseif = _codegen.getNextLabel();
		int endif = _codegen.getNextLabel();

		_codegen.genPop(Register.T0);							// pop the relation
		_codegen.genJumpEqualToZero(Register.T0, elseif);		// if relation != false
		checkAndVisit(node.thenInstr());						// then, do thenInstruction
		_codegen.genJump(endif);								// and jump to endif

		_codegen.genLabel(elseif);								// generate else label
		checkAndVisit(node.elseInstr());						// then do elseInstruction

		_codegen.genLabel(endif);								// generate endif label
	}

	/**
	 * Generate code for the output.
	 * Get the value to output, and output.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrOutput(NodeInstrOutput node) throws VisitorException {
		// visit the expression to output
		checkAndVisit(node.expr());

		//generate code
		_codegen.genPop(Register.A0);
		_codegen.genOutput();
	}

	/**
	 * Generate code for inputing an array.
	 * Get the input, then the index.
	 * Then, assign the input to the array at the index.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrInputArray(NodeInstrInputArray node) throws VisitorException {
		
		// visit index
		checkAndVisit(node.index());

		_codegen.genInput();														// get the input
		_codegen.genPop(Register.T0);												// pop the index

		// convert the index to offset
		_codegen.genLoadI(Register.T1, 4);
		_codegen.genTimes(Register.T0, Register.T1);

		_codegen.genStoreIndirect(node.identifier(), Register.V0, Register.T0);		// assign the input to the array at the index
	}

	/**
	 * Generate code for inputing a scalar.
	 * Get the input, and assign it to the scalar.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrInputSingle(NodeInstrInputSingle node) throws VisitorException {

		_codegen.genInput();			// get the input
		_codegen.genStore(node.identifier(), Register.V0);
	}

	/**
	 * Generate code for a while loop.
	 * while(relation == true), do instruction.
	 * 
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrWhile(NodeInstrWhile node) throws VisitorException {

		
		int whileLoop = _codegen.getNextLabel();
		int endWhile = _codegen.getNextLabel();

		_codegen.genLabel(whileLoop);							// while

		checkAndVisit(node.relation());							// //visit the relation
		_codegen.genPop(Register.T0);							// get the relation (boolean)

		_codegen.genJumpEqualToZero(Register.T0, endWhile);		// while(relation == true)
		checkAndVisit(node.instr());							//	do instruction
		_codegen.genJump(whileLoop);							//
		_codegen.genLabel(endWhile);							// endWhileLoop
	}

	/**
	 * No code generated.
	 * Its children are visited.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeListInstrCons(NodeListInstrCons node) throws VisitorException {

		// visit children
		checkAndVisit(node.head());
		checkAndVisit(node.tail());
	}

	/**
	 * No code to generate! This is a termination of a scope.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeListInstrNil(NodeListInstrNil node) throws VisitorException {
		// nothing to do!!
	}

	/**
	 * Generate code for entering and exiting the main code.
	 * Saves $s registers, execute the code, then loads the $s registers and jump up.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeProgram(NodeProgram node) throws VisitorException {
		_codegen.genMain();

		checkAndVisit(node.listInstr());	// start visiting the tree

		_codegen.genReturnFromMain();
	}

	/**
	 * Generate code for less-then relation.
	 * Push 1 if true. Otherwise push 0.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationLessThan(NodeRelationLessThan node) throws VisitorException {
		//visit children
		checkAndVisit(node.right());
		checkAndVisit(node.left());

		//generate code
		_codegen.genPop(Register.T0);								// pop lefthand
		_codegen.genPop(Register.T1);								// pop righthand
		_codegen.genStoreLessThan(Register.T0, Register.T1);		// check the less-than relation
		_codegen.genPush(Register.T0);								// push to the stack


	}

	/**
	 * Generate code for less-then-equals relation.
	 * Push 1 if true. Otherwise push 0.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationLessThanEqual(NodeRelationLessThanEqual node) throws VisitorException {
		//visit children
		checkAndVisit(node.right());
		checkAndVisit(node.left());

		//generate code
		_codegen.genPop(Register.T0);								// pop lefthand
		_codegen.genPop(Register.T1);								// pop righthand
		_codegen.genStoreLessEq(Register.T0, Register.T1);			// check the less-than-equal relation
		_codegen.genPush(Register.T0);								// push to the stack
	}

	/**
	 * Generate code for equals relation.
	 * Push 1 if true. Otherwise push 0.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationEqual(NodeRelationEqual node) throws VisitorException {
		//visit children
		checkAndVisit(node.right());
		checkAndVisit(node.left());

		//generate code
		_codegen.genPop(Register.T0);								// pop lefthand
		_codegen.genPop(Register.T1);								// pop righthand
		_codegen.genStoreEq(Register.T0, Register.T1);				// check the equal relation
		_codegen.genPush(Register.T0);								// push to the stack
	}

	/**
	 * Generate code for greater-then-equals relation.
	 * Push 1 if true. Otherwise push 0.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationGreaterThanEqual(NodeRelationGreaterThanEqual node) throws VisitorException {
		//visit children
		checkAndVisit(node.right());
		checkAndVisit(node.left());

		//generate code
		_codegen.genPop(Register.T0);								// pop lefthand
		_codegen.genPop(Register.T1);								// pop righthand
		_codegen.genStoreGreaterEq(Register.T0, Register.T1);		// check the greater-than-equal relation
		_codegen.genPush(Register.T0);								// push to the stack
	}

	/**
	 * Generate code for greater-then relation.
	 * Push 1 if true. Otherwise push 0.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationGreaterThan(NodeRelationGreaterThan node) throws VisitorException {
		//visit children
		checkAndVisit(node.right());
		checkAndVisit(node.left());

		//generate code
		_codegen.genPop(Register.T0);								// pop lefthand
		_codegen.genPop(Register.T1);								// pop righthand
		_codegen.genStoreGreaterThan(Register.T0, Register.T1);		// check the greater-than relation
		_codegen.genPush(Register.T0);								// push to the stack
	}

	/**
	 * Generate code for AND relation.
	 * Push 1 if true. Otherwise push 0.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationAnd(NodeRelationAnd node) throws VisitorException {

		int end = _codegen.getNextLabel();

		//** check lefthand is true
		//if lefthand is false, jump to the end
		//otherwise,  check righthand

		checkAndVisit(node.left());							// visit lefthand
		_codegen.genPop(Register.T0);						// pop lefthand

		_codegen.genJumpEqualToZero(Register.T0, end);		// if(lefthand != true), jump to end

		//otherwise, check righthand
		checkAndVisit(node.right());						// visit righthand
		_codegen.genPop(Register.T0);						// pop righthand

		_codegen.genLabel(end);								// generate end label
		_codegen.genPush(Register.T0);						// push to the stack
	}

	/**
	 * Generate code for OR relation.
	 * Push 1 if true. Otherwise push 0.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationOr(NodeRelationOr node) throws VisitorException {
		
		int end = _codegen.getNextLabel();

		//** check lefthand is true
		//if lefthand is true, jump to the end
		//otherwise,  check righthand

		checkAndVisit(node.left());							// visit lefthand
		_codegen.genPop(Register.T0);						// pop lefthand

		_codegen.genJumpNotEqualToZero(Register.T0, end);	// if(lefthand == true), jump to end

		//otherwise, check righthand
		checkAndVisit(node.right());						// visit righthand
		_codegen.genPop(Register.T0);						// pop righthand

		_codegen.genLabel(end);								// generate end label
		_codegen.genPush(Register.T0);						// push to the stack
	}

	/**
	 * Generate code for NOT relation.
	 * Push 1 if true. Otherwise push 0.
	 *
	 * @param node
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationNot(NodeRelationNot node) throws VisitorException {

		checkAndVisit(node.relation());						// visit relation
		_codegen.genPop(Register.T0);						// pop relation

		_codegen.genLoadI(Register.T1, 0);
		_codegen.genStoreEq(Register.T0, Register.T1);		// (relation == false)
		_codegen.genPush(Register.T0);						// push to the stack
	}
}




















