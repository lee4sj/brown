/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
import comp.ExprType;

//TODO: comment

/**
 *
 * @author sl136
 */
public class NodeExprStruct extends NodeVariableExpr {

	private NodeVariableExpr var;

	public NodeExprStruct(String id, NodeVariableExpr var){
		super(id);
		this.var = var;
	}


	public NodeVariableExpr variableInStruct(){
		return var;
	}

	public void visit(Visitor visitor) throws VisitorException{
		visitor.handleNodeExprStruct(this);
	}

	@Override
	public ExprType exprType(){
		return ExprType.STRUCT;
	}
}
