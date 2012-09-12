/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package comp.nodes;

import comp.SemanticType;
import comp.Visitor;
import comp.VisitorException;
import comp.ExprType;

/**
 *
 * @author sl136
 */
public abstract class NodeVariableExpr extends NodeExpr
{
	String id;

	public NodeVariableExpr(String id){
		this.id = id;
	}

	public abstract void visit( Visitor visitor ) throws VisitorException;
	/**
	 * All relational expressions evaluate to booleans.
	 */
	public SemanticType semanticType() {
		return SemanticType.INTEGER;
	}


	/**
	 *
	 * @return	the expressions type of this NodeExpr
	 */
	public ExprType exprType(){
		return ExprType.VARIABLE;
	}

	/**
	 *
	 * @return the name of this variable
	 */
	public String variable(){
		return id;
	}

	public void changeVariableName(String changeName){
		id = changeName;
	}

}
