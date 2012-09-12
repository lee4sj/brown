/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
//TODO: comment

/**
 *
 * @author sl136
 */
public class NodeInstrDeclConsStruct extends NodeInstrDecl {

	private NodeListDecl varList;

	public NodeInstrDeclConsStruct(String name, NodeListDecl varList){
		super(name);
		this.varList = varList;
	}

	public void visit(Visitor visitor) throws VisitorException{
		visitor.handleNodeInstrDeclConsStruct(this);
	}

	public NodeListDecl variableList(){
		return varList;
	}
}
