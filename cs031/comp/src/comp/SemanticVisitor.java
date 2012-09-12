package comp;

import comp.nodes.*;
import java.util.ArrayList;
import java.util.Stack;

/**
 * Class that will check the semantics of the parsed syntax tree of a NodeProgram.
 * Semantic check includes:
 *    redefining of variables in the same scope
 *    using a single as an array or an array as a single
 *    using an undefined variable
 *    type analysis
 * <p>
 * Instantiate it and then have it visit a NodeProgram or any other Node.
 */
public class SemanticVisitor implements Visitor {
	
	private ArrayList<DeclVar> declList;	// ArrayList for keeping list of declared variables
											// and for keeping track of the largest variable with the same name but is declared in different scope
											// this is passed to CodeGenVisitor for memory allocation purpose.

    private Stack<ID> idList;			// ArrayList for scope analysis
    private int scopeCounter;				// Used to keep track of the scopes

    private Stack<SemanticType> typeStack;	// Stack for type checking

	public SemanticVisitor(){
		declList = new ArrayList<DeclVar>();

        idList = new Stack<ID>();
        scopeCounter = 0;
        idList.push(new ID((scopeCounter + "scope"), IDType.NEWSCOPE));
        scopeCounter++;

        typeStack = new Stack<SemanticType>();
	}

    public ArrayList<DeclVar> getDeclList(){
        return declList;
    }

    private void die(String message) throws SemanticException{
        throw new SemanticException("Semantic Error: " + message);
    }
	
