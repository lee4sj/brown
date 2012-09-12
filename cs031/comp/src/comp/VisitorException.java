package comp;

/**
 * Signals an error during a visitor's execution.
 * 
 * @author The TAs
 */
public class VisitorException extends Exception {

    public VisitorException(String message) {
	// Important!
	// Do not remove "VisitorException" from the below line - it will disrupt test suites and you WILL lose points
        super("VisitorException: Visitor error: " + message);
    }

}
