package comp.nodes;

import comp.Visitor;
import comp.VisitorException;

/**
 * Typing class that represents declaring a single type.
 */

public class NodeInstrDeclSingle extends NodeInstrDecl
{
	private String _id;

	/**
	 * Creates a NodeInstrDeclSingle with identitifer id.
	 */
	public NodeInstrDeclSingle(
	String id
	)
	{
		_id = id;
	}

	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrDeclSingle(this);
	}

	/**
	 * Returns the identifier for this single.
	 */
	public String identifier()
	{
		return _id;
	}
}
