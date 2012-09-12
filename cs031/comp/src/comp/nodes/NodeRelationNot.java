
      /**
	* This class holds a not relation.
	* 
	* @version foo
	* @author Jon Natkins (jnatkins)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents a not condition.
 */
 
public class NodeRelationNot extends NodeRelation
{
	private NodeExpr _relation;
	 
	/**
	 * Creates a NodeRelationNot, which represents a not 
	 * condition with left-hand-side left and right-hand-side right:<br>
	 * left &lt; right
	 */
	public NodeRelationNot( NodeExpr relation )
	{
		_relation = relation;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeRelationNot(this);
	}
	 
	/**
	 * Returns the NodeExpr that represents the negated expression
	 */
	public NodeExpr relation()
	{
		return _relation;
	}
}
 
 
