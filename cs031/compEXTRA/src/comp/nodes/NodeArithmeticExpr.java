package comp.nodes;

import comp.SemanticType;
import comp.Visitor;
import comp.VisitorException;
import comp.ExprType;

public abstract class NodeArithmeticExpr extends NodeExpr {
	public abstract void visit( Visitor visitor ) throws VisitorException;
	/**
	 * All arithmetic expressions evaluate to integers.
	 */
	public SemanticType semanticType() {
		return SemanticType.INTEGER;
	}


	/**
	 *
	 * @return	the expressions type of this NodeExpr
	 */
	public ExprType exprType(){
		return ExprType.ARITHMETIC;
	}
}
