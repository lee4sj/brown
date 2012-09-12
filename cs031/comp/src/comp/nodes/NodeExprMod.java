
/**
	* This class represents a modulo expression.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents a modulo expression.
 */
 
public class NodeExprMod extends NodeArithmeticExpr
{
	private NodeExpr _left;
	private NodeExpr _right;
	 
	/**
	 * Creates a NodeExprMod, which represents a modulo expression,
	 * with left-hand-size left and right-hand-side right.
	 */
	public NodeExprMod( NodeExpr left,NodeExpr right )
	{
		_left = left;
		_right = right;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeExprMod(this);
	}
	 
	/**
	 * Returns the NodeExpr for the left-hand-side.
	 */
	public NodeExpr leftChild()
	{
		return _left;
	}
	 
	/**
	 * Returns the NodeExpr for the right-hand-side.
	 */
	public NodeExpr rightChild()
	{
		return _right;
	}
}
 
