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
public class NodeListDeclNil extends NodeListDecl {

	public NodeListDeclNil(){

	}

	public void visit(Visitor visitor) throws VisitorException{
		visitor.handleNodeListDeclNil(this);
	}

}
