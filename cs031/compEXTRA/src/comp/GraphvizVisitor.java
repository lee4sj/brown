package comp;

import comp.nodes.*;
import java.io.*;
import java.util.HashMap;

/**
 * Class that will print out the parsed syntax tree of a NodeProgram.
 * <p>
 * Instantiate it and then have it visit a NodeProgram or any other Node.
 */
public class GraphvizVisitor implements Visitor {

	private BufferedWriter out;
	private int nodeCounter;
	private HashMap<Node, String> nodeIdentifierMap;

	public GraphvizVisitor(Writer o) {
		nodeCounter = 1;
		nodeIdentifierMap = new HashMap<Node, String>();
		try {
			out = new BufferedWriter (o);
			out.write("digraph parse_tree {");
			out.newLine();
			out.write("node [color=lightblue2, style=filled];");
			out.newLine();
		}
		catch (IOException e){
			e.printStackTrace();
			System.out.println(e);
		}
	}

	public GraphvizVisitor (String filepath){
		nodeCounter = 1;
		nodeIdentifierMap = new HashMap<Node, String>();
		try {
			out = new BufferedWriter (new FileWriter (filepath));
			out.write("digraph parse_tree {");
			out.newLine();
			out.write("node [color=lightblue2, style=filled];");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
	}

	/**
	 * Finishes writing the graphviz file and closes the output stream
	 **/
	public void finish (){
		try {
			out.write("}");
			out.newLine();
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
	}
	
	/**
	 * Checks to make sure the node exists, and if so, visits it.
	 */
	private void checkAndVisit(Node node) throws VisitorException{
		if (node != null)
			node.visit(this);
	}
	
	/**
	 * @return The identifier that will be used to identify the given node in the graphviz output file
	 */
	public String getIdentifier (Node node){
		if(nodeIdentifierMap.containsKey(node))
			return nodeIdentifierMap.get(node);
		else{
			String nodeName = "N" + nodeCounter++;
			nodeIdentifierMap.put(node, nodeName);
			return nodeName;
		}
	}

	/**
	 * Handles visiting a NodeInstrDeclArray
	 * @throws IOException 
	 */
	public void handleNodeInstrDeclArray(NodeInstrDeclArray node) throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrDeclArray: " + node.identifier()
					+ " [" + node.size() + "]\"];");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
	}

