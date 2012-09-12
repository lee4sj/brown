
/**
	* This class represents a while loop.
	*
	* @version foo
	* @author Mike Boilen
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
 
/**
 * Typing class that represents a while loop.
 */
 
public class NodeInstrWhile extends NodeInstr
{
	private NodeExpr _relation;
	private NodeInstr _instr;
	 
	/**
	 * Creates a NodeInstrWhile, which represents a while loop,
	 * with while condition relation and instruction to repeat instr.
	 */
	public NodeInstrWhile(
	NodeExpr relation,
  	NodeInstr instr )
	{
		_relation = relation;
		_instr = instr;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrWhile(this);
	}
	 
	/**
	 * Returns the NodeRelation that represents the while condition.
	 */
	public NodeExpr relation()
	{
		return _relation;
	}
	 
	/**
	 * Returns the NodeInstr that represents what instruction to repeat.
	 */
	public NodeInstr instr()
	{
		return _instr;
	}
}
 
