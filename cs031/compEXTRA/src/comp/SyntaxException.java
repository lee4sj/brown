package comp;

/**
 * Signals an error in the form of the program discovered by the parser. Note
 * that it requires a line number, which can be gotten by calling
 * Tokenizer.currentLine().
 * 
 * @version Tue Nov 18 23:36:39 1997
 * @author The TAs
 */
public class SyntaxException extends Exception {

    public SyntaxException(int linenum, String message) {
	// Important!
	// Do not remove "SyntaxException" from the below line - it will disrupt test suites and you WILL lose points
        super("SyntaxException: syntax error in line " + linenum + ": " + message);
    }
}
