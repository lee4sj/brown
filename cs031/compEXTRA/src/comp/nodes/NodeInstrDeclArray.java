

package comp.nodes;

import comp.Visitor;
import comp.VisitorException;

/**
 * Typing class that represents declaring an array type.
 */

public class NodeInstrDeclArray extends NodeInstrDecl
{
	private int _size;

	/**
	 * Creates a NodeInstrDeclArray with identifier id and size size.
	 */
	public	NodeInstrDeclArray(
	String id,
	int size
	)
	{
		super(id);
		_size = size;
	}
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrDeclArray(this);
	}

	/**
	 * Returns the size of this array.
	 */
	public int size()
	{
		return _size;
	}
}
