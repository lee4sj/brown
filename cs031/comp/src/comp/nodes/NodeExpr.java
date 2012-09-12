
 
package comp.nodes;

import comp.SemanticType;
import comp.Visitor;
import comp.VisitorException;
 
 
/**
 * Typing superclass to make sure that any node that should be an
 * expression is, in fact, an expression.  Anything of type NodeExpr
 * will actually be of class, for example, NodeExprPlus, or another
 * subclass of NodeExpr.
 *   
 * @version   Tue Nov 18 23:21:18 1997
 * @author Mark Handy
 * @author Mike Boilen (mgb)
 * @author Andrew Oates
 */
abstract public class NodeExpr extends Node
{
	 
	abstract public void visit( Visitor visitor ) throws VisitorException;

	/**
	 * Returns the semantic type of the given expression.
	 */
	abstract public SemanticType semanticType();
}
 
