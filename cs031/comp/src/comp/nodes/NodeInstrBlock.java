
/**
	* This class represents a block of instructions.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
 
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;

/**
 * Typing class that represents a block of instructions.
 */
 
 
public class NodeInstrBlock extends NodeInstr
{
	private NodeListInstr _linstr;
	 
	/**
	 * Creates a NodeInstrBlock, which represents a block of instructions,
	 * with list of instructions listr.
	 */
	public NodeInstrBlock( NodeListInstr linstr )
	{
		_linstr = linstr;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrBlock(this);
	}
	 
	/**
	 * Returns the NodeListInstr that represents the list of instructions
	 * to execure in this block.
	 */
	public NodeListInstr list()
	{
		return _linstr;
	}
}
 
