
/**
	* This class holds a greater than relation.
	* 
	* @version foo
	* @author Jon Natkins (jnatkins)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents a greater-than condition.
 */
 
public class NodeRelationGreaterThan extends NodeRelation
{
	private NodeExpr _left;
	private NodeExpr _right;
	 
	/**
	 * Creates a NodeRelationGreaterThan, which represents a greater-than
	 * condition with left-hand-side left and right-hand-side right:<br>
	 * left &lt; right
	 */
	public NodeRelationGreaterThan( NodeExpr left,NodeExpr right )
	{
		_left = left;
		_right = right;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeRelationGreaterThan(this);
	}
	 
	/**
	 * Returns the NodeExpr that represents the left-hand-side.
	 */
	public NodeExpr left()
	{
		return _left;
	}
	 
	/**
	 * Returns the NodeExpr that represents the right-hand-side.
	 */
	public NodeExpr right()
	{
		return _right;
	}
}
 
 
