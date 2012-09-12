
/**
	* This class is the terminator for a list of instructions.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents a null/terminating entry in a linked list 
 * of instructions.
 */
 
public class NodeListInstrNil extends NodeListInstr
{
	/**
	 * Creates a terminator for a linked list of instructions.
	 */
	public NodeListInstrNil()
{ }
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeListInstrNil(this);
	}
}
 
 
 
