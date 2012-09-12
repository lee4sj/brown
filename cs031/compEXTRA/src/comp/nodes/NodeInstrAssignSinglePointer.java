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
public class NodeInstrAssignSinglePointer extends NodeInstrAssignSingle {

	public NodeInstrAssignSinglePointer( String id,NodeExpr expr ){
		super(id, expr);
	}

	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrAssignSinglePointer( this );
	}
}
