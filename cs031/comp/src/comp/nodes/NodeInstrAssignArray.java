
/**
	* This class represents an array assignment instruction.
	*
	* @version foo
	* @author Mike Boilen (mgb)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
 
/**
 * Typing class that represents an array assignment instruction.
 */
public class NodeInstrAssignArray extends NodeInstr
{
	private String _id;
	private NodeExpr _index;
	private NodeExpr _expr;
	 
	/**
	 * Creates a NodeInstrAssignArray, which represents an array assignment
	 * instruction, with array identifier id, index index, and expression expr:<br>
	 * id[index] := expr
	 */
	public NodeInstrAssignArray( String id,NodeExpr index,NodeExpr expr )
	{
		_id = id;
		_index = index;
		_expr = expr;
	}
	 
	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrAssignArray(this);
	}
	 
	/**
	 * Returns the identifier of the array being assigned to.
	 */
	public String identifier()
	{
		return _id;
	}
	 
	/**
	 * Returns the index in the array.
	 */
	public NodeExpr index()
	{
		return _index;
	}
	 
	/**
	 * Returns the NodeExpr that represents the value being stored.
	 */
	public NodeExpr expression()
	{
		return _expr;
	}
}
 
