/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package comp.nodes;


//TODO: comment

import comp.Visitor;
import comp.VisitorException;
/**
 *
 * @author sl136
 */
public class NodeInstrAssignStruct extends NodeInstr {

	private String structName;
	private NodeVariableExpr var;
	private NodeExpr expr;

	public NodeInstrAssignStruct(String structName, NodeVariableExpr var, NodeExpr expr){
		this.structName = structName;
		this.var = var;
		this.expr = expr;
	}

	public String structName(){
		return structName;
	}

	public NodeVariableExpr variableInStruct(){
		return var;
	}

	public NodeExpr expression(){
		return expr;
	}

	public void visit(Visitor visitor) throws VisitorException{
		visitor.handleNodeInstrAssignStruct(this);
	}
}
