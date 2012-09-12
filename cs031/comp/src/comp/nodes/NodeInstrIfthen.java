
/**
	* This represents an if-then instruction.
	*
	* @version foo
	* @author Mike Boilen
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
 
/**
 * Typing class that represents an if-then instruction.
 */
 
public class NodeInstrIfthen extends NodeInstr
{
	private NodeExpr _relation;
	private NodeInstr _then;
	 
	/**
	 * Creates a nodeInstrIfthen, which represents an if-then instruction, with conditional relation and then instruction ithen:<br>
	 * if(relation)<br>
	 * ithen
	 */
	public NodeInstrIfthen(
	NodeExpr relation,
  	NodeInstr ithen )
	{
		_relation = relation;
		_then = ithen;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrIfthen(this);
	}
	 
	/**
	 * Returns the NodeRelation that represents the if conditional.
	 */
	public NodeExpr relation()
	{
		return _relation;
	}
	 
	/**
	 * Returns the NodeInstr to be run if relation() is true.
	 */
	public NodeInstr thenInstr()
	{
		return _then;
	}
}
 
