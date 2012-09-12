/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package comp.nodes;

import comp.Visitor;
import comp.VisitorException;

//TODO: Comment
/**
 *
 * @author sl136
 */
public class NodeListDeclCons extends NodeListDecl{

	private NodeInstrDecl var;
	private NodeListDecl list;

	public NodeListDeclCons(NodeInstrDecl var, NodeListDecl list){
		this.var = var;
		this.list = list;

	}

	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeListDeclCons(this);
	}
}
