
/**
	* This class holds a less than relation.
	* 
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents a less-than condition.
 */
 
public class NodeRelationLessThan extends NodeRelation
{
	private NodeExpr _left;
	private NodeExpr _right;
	 
	/**
	 * Creates a NodeRelationLessThan, which represents a less-than
	 * condition with left-hand-side left and right-hand-side right:<br>
	 * left &lt; right
	 */
	public NodeRelationLessThan( NodeExpr left,NodeExpr right )
	{
		_left = left;
		_right = right;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeRelationLessThan(this);
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
 
 
