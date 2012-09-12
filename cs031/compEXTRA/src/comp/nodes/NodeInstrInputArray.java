
/**
	* This class represents an array input instruction.
	*
	* @version foo
	* @author Jon Natkins (jnatkins)
	*/
package comp.nodes;

import comp.Visitor;
import comp.VisitorException;


/**
 * Typing class that represents an array input instruction.
 */
public class NodeInstrInputArray extends NodeInstr
{
	private String _id;
	private NodeExpr _index;

	/**
	 * Creates a NodeInstrInputArray, which represents an array input
	 * instruction, with array identifier id and an index index:<br>
	 * input(id[index]);
	 */
	public NodeInstrInputArray( String id,NodeExpr index )
	{
		_id = id;
		_index = index;
	}

	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrInputArray(this);
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
}
