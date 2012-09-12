
/**
	* This class is a linked list structure that holds a list of instructions.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents a non-null entry in a linked list of instructions.
 */
 
public class NodeListInstrCons extends NodeListInstr
{
	private NodeInstr _head;
	private NodeListInstr _tail;
	 
	/**
	 * Creates a NodeListInstrCons, which represents an entry in a linked 
	 * list of instructions, with entry head and next-entry-pointer tail.
	 */
	public NodeListInstrCons(
  	NodeInstr head,
  	NodeListInstr tail
	)
	{
		_head = head;
		_tail = tail;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeListInstrCons(this);
	}
	 
	/**
	 * Returns the NodeInstr that represents this entry in the linked-list.
	 */
	public NodeInstr head()
	{
		return _head;
	}
	 
	/**
	 * Returns the NodeListInstr that represents the rest of the list.
	 */
	public NodeListInstr tail()
	{
		return _tail;
	}
}
 
 
