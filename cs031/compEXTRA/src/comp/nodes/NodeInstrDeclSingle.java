package comp.nodes;

import comp.Visitor;
import comp.VisitorException;

/**
 * Typing class that represents declaring a single type.
 */

public class NodeInstrDeclSingle extends NodeInstrDecl
{
	/**
	 * Creates a NodeInstrDeclSingle with identitifer id.
	 */
	public NodeInstrDeclSingle(
	String id
	)
	{
		super(id);
	}

	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrDeclSingle(this);
	}
}
