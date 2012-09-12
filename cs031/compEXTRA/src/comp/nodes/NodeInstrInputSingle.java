

package comp.nodes;

import comp.Visitor;
import comp.VisitorException;

/**
 * Typing class that represents a singleton variable input instruction.
 */


public class NodeInstrInputSingle extends NodeInstr
{
	private String _id;

	/**
	 * Creates a NodeInstrInputSingle, which represents a singleton variable
	 * input instruction, with variable identifier id:<br>
	 * input(id)
	 */
	public NodeInstrInputSingle( String id )
	{
		_id = id; // _id is the name of the variable being assigned to
	}

	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrInputSingle( this );
	}

	/**
	 * Returns the identifier for the variable being set.
	 */
	public String identifier()
	{
		return _id;
	}
}
