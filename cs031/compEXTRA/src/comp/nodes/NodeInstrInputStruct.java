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
public class NodeInstrInputStruct extends NodeInstr
{
	private String id;
	private NodeVariableExpr var;

	/**
	 *
	 */
	public NodeInstrInputStruct( String id, NodeVariableExpr var )
	{
		this.id = id;
		this.var = var;
	}

	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrInputStruct( this );
	}

	/**
	 * Returns the identifier for the variable being set.
	 */
	public String structName()
	{
		return id;
	}

	public NodeVariableExpr variable(){
		return var;
	}
}
