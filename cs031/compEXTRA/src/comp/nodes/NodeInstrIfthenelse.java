
/**
	* This class represents an If-then-else instruction.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents an If-the-else instruction.
 */
 
public class NodeInstrIfthenelse extends NodeInstr
{
	private NodeExpr _relation;
	private NodeInstr _then;
	private NodeInstr _else;
	 
	/**
	 * Creates a NodeInstrIfthenelse, which represents an If-then-else
	 * instruction, with if condition relation, then instruction ithen,
	 * and else instruction ielse.
	 */
	public NodeInstrIfthenelse(
	NodeExpr relation,
  	NodeInstr ithen,
  	NodeInstr ielse )
	{
		_relation = relation;
		_then = ithen;
		_else = ielse;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrIfthenelse(this);
	}
	 
	/**
	 * Returns the NodeRelation that represents the if condition.
	 */
	public NodeExpr relation()
	{
		return _relation;
	}
	 
	/**
	 * Returns the NodeInstr that represents the then instruction.
	 */
	public NodeInstr thenInstr()
	{
		return _then;
	}
	 
	/**
	 * Returns the NodeInstr that represents the else instruction.
	 */
	public NodeInstr elseInstr()
	{
		return _else;
	}
}
 
