
/**
	* This class represents a numeric literal in an expression.
	* 
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
  * Typing class that represents a numeric literal in an expression.
  */
 
public class NodeExprInt extends NodeArithmeticExpr
{
	private int _value;
	 
	/**
	 * Creates a NodeExprInt, which represents a literal integer 
	 * with value value.
	 */
	public NodeExprInt( int value )
	{
		_value = value;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeExprInt(this);
	}
	 
	/**
	 * Returns the value of the Integer.
	 */
	public int value()
	{
		return _value;
	}
}
 
