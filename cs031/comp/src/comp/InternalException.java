package comp;

/**
 * This exception indicates a buggy compiler. Throw an this class when there is
 * something wrong with the internals of the parser and you need an escape.
 * Throw other kinds of exceptions when there is something wrong with the input
 * to the compiler.
 * 
 * This class extends <code>RuntimeException</code> so that you don't need to
 * worry about handling <code>InternalException</code>s. Classes that extend
 * <code>Exception</code> have to be either caught or declared whenever they can
 * occur.
 * 
 * @version Fall 2007
 * @author The TAs
 */
public class InternalException extends RuntimeException {

    /**
     * Create a new <code>InternalException</code> with the given message.
     *
     * @param message A message to print when this <code>Exception</code> is
     * thrown.
     */
    public InternalException(String message) {
        super(message);
    }

}
