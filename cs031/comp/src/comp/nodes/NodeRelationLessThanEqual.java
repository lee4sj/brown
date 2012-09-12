
/**
	* This class holds a less than or equal relation.
	* 
	* @version foo
	* @author Jon Natkins (jnatkins)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents a less than or equal condition.
 */
 
public class NodeRelationLessThanEqual extends NodeRelation
{
	private NodeExpr _left;
	private NodeExpr _right;
	 
	/**
	 * Creates a NodeRelationLessThanEqual, which represents a less than or
	 * equal condition with left-hand-side left and right-hand-side right:<br>
	 * left &lt; right
	 */
	public NodeRelationLessThanEqual( NodeExpr left,NodeExpr right )
	{
		_left = left;
		_right = right;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeRelationLessThanEqual(this);
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
 
 
