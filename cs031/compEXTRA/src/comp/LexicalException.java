package comp;

/**
 * This class signals a lexical problem (illegal character or unrecognized
 * token) in the input Blaise file.
 * 
 * @version Fall 2007
 * @author The TAs
 */
public class LexicalException extends Exception {

    /**
     * Creates a new <code>LexicalException</code>. It uses the line number and
     * string to generate an error message.
     *
     * @param linenum The line number on which the error occured.
     * @param message A description of the error that occured.
     */
    public LexicalException(int linenum, String message) {
	// Important!
	// Do not remove "LexicalException" from the below line - it will disrupt test suites and you WILL lose points
        super("LexicalException: lexical error in line " + linenum + ": " + message);
    }
    
}