	private void checkAndVisit(Node node) throws VisitorException{
        if (node != null)
            node.visit(this);
        else throw new VisitorException("This is not supposed to happen, but a node is missing something");
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrDeclArray(NodeInstrDeclArray node) throws VisitorException {
		//***** scope analysis
        ID temp = new ID(node.identifier(), IDType.ARRAY);
        ID temp2 = new ID(node.identifier(), IDType.SINGLE);
        if(idList.contains(temp) || idList.contains(temp2))
           die( node.identifier()+ " is already declared!");

        idList.push(temp);

        //***** memory allocation: ** note that memory allocation happens after it passes the scope analysis
        DeclVar var = new DeclVar(node.identifier(), node.size());
        int index = declList.indexOf(var);
        if(index < 0 || node.size() > declList.get(index).size) // if node.ID is not already used in some other scope
                                                                //    or has greater size than the one already in the map
            declList.add(var);                                  //          add node.ID

        //otherwise, just keep the largest one
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrDeclSingle(NodeInstrDeclSingle node) throws VisitorException {
        //*****scope analysis
        ID temp = new ID(node.identifier(), IDType.ARRAY);
        ID temp2 = new ID(node.identifier(), IDType.SINGLE);
        if(idList.contains(temp) || idList.contains(temp2))
           die( node.identifier()+ " is already declared!");

        idList.push(temp2);

        //***** memory allocation: ** note that memory allocation happens after it passes the scope analysis
        DeclVar var = new DeclVar(node.identifier(), 1);
        int index = declList.indexOf(var);
        if(index < 0)                              // if node.ID is not already used in some other scope
            declList.add(var);                     //          add node.ID

        //otherwise, just keep the largest one
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprArrayVar(NodeExprArrayVar node) throws VisitorException {

        checkAndVisit(node.index());
        
        //***** scope analysis
        if(!idList.contains(new ID(node.variable(), IDType.ARRAY)))
            die("a variable is used without declaration or is a scalar, but used as an array!");

        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER)
            die(" the index of the array has wrong semantic type!!");

        typeStack.push(node.semanticType());

	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprInt(NodeExprInt node) throws VisitorException {
		//***** type checking
        typeStack.push(SemanticType.INTEGER);
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprMinus(NodeExprMinus node) throws VisitorException {
		checkAndVisit(node.leftChild());
        checkAndVisit(node.rightChild());

        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" the input to minus operator has wrong semantic type!");

        typeStack.push(node.semanticType());

	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprPlus(NodeExprPlus node) throws VisitorException {
		checkAndVisit(node.leftChild());
        checkAndVisit(node.rightChild());

        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" the input to plus operator has wrong semantic type!");

        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprTimes(NodeExprTimes node) throws VisitorException {
		checkAndVisit(node.leftChild());
        checkAndVisit(node.rightChild());

        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" the input to multiplication operator has wrong semantic type!");

        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprDivide(NodeExprDivide node) throws VisitorException {
		checkAndVisit(node.leftChild());
        checkAndVisit(node.rightChild());

        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" the input to division operator has wrong semantic type!");

        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprMod(NodeExprMod node) throws VisitorException {
		checkAndVisit(node.leftChild());
        checkAndVisit(node.rightChild());

        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" the input to modulus operator has wrong semantic type!");

        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeExprVar(NodeExprVar node) throws VisitorException {
		//***** scope analysis
        if(!idList.contains(new ID(node.variable(), IDType.SINGLE)))
            die(node.variable() + " is used without declaration or is an array, but used as a scalar!");

        //***** type checking
        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrAssignArray(NodeInstrAssignArray node) throws VisitorException {
		checkAndVisit(node.index());
        checkAndVisit(node.expression());

		//***** scope analysis
		if(!idList.contains(new ID(node.identifier(), IDType.ARRAY)))
            die("a variable is used without declaration or is a scalar, but used as an array!");

        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" the input to modulus operator has wrong semantic type!");
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrAssignSingle(NodeInstrAssignSingle node) throws VisitorException {

        checkAndVisit(node.expression());

		//***** scope analysis
        if(!idList.contains(new ID(node.identifier(), IDType.SINGLE)))
            die(node.identifier() + " is used without declaration or is an array, but used as a scalar!");

        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER)
            die(" invalid assignment (wrong semantic type)!");
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrBlock(NodeInstrBlock node) throws VisitorException {

        //***** scope analysis: note that scope analysis must be done first here before visiting its children
        idList.push(new ID((scopeCounter + "scope"), IDType.NEWSCOPE));
        scopeCounter++;

        checkAndVisit(node.list());

	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrIfthen(NodeInstrIfthen node) throws VisitorException {
        checkAndVisit(node.relation());
		checkAndVisit(node.thenInstr());

        //***** type checking
        if(typeStack.pop() != SemanticType.BOOLEAN)
            die(" invalid if statement (wrong semantic type)!");


	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrIfthenelse(NodeInstrIfthenelse node) throws VisitorException {
		checkAndVisit(node.relation());
		checkAndVisit(node.thenInstr());
        checkAndVisit(node.elseInstr());

        //***** type checking
        if(typeStack.pop() != SemanticType.BOOLEAN)
            die(" invalid if statement (wrong semantic type)!");

	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrOutput(NodeInstrOutput node) throws VisitorException {
        checkAndVisit(node.expr());
		
        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER)
            die(" invalid value to output (wrong semantic type)!");
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrInputArray(NodeInstrInputArray node) throws VisitorException {
        checkAndVisit(node.index());

		// scope analysis
        if(!idList.contains(new ID(node.identifier(), IDType.ARRAY)))
            die("a variable is used without declaration or is a scalar, but used as an array!");

        //***** type checking
        if(typeStack.pop() != SemanticType.INTEGER)
            die(" invalid value to input (wrong semantic type)!");
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrInputSingle(NodeInstrInputSingle node) throws VisitorException {

		//****** scope analysis
        if(!idList.contains(new ID(node.identifier(), IDType.SINGLE)))
            die(node.identifier() + " is used without declaration or is an array, but used as a scalar!");
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeInstrWhile(NodeInstrWhile node) throws VisitorException {
        checkAndVisit(node.relation());
        checkAndVisit(node.instr());

		//***** type analysis
        if(typeStack.pop() != SemanticType.BOOLEAN)
            die(" invalide while loop (wrong semantic type)!");


	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeListInstrCons(NodeListInstrCons node) throws VisitorException {
        checkAndVisit(node.head());
        checkAndVisit(node.tail());

		// TODO: Your code here
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeListInstrNil(NodeListInstrNil node) throws VisitorException {
		// scope analysis
        scopeCounter--;
		boolean inScope = true;

		while(inScope){
			if(idList.pop().type == IDType.NEWSCOPE)
				inScope = false;
		}
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeProgram(NodeProgram node) throws VisitorException {
        checkAndVisit(node.listInstr());

		// TODO: Your code here
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationLessThan(NodeRelationLessThan node) throws VisitorException {
        checkAndVisit(node.left());
        checkAndVisit(node.right());

		//***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" invalid expressions for less-than relation (wrong semantic type)!");
        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationLessThanEqual(NodeRelationLessThanEqual node) throws VisitorException {
		checkAndVisit(node.left());
        checkAndVisit(node.right());

		//***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" invalid expressions for less-than-equal  relation (wrong semantic type)!");
        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationEqual(NodeRelationEqual node) throws VisitorException {
		checkAndVisit(node.left());
        checkAndVisit(node.right());

		//***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" invalid expressions for equal relation (wrong semantic type)!");
        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationGreaterThanEqual(NodeRelationGreaterThanEqual node) throws VisitorException {
		checkAndVisit(node.left());
        checkAndVisit(node.right());

		//***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" invalid expressions for greater-than-eqaul relation (wrong semantic type)!");
        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationGreaterThan(NodeRelationGreaterThan node) throws VisitorException {
		checkAndVisit(node.left());
        checkAndVisit(node.right());

		//***** type checking
        if(typeStack.pop() != SemanticType.INTEGER || typeStack.pop() != SemanticType.INTEGER)
            die(" invalid expressions for greater-than relation (wrong semantic type)!");
        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationAnd(NodeRelationAnd node) throws VisitorException {
		checkAndVisit(node.left());
        checkAndVisit(node.right());

		//***** type checking
        if(typeStack.pop() != SemanticType.BOOLEAN || typeStack.pop() != SemanticType.BOOLEAN)
            die(" invalid expressions for AND relation (wrong semantic type)!");
        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationOr(NodeRelationOr node) throws VisitorException {
		checkAndVisit(node.left());
        checkAndVisit(node.right());

		//***** type checking
        if(typeStack.pop() != SemanticType.BOOLEAN || typeStack.pop() != SemanticType.BOOLEAN)
            die(" invalid expressions for OR relation (wrong semantic type)!");
        typeStack.push(node.semanticType());
	}

	/**
	 * semantic analysis for the input node.
	 *
	 * @param node	the node to be analyzed
	 * @throws comp.VisitorException
	 */
	public void handleNodeRelationNot(NodeRelationNot node) throws VisitorException {
		checkAndVisit(node.relation());

		//***** type checking
        if(typeStack.pop() != SemanticType.BOOLEAN)
            die(" invalid expressions for NOT relation (wrong semantic type)!");
        typeStack.push(node.semanticType());
	}
}
