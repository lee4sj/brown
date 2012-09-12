
/**
	* This class represents a variable in an expression.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;

/**
 * Typing class that represents a single variable in an expression.
 */
 
 
public class NodeExprVar extends NodeArithmeticExpr
{
	private String _var;
	 
	/**
	 * Creates a NodeExprVar, which represents a single variable in an
	 * expression, with identifier var.
	 */
	public NodeExprVar( String var )
	{
		_var = var;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeExprVar(this);
	}
	 
	/**
	 * Returns the identifier for this variable.
	 */
	public String variable()
	{
		return _var;
	}
}
 
