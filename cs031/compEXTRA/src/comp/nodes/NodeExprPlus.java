
/**
	* This class represents an addition expression.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;

/**
 * Typing class that represents an addition expression.
 */
 
 
public class NodeExprPlus extends NodeArithmeticExpr
{
	private NodeExpr _left;
	private NodeExpr _right;
	 
	/**
	 * Creates a NodeExprPlus, which represents an addition expression,
	 * with left-hand-side left and right-hand-side right.
	 */
	public NodeExprPlus( NodeExpr left,NodeExpr right )
	{
		_left = left;
		_right = right;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeExprPlus(this);
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
 
