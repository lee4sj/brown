package comp.nodes;

import comp.SemanticType;
import comp.Visitor;
import comp.VisitorException;

public abstract class NodeArithmeticExpr extends NodeExpr {
	public abstract void visit( Visitor visitor ) throws VisitorException;
	/**
	 * All arithmetic expressions evaluate to integers.
	 */
	public SemanticType semanticType() {
		return SemanticType.INTEGER;
	}
}
