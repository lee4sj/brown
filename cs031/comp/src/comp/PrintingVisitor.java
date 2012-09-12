package comp;

import comp.nodes.*;

/**
 * Class that will print out the parsed syntax tree of a NodeProgram.
 * <p>
 * Instantiate it and then have it visit a NodeProgram or any other Node.
 */
public class PrintingVisitor implements Visitor {
	
	private int _tab;
	
	public PrintingVisitor() {
		_tab = 0;
	}
	
	/**
	 * Checks to make sure the node exists, and if so, visits it.
	 */
	private void checkAndVisit(Node node) throws VisitorException{
		if (node != null)
			node.visit(this);
	}
	
	/**
	 * Handles visiting a NodeInstrDeclArray
	 */
	public void handleNodeInstrDeclArray(NodeInstrDeclArray node) throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrDeclArray: " + node.identifier()
				+ " [" + node.size() + "]\n");
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrDeclSingle
	 */
	public void handleNodeInstrDeclSingle(NodeInstrDeclSingle node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrDeclSingle: " + node.identifier()
				+ "\n");
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeExprArrayVar
	 */
	public void handleNodeExprArrayVar(NodeExprArrayVar node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeExprArrayVar: " + node.variable()
				+ " []");
		checkAndVisit(node.index());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeExprInt
	 */
	public void handleNodeExprInt(NodeExprInt node) throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeExprInt " + node.value() + "\n");
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeExprMinus
	 */
	public void handleNodeExprMinus(NodeExprMinus node) throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeExprMinus");
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeExprPlus
	 */
	public void handleNodeExprPlus(NodeExprPlus node) throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeExprPlus");
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeExprTimes
	 */
	public void handleNodeExprTimes(NodeExprTimes node) throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeExprTimes");
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
		_tab--;
	}

	/**
	 * Handles visiting a NodeExprDivide
	 */
	public void handleNodeExprDivide(NodeExprDivide node) throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeExprDivide");
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeExprMod
	 */
	public void handleNodeExprMod(NodeExprMod node) throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeExprMod");
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeExprVar
	 */
	public void handleNodeExprVar(NodeExprVar node) throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeExprVar: " + node.variable() + 
		"\n");
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrAssignArray
	 */
	public void handleNodeInstrAssignArray(NodeInstrAssignArray node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrAssignArray: "
				+ node.identifier() + " []");
		System.out.println(tab(_tab) + "AssignArray Index:");
		checkAndVisit(node.index());
		System.out.println(tab(_tab) + "AssignArray Expr:");
		checkAndVisit(node.expression());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrAssignSingle
	 */
	public void handleNodeInstrAssignSingle(NodeInstrAssignSingle node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrAssignSingle: "
				+ node.identifier());
		System.out.println(tab(_tab) + "Assign Expr:");
		checkAndVisit(node.expression());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrBlock
	 */
	public void handleNodeInstrBlock(NodeInstrBlock node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrBlock");
		System.out.println(tab(_tab) + "Block Body:");
		checkAndVisit(node.list());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrIfthen
	 */
	public void handleNodeInstrIfthen(NodeInstrIfthen node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrIfthen");
		System.out.println(tab(_tab) + "If Relation:");
		checkAndVisit(node.relation());
		System.out.println(tab(_tab) + "Then Body:");
		checkAndVisit(node.thenInstr());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrIfthenelse
	 */
	public void handleNodeInstrIfthenelse(NodeInstrIfthenelse node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrIfthenelse");
		System.out.println(tab(_tab) + "If Relation:");
		checkAndVisit(node.relation());
		System.out.println(tab(_tab) + "Then Body:");
		checkAndVisit(node.thenInstr());
		System.out.println(tab(_tab) + "Else Body:");
		checkAndVisit(node.elseInstr());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrOutput
	 */
	public void handleNodeInstrOutput(NodeInstrOutput node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrOutput:");
		checkAndVisit(node.expr());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrInputArray
	 */
	public void handleNodeInstrInputArray(NodeInstrInputArray node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrInputArray: " + node.identifier() + "[]");
		System.out.println(tab(_tab) + "NodeInstrInputArray Expr: ");
		checkAndVisit(node.index());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrInputSingle
	 */
	public void handleNodeInstrInputSingle(NodeInstrInputSingle node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrInputSingle: " + node.identifier());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeInstrWhile
	 */
	public void handleNodeInstrWhile(NodeInstrWhile node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeInstrWhile");
		System.out.println(tab(_tab) + "While Relation:");
		checkAndVisit(node.relation());
		System.out.println(tab(_tab) + "While Body:");
		checkAndVisit(node.instr());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeListInstrCons
	 */
	public void handleNodeListInstrCons(NodeListInstrCons node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeListInstrCons");
		checkAndVisit(node.head());
		checkAndVisit(node.tail());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeListInstrNil
	 */
	public void handleNodeListInstrNil(NodeListInstrNil node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeListInstrNil\n");
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeProgram
	 */
	public void handleNodeProgram(NodeProgram node) throws VisitorException {
		System.out.println(tab(_tab) + "NodeProgram");
		checkAndVisit(node.listInstr());
	}
	
	/**
	 * Handles visiting a NodeRelationLessThan
	 */
	public void handleNodeRelationLessThan(NodeRelationLessThan node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeRelationLessThan");
		checkAndVisit(node.left());
		checkAndVisit(node.right());
		_tab--;
		
	}
	
	/**
	 * Handles visiting a NodeRelationLessThanEqual
	 */
	public void handleNodeRelationLessThanEqual(NodeRelationLessThanEqual node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeRelationLessThanEqual");
		checkAndVisit(node.left());
		checkAndVisit(node.right());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeRelationEqual
	 */
	public void handleNodeRelationEqual(NodeRelationEqual node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeRelationEqual");
		checkAndVisit(node.left());
		checkAndVisit(node.right());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeRelationGreaterThanEqual
	 */
	public void handleNodeRelationGreaterThanEqual(NodeRelationGreaterThanEqual node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeRelationGreaterThanEqual");
		checkAndVisit(node.left());
		checkAndVisit(node.right());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeRelationGreaterThan
	 */
	public void handleNodeRelationGreaterThan(NodeRelationGreaterThan node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeRelationGreaterThan");
		checkAndVisit(node.left());
		checkAndVisit(node.right());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeRelationAnd
	 */
	public void handleNodeRelationAnd(NodeRelationAnd node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeRelationAnd");
		checkAndVisit(node.left());
		checkAndVisit(node.right());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeRelationOr
	 */
	public void handleNodeRelationOr(NodeRelationOr node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeRelationOr");
		checkAndVisit(node.left());
		checkAndVisit(node.right());
		_tab--;
	}
	
	/**
	 * Handles visiting a NodeRelationNot
	 */
	public void handleNodeRelationNot(NodeRelationNot node)
	throws VisitorException {
		_tab++;
		System.out.println(tab(_tab) + "NodeRelationNot");
		checkAndVisit(node.relation());
		_tab--;
	}
	
	public String tab(int amount) {
		String result = "";
		for (int x = 0; x < amount; x++)
			result = result + " ";
		return result;
	}
	
}
