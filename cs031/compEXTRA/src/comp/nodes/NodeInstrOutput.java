
/**
	* This class represents an output instruction.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
 
/**
 * Typing class that represents an output instruction.
 */
 
public class NodeInstrOutput extends NodeInstr
{
	private NodeExpr _expr;
	 
	/**
	 * Creates a NodeInstrOutput, which represents an output instruction,
	 * with value to output expr.
	 */
	public NodeInstrOutput( NodeExpr expr )
	{
		_expr = expr;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrOutput(this);
	}
	 
	/**
	 * Returns the NodeExpr that represents the value to output.
	 */
	public NodeExpr expr()
	{
		return _expr;
	}
}
 
