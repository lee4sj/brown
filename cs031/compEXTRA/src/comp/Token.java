package comp;

/**
 * Single class to represent all the various kinds of tokens that can legally be
 * present in a Blaise file. A token is either a single significant character or
 * a string of characters that can be treated as a single entity for parsing
 * purposes. Tokens are typically separated by white space. The tokenizing is a
 * little more sophisticated than just looking for white space, however, because
 * something like "myVariable=b;" is scanned as four tokens (myBariable, =, b, ;)
 * <p>
 * TokenType.ID tokens have a name, which you can get with getName().
 * <p>
 * TokenType.INTEGER tokens have a value, which you can get with getValue().
 * 
 * @version Tue Oct 21 18:00:05 1997
 * @author The TAs
 */
public class Token {

    /**
     * The type of the token, from the TokenType enumeration
     */
    private TokenType _type;

    /**
     * Token's name when it is of type TokenType.ID
     */
    private String _name;

    /**
     * Token's value when it is of type TokenType.INTEGER
     */
    private int _value;

    /**
     * Constructor for a token which has only a type (i.e. no name or value).
     * 
     * @param type
     *           Type of the token
     */
    public Token(TokenType type) {
        _type = type;
    }

    /**
     * Constructor for a token which is an identifier -- i.e., a variable.
     * 
     * @param type
     *           Must be TokenType.ID
     * @param name
     *           Identifier's name (e.g., myVariable)
     */
    public Token(TokenType type, String name) {
        if (type != TokenType.ID)
            throw new InternalException("Token(tokenType,name): tokenType " +
                    "!= TokenType.ID.");
        _type = type;
        _name = name;
    }

    /**
     * Constructor for a token which is an integer. That is, the tokenizer
     * parses the string "123" into the integer 123 and instantiates a token
     * with this constructor.
     * 
     * @param type
     *           Must be TokenType.INTEGER from interface TokenType
     * @param value
     *           The value of the integer
     */
    public Token(TokenType type, int value) {
        if (type != TokenType.INTEGER)
            throw new InternalException("Token(tokenType,value): tokenType " +
                    "!= TokenType.INTEGER.");
        _type = type;
        _value = value;
    }

    /**
     * Print the type (and, if appropriate, the name or value) of the token to
     * standard error (which is usually the shell).
     */
	public void print() {
		System.err.println(toString());
	}

	/**
	 * Returns the type (and, if appropriate, the name or value) of the token
	 * as a string.
	 */

    public String toString() {
        switch (_type) {
        case ID:
            return "Identifier: " + _name;
        case INTEGER:
            return "Integer: " + _value;
        case LEFTPAREN:
            return "left parenthesis";
        case RIGHTPAREN:
            return "right parenthesis";
        case SEMICOLON:
            return "semicolon";
        case PLUS:
            return "plus";
        case MINUS:
            return "minus";
        case TIMES:
            return "times";
        case LESSTHAN:
            return "less than";
        case ASSIGN:
            return "assign";
        case LEFTBRACE:
            return "left brace";
        case RIGHTBRACE:
            return "right brace";
        case LEFTBRACKET:
            return "left bracket";
        case RIGHTBRACKET:
            return "right bracket";
        case INT:
            return "int (the letters i-n-t)";
        case IF:
            return "if";
        case WHILE:
            return "while";
        case ELSE:
            return "else";
        case ENDIF:
            return "endif";
        case OUTPUT:
            return "output";
		case DIVIDE:
			return "DIVIDE";
		case MODULUS:
			return "MODULUS";
		case AND:
			return "AND";
		case OR:
			return "OR";
		case NOT:
			return "NOT";
		case GREATERTHAN:
			return "GREATERTHAN";
		case LESSEQ:
			return "LESSEQ";
		case GREATEREQ:
			return "GREATEREQ";
		case EQUAL:
			return "EQUAL";
		case INPUT:
			return "INPUT";
        default:
            throw new InternalException("Bad token type: " + _type);
        }
    }

    /**
     * @return The type of this token
     */
    protected TokenType getType() {
        return _type;
    }

    /**
     * @return The identifier's name, if the token type is TokenType.ID
     */
    protected String getName() {
        if (_type != TokenType.ID)
            throw new InternalException("getName() called on " +
                    "non-TokenType.ID token.");
        return _name;
    }

    /**
     * @return The integer's value, if the token type is TokenType.INTEGER
     */
    protected int getValue() {
        if (_type != TokenType.INTEGER)
            throw new InternalException(
                    "getValue() called on non-TokenType.INTEGER token.");
        return _value;
    }

}
