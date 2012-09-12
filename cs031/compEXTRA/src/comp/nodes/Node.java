package comp.nodes;

import comp.Visitor;
import comp.VisitorException;
 
/**
 * The abstract Node models the generic node of the syntax tree. The tree
 * is constructed via scanning and parsing and then traversed by the
 * compiler in the process of semantic analysis and code-generation;
 * concrete instances of the Node constitute the elements of this tree.
 * Like any abstract class, Node serves as the prototype for its
 * subclasses, each of which implements its inherited methods according
 * to its specific role in the model. Note that certain subclasses have the
 * inherited generate() method throw an exception, and define instead their
 * own task-specific method to generate code (consider the subclasses of
 * NodeExpr and of NodeRelation for details). Note also that the *Cons and
 * *Nil subclasses of Node do not represent linguistic structures of the
 * Blaise grammar - as do the other subclasses - but rather organizational
 * structures within the syntax tree: instances of *Cons organize the
 * elements of the subtrees below them into lists and instances of *Nil
 * mark the end of these lists. For more documentation on these and other
 * Node classes, please see the detailed handout.
 * 
 * @version Thu Oct 22 22:03:31 EDT 1998
 * @author Ilana Frankel 
 * @author Mike Boilen (mgb)
 *
 */
 
public abstract class Node
{
	/**
		* Handle the <code>Visitor</code>.  This method is responsible for
		* calling <code>visit</code> on the <code>Visitor</code>, and then
		* passing the <code>Visitor</code> to the children of this
		* <code>Node</code>, if any.
		*/
	public abstract void visit( Visitor visitor ) throws VisitorException;
	 
}
 
