/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
/**
 *
 * @author sl136
 */
public class NodeInstrDeclStruct extends NodeInstrDecl {

	String structName;

	public NodeInstrDeclStruct( String name, String struct){
		super(name);
		this.structName = struct;
	}

	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrDeclStruct(this);
	}


}
