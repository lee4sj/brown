package comp;

/**
 * This exception class signals an error in the declaration or use of a
 * variable. Note that it does not need a line number.
 * 
 * @version Tue Nov 18 23:35:52 1997
 * @author The TAs
 */
public class SemanticException extends VisitorException {

    public SemanticException(String message) {
	// Important!
	// Do not remove "SemanticException" from the below line - it will disrupt test suites and you WILL lose points
        super("SemanticException: semantic error: " + message);
    }
}
