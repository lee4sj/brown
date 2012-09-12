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
public class NodeExprVarDeref extends NodeExprVar
{
	public NodeExprVarDeref(String var){
		super(var);
	}

	public void visit( Visitor visitor ) throws VisitorException{
		visitor.handleNodeExprVarDeref(this);
	}
}
