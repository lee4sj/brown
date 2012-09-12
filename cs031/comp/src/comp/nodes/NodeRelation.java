package comp.nodes;

import comp.SemanticType;
import comp.Visitor;
import comp.VisitorException;
 
 
/**
 * A typing superclass to make sure that any node that should be a
 * relation is, in fact, a relation.  Anything of type NodeRelation
 * will actually be of class NodeRelationSmaller, or another subclass
 * of NodeRelation.  
 *   
 * @version   Tue Nov 18 23:28:30 1997
 * @author Mark Handy
 * @author Mike Boilen (mgb)
 */
public abstract class NodeRelation extends NodeExpr
{
	public abstract void visit( Visitor visitor ) throws VisitorException;
	/**
	 * All relational expressions evaluate to booleans.
	 */
	public SemanticType semanticType() {
		return SemanticType.BOOLEAN;
	}
}
 
