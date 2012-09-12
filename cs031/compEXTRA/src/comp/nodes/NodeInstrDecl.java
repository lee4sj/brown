package comp.nodes;
 
 
/**
 * Just a typing class to make sure that a node that should be a
 * declaration is, in fact, a declaration.  Anything of type NodeInstrDecl
 * will actually be either a NodeInstrDeclArray or a NodeInstrDeclSingle.
 * 
 * @author Mark Handy
 */
public abstract class NodeInstrDecl extends NodeInstr
{
	private String _id;

	public NodeInstrDecl(String id){
		_id = id;
	}

	/**
	 * returns the name of this variable
	 *
	 * @return	String, the name of this variable
	 */
	public String identifier()
	{
		return _id;
	}

	public void changeName(String name){
		_id = name;
	}
}
 
 
