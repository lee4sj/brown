
/**
	* This class represents a division expression.
	*
	* @version foo
	* @author Jon Natkins (jnatkins)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents a division expression.
 */
 
public class NodeExprDivide extends NodeArithmeticExpr
{
	private NodeExpr _left;
	private NodeExpr _right;
	 
	/**
	 * Creates a NodeExprDivide, which represents a division expression,
	 * with left-hand-size left and right-hand-side right.
	 */
	public NodeExprDivide( NodeExpr left,NodeExpr right )
	{
		_left = left;
		_right = right;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeExprDivide(this);
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
 
