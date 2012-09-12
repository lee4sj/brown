
/**
	* This class models a variable from an array.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;

/**
 * Typing class that represents a variable from an array.
 */
 
public class NodeExprArrayVar extends NodeVariableExpr
{
	private NodeExpr _index;
	 
	/**
	 * Creates a NodeExprArrayVar, which represents an array with 
	 * identifier var and index of the array index.
	 */
	public NodeExprArrayVar( String var,NodeExpr index )
	{
		super(var);
		_index = index;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeExprArrayVar(this);
	}
	 

	/**
	 * Returns the index of the array for this expression.
	 */
	public NodeExpr index()
	{
		return _index;
	}

	
}
 
