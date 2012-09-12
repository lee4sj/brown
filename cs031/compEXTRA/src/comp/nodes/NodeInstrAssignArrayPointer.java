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
public class NodeInstrAssignArrayPointer extends NodeInstrAssignArray {

	public NodeInstrAssignArrayPointer( String id,NodeExpr index,NodeExpr expr ){
		super(id, index, expr);
	}

	public void visit( Visitor visitor ) throws VisitorException
	{
		visitor.handleNodeInstrAssignArrayPointer(this);
	}

}
