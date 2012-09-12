
/**
 * This class is the root of the parse tree.
 *
 * @version foo
 * @author Mike Boilen (mgb)
 */
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 

/**
 * Typing class that represents the Program in all its glory.
 */
 
public class NodeProgram extends Node
{
	private NodeListInstr _listInstr;
	 
	/**
	 * Creates a NodeProgram, which represents the Program, with
	 * linked list of declatations listDecl and linked list of 
	 * instructions listInstr.
	 */
	public NodeProgram(
  	NodeListInstr listInstr
	)
	{
		_listInstr = listInstr;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeProgram(this);
	}
	 
	/**
	 * Returns the NodeListDecl that represents the list of instructions.
	 */
	public NodeListInstr listInstr()
	{
		return _listInstr;
	}
}
 
 
