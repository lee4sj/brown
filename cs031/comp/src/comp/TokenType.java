package comp;

/**
 * Enumeration for the Blaise token types.
 * 
 * @version Wed Oct 22 18:12:13 1997
 * @author The TAs
 */
public enum TokenType {

    // An identifier literal (variable name)
    ID,
    
    //An integer literal - e.g. 4. Not the same as INT, below
    INTEGER,

    // Operators
    PLUS, MINUS, TIMES, DIVIDE, MODULUS, LESSTHAN, AND, OR, NOT,
    GREATERTHAN, LESSEQ, GREATEREQ, EQUAL,

    // Control structures
    IF, WHILE, ELSE, ENDIF,
    
    // The string ":="
    ASSIGN,

    // The string "int"
    INT,

    // The string "output"
    OUTPUT, INPUT,

    // Punctuation
    LEFTPAREN, RIGHTPAREN, SEMICOLON, LEFTBRACE, RIGHTBRACE, LEFTBRACKET, RIGHTBRACKET,
    
    // End of file
    EOF
    
}