	/**
	 * Handles visiting a NodeInstrDeclSingle
	 */
	public void handleNodeInstrDeclSingle(NodeInstrDeclSingle node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrDeclSingle: " + node.identifier()
					+ "\"];");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
	}

	/**
	 * Handles visiting a NodeExprArrayVar
	 */
	public void handleNodeExprArrayVar(NodeExprArrayVar node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeExprArrayVar: " + node.variable()
					+ "[]\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.index()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.index());
	}

	/**
	 * Handles visiting a NodeExprInt
	 */
	public void handleNodeExprInt(NodeExprInt node) throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeExprInt: " + node.value()
					+ "\"];");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
	}

	/**
	 * Handles visiting a NodeExprMinus
	 */
	public void handleNodeExprMinus(NodeExprMinus node) throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeExprMinus"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.leftChild()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.rightChild()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
	}

	/**
	 * Handles visiting a NodeExprPlus
	 */
	public void handleNodeExprPlus(NodeExprPlus node) throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeExprPlus"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.leftChild()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.rightChild()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
	}

	/**
	 * Handles visiting a NodeExprTimes
	 */
	public void handleNodeExprTimes(NodeExprTimes node) throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeExprTimes"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.leftChild()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.rightChild()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
	}

	/**
	 * Handles visiting a NodeExprDivide
	 */
	public void handleNodeExprDivide(NodeExprDivide node) throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeExprDivide"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.leftChild()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.rightChild()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
	}
	
	/**
	 * Handles visiting a NodeExprMod
	 */
	public void handleNodeExprMod(NodeExprMod node) throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeExprMod"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.leftChild()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.rightChild()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.leftChild());
		checkAndVisit(node.rightChild());
	}

	/**
	 * Handles visiting a NodeExprVar
	 */
	public void handleNodeExprVar(NodeExprVar node) throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeExprVar: " + node.variable()
					+ "\"];");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
	}

	/**
	 * Handles visiting a NodeInstrAssignArray
	 */
	public void handleNodeInstrAssignArray(NodeInstrAssignArray node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrAssignArray: " + node.identifier()
					+ "[]\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.index()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.expression()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		//System.out.println("AssignArray Index:");
		checkAndVisit(node.index());
		//System.out.println("AssignArray Expr:");
		checkAndVisit(node.expression());
	}

	/**
	 * Handles visiting a NodeInstrAssignSingle
	 */
	public void handleNodeInstrAssignSingle(NodeInstrAssignSingle node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrAssignSingle: " + node.identifier()
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.expression()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.expression());
	}

	/**
	 * Handles visiting a NodeInstrBlock
	 */
	public void handleNodeInstrBlock(NodeInstrBlock node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrBlock: "
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.list()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		//System.out.println("Block Body:");
		checkAndVisit(node.list());
	}

	/**
	 * Handles visiting a NodeInstrIfthen
	 */
	public void handleNodeInstrIfthen(NodeInstrIfthen node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrIfthen: "
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.relation()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.thenInstr()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		
		//System.out.println("If Relation:");
		checkAndVisit(node.relation());
		//System.out.println("Then Body:");
		checkAndVisit(node.thenInstr());
	}

	/**
	 * Handles visiting a NodeInstrIfthenelse
	 */
	public void handleNodeInstrIfthenelse(NodeInstrIfthenelse node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrIfthenelse: "
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.relation()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.thenInstr()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.elseInstr()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		
		//System.out.println("If Relation:");
		checkAndVisit(node.relation());
		//System.out.println("Then Body:");
		checkAndVisit(node.thenInstr());
		//System.out.println("Else Body:");
		checkAndVisit(node.elseInstr());
	}

	/**
	 * Handles visiting a NodeInstrOutput
	 */
	public void handleNodeInstrOutput(NodeInstrOutput node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrOutput:"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.expr()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.expr());
	}
	
	/**
	 * Handles visiting a NodeInstrInputArray
	 */
	public void handleNodeInstrInputArray(NodeInstrInputArray node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrInputArray: " + node.identifier()
					+ "[]\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.index()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.index());
	}
	
	/**
	 * Handles visiting a NodeInstrInputSingle
	 */
	public void handleNodeInstrInputSingle(NodeInstrInputSingle node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrInputSingle: " + node.identifier()
					+ "\"];");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
	}

	/**
	 * Handles visiting a NodeInstrWhile
	 */
	public void handleNodeInstrWhile(NodeInstrWhile node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeInstrWhile"
					+ "\"];");
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.relation()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.instr()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		
		//System.out.println("While Relation:");
		checkAndVisit(node.relation());
		//System.out.println("While Body:");
		checkAndVisit(node.instr());
	}

	/**
	 * Handles visiting a NodeListInstrCons
	 */
	public void handleNodeListInstrCons(NodeListInstrCons node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeListInstrCons"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.head()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.tail()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		
		checkAndVisit(node.head());
		checkAndVisit(node.tail());
	}

	/**
	 * Handles visiting a NodeListInstrNil
	 */
	public void handleNodeListInstrNil(NodeListInstrNil node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeListInstrNil "
					+ "\"];");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
	}

	/**
	 * Handles visiting a NodeProgram
	 */
	public void handleNodeProgram(NodeProgram node) throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeProgram"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.listInstr()) + ";");
			out.newLine();
			
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.listInstr());
	}

	/**
	 * Handles visiting a NodeRelationLessThan
	 */
	public void handleNodeRelationLessThan(NodeRelationLessThan node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeRelationLessThan"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.left()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.right()) + ";");
			out.newLine();			
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.left());
		checkAndVisit(node.right());
	}

	/**
	 * Handles visiting a NodeRelationLessThanEqual
	 */
	public void handleNodeRelationLessThanEqual(NodeRelationLessThanEqual node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeRelationLessThanEqual"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.left()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.right()) + ";");
			out.newLine();			
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.left());
		checkAndVisit(node.right());
	}
	
	/**
	 * Handles visiting a NodeRelationEqual
	 */
	public void handleNodeRelationEqual(NodeRelationEqual node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeRelationEqual"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.left()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.right()) + ";");
			out.newLine();	
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.left());
		checkAndVisit(node.right());
	}

	/**
	 * Handles visiting a NodeRelationGreaterThanEqual
	 */
	public void handleNodeRelationGreaterThanEqual(NodeRelationGreaterThanEqual node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeRelationGreaterThanEqual"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.left()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.right()) + ";");
			out.newLine();	
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.left());
		checkAndVisit(node.right());
	}

	/**
	 * Handles visiting a NodeRelationGreaterThan
	 */
	public void handleNodeRelationGreaterThan(NodeRelationGreaterThan node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeRelationGreaterThan"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.left()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.right()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.left());
		checkAndVisit(node.right());
	}

	/**
	 * Handles visiting a NodeRelationAnd
	 */
	public void handleNodeRelationAnd(NodeRelationAnd node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeRelationAnd"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.left()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.right()) + ";");
			out.newLine();	
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.left());
		checkAndVisit(node.right());
	}

	/**
	 * Handles visiting a NodeRelationOr
	 */
	public void handleNodeRelationOr(NodeRelationOr node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeRelationOr"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.left()) + ";");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.right()) + ";");
			out.newLine();	
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.left());
		checkAndVisit(node.right());
	}

	/**
	 * Handles visiting a NodeRelationNot
	 */
	public void handleNodeRelationNot(NodeRelationNot node)
	throws VisitorException {
		try {
			out.write(getIdentifier(node) + "[label=\"NodeRelationNot"
					+ "\"];");
			out.newLine();
			
			out.write(getIdentifier(node) + " -> " + getIdentifier(node.relation()) + ";");
			out.newLine();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println(e);
		}
		checkAndVisit(node.relation());
	}
}
