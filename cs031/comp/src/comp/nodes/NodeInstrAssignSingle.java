
 
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * Typing class that represents a singleton variable assignment instruction.
 */
 
 
public class NodeInstrAssignSingle extends NodeInstr
{
	private String _id;
	private NodeExpr _expr;
	 
	/**
	 * Creates a NodeInstrAssignSingle, which represents a singleton variable
	 * assignment instruction, with variable identifier id and expression expr:<br>
	 * id := expr
	 */
	public NodeInstrAssignSingle( String id,NodeExpr expr )
	{
		_id = id; // _id is the name of the variable being assigned to
		 
		_expr = expr; // _expr is a link to the expression tree for
		// the expression being assigned to the variable.
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrAssignSingle( this );
	}
	 
	/**
	 * Returns the identifier for the variable being set.
	 */
	public String identifier()
	{
		return _id;
	}
	
	/**
	 * Returns the NodeExpr that represents the value being stored.
	 */
	public NodeExpr expression()
	{
		return _expr;
	}
	 
}
 
