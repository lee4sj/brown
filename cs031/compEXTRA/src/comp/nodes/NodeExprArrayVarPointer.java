/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
/**
 *
 * @author sl136
 */
public class NodeExprArrayVarPointer extends NodeExprArrayVar
{
	public NodeExprArrayVarPointer( String var,NodeExpr index ){
		super(var, index);
	}

	public void visit( Visitor visitor ) throws VisitorException{
		visitor.handleNodeExprArrayVarPointer(this);
	}
}
