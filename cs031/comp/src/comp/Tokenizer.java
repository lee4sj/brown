
package comp;
 
import java.io.*; // we use file-input and stream-parsing classes
 
 
/**
 * Tokenizer reads a text file containing a program in the Blaise
 * language and chops up the series of characters into a series of
 * Tokens.  Each call of getToken() returns the next token in the
 * series.  Each token returned has a type, which you can case on to
 * parse the file.  When the tokenizer reaches the
 * end of the input file, the last token you get from getToken() will
 * have type TokenType.EOF.
 * <p>
 * getToken() throws a LexicalException if the input file contains a
 * series of characters that the Tokenizer doesn't recognize as a
 * valid token.  You will need to deal with this exception
 * intelligently.  If you're shaky on working with exceptions, see the
 * main(.) procedure for a refresher.  (getToken() also throws an
 * IOException if there is a problem in reading the input file,
 * but that is unlikely to happen.)
 * <p>
 * To see how this works, you can use the main(.) procedure to test
 * the Tokenizer.  Just invoke it with a file name like this: <br><code>
 * java compiler.Tokenizer myprogram.blaise
 * </code><br>The tokenizer will scan the file for tokens it recognizes and
 * print out each token's type (and value, if appropriate).  If your
 * file contains characters that aren't part of Blaise, you will get
 * an exception.  Otherwise, you will get a list of tokens that should
 * match, in order and type, the tokens in your program file.
 * <p>
 * You may find it useful to copy and paste the case statement in the
 * main(.) procedure for use in your code. 
 *   
 *
 * @version Mon Oct 27 16:10:05 EST 1997
 * @author The TAs
 */
public class Tokenizer
{
	 
	// minimal tokenizer provided by java - to be adapted for Blaise
	private StreamTokenizer _tokenizer;
	 
	private Reader _inputStream;
	 
	private int _lineCounter = 1; // we start on first line of file
	 
	// last token gotten from _tokenizer -- different from token type
	// returned to client
	private int _currentToken;
	 
	private boolean _done = false; // used to avoid reading past EOF
    
	 

    /**
     * Creates a {@link Tokenizer} from the given {@link Reader}.
     *
     * @param input The source from which to read input.
     */
    public Tokenizer( Reader input ) {
	_inputStream = input;
	_tokenizer = new StreamTokenizer( _inputStream );
		 
	// need to treat EOL (\n) separately in order to count lines.  After
	// line-counting is handled, however, we treat EOL as just more
	// white space
	_tokenizer.eolIsSignificant( true );
		 
	// "ordinary characters" are those treated by the _tokenizer as single-
	// character tokens; this includes the colon and equal sign, which
	// we combine by hand (in getToken()) into a single token :=
	_tokenizer.ordinaryChars( 33, 47 );
	_tokenizer.ordinaryChars( 58, 64 );
	_tokenizer.ordinaryChars( 91, 96 );
	_tokenizer.ordinaryChars( 123, 126 );
		 
	// in contrast, characters not specified above are considered parts
	// of either "words" or "numbers,"
    }
    
    /**
     * 
     * @param fileName Blaise program to scan for tokens
     */
    public Tokenizer( String fileName ) throws FileNotFoundException {
	this( new FileReader( fileName )); // throws FNFE
    }
    
    public Tokenizer()
    {
	this( new InputStreamReader( System.in ) );
    }
	 
	// auxiliary methods to getToken method
	private boolean eof()
	{
		if( _currentToken != StreamTokenizer.TT_EOF ) return false;
		_done = true; // getToken() will never work again
		return true;
	}
	 
	private boolean word()
	{ // where "word" is as described in constructor
		if( _currentToken == StreamTokenizer.TT_WORD ) return true;
		return false;
	}
	 
	private boolean number() throws LexicalException {
		// The _tokenizer accepts floating-point numbers, but the
		// Tokenizer shouldn't, because Blaise doesn't.
		if( _currentToken == StreamTokenizer.TT_NUMBER )
		{
			// make sure there is no difference between the value and the
			// rounded value.
			if( _tokenizer.nval == ( float )( ( int )_tokenizer.nval ) ) return true;
			else throw new LexicalException
				( currentLine(), "FLOATING POINT NUMBERS NOT PART OF LANGUAGE" );
		}
		return false;
	}
	 
	private String sval()
	{
		// assumes that _tokenizer is currently looking at a "word" token
		return _tokenizer.sval;
	}
	 
	private int nval()
	{
		// assumes that _tokenizer is currently looking at a "number" token --
		// must call number() to make sure the "number" is an integer, BEFORE
		// doing the cast below, which will work by rounding
		return ( int )_tokenizer.nval;
	}
	 
	 
	 
	 
	 
	/**
	 * A token is either a single significant character or a sequence of
	 * characters that are significant only when taken together.  A left
	 * parenthesis is an example of the first kind.  A variable name is an
	 * example of the second kind -- the letters are meaningless as
	 * individual letters.
	 * 
	 * @return The next Token in the input file
	 */
	public Token getToken() throws IOException, LexicalException {
		 
		if ( _done ) throw new LexicalException
			( currentLine(), "getToken() called after EOF reached." );
		 
		_currentToken = _tokenizer.nextToken();
		 
		// count lines -- while-loop to allow multiple blank lines
		while ( _currentToken == StreamTokenizer.TT_EOL )
		{
			_lineCounter++;
			_currentToken = _tokenizer.nextToken();
		}
		 
		if( eof() ) return new Token( TokenType.EOF );
		 
		if( word() )
		{
			if( sval().equals( "if" ) ) return new Token( TokenType.IF );
			if( sval().equals( "while" ) ) return new Token( TokenType.WHILE );
			if( sval().equals( "int" ) ) return new Token( TokenType.INT );
			if( sval().equals( "else" ) ) return new Token( TokenType.ELSE );
			if( sval().equals( "endif" ) ) return new Token( TokenType.ENDIF );
			if( sval().equals( "output" ) ) return new Token( TokenType.OUTPUT );
			if( sval().equals( "input" ) ) return new Token( TokenType.INPUT );
			return new Token( TokenType.ID, sval() );
		}
		 
		if( number() ) return new Token( TokenType.INTEGER, nval() );
		 
		switch( _currentToken )
		{
			case 33 : return new Token( TokenType.NOT );
			case 37 : return new Token( TokenType.MODULUS );
			case 40 : return new Token( TokenType.LEFTPAREN );
			case 41 : return new Token( TokenType.RIGHTPAREN );
			case 42 : return new Token( TokenType.TIMES );
			case 43 : return new Token( TokenType.PLUS );
			case 45 : return new Token( TokenType.MINUS );
			case 47 : return new Token( TokenType.DIVIDE );
				 
			case 58 : // 58 == ':' so need to make sure it's followed by =
				_currentToken = _tokenizer.nextToken();
				if ( _currentToken != 61 ) throw new LexicalException
					( currentLine(), "Character ':' not followed by character '='." );
				return new Token ( TokenType.ASSIGN );
				 

			case 38 : // 38 == '&' so need to make sure it's followed by &
				_currentToken = _tokenizer.nextToken();
				if ( _currentToken != 38 ) throw new LexicalException
					( currentLine(), "Character '&' not followed by character '&'." );
				return new Token ( TokenType.AND );

			case 124 : // 124 == '|' so need to make sure it's followed by |
				_currentToken = _tokenizer.nextToken();
				if ( _currentToken != 124 ) throw new LexicalException
					( currentLine(), "Character '|' not followed by character '|'." );
				return new Token ( TokenType.OR );
				

			case 60 : // 60 == '<' so see if it's followed by =
				_currentToken = _tokenizer.nextToken();
				if ( _currentToken != 61 ) {
					_tokenizer.pushBack();
					return new Token( TokenType.LESSTHAN );
				}
				return new Token ( TokenType.LESSEQ );


			case 62 : // 62 == '>' so see if it's followed by =
				_currentToken = _tokenizer.nextToken();
				if ( _currentToken != 61 ) {
					_tokenizer.pushBack();
					return new Token( TokenType.GREATERTHAN );
				}
				return new Token ( TokenType.GREATEREQ );

			case 61 : // 61 == '=' so need to make sure it's followed by =
				_currentToken = _tokenizer.nextToken();
				if ( _currentToken != 61 ) throw new LexicalException
					( currentLine(), "Character '=' not followed by character '='." );
				return new Token ( TokenType.EQUAL );
				
			case 59 : return new Token( TokenType.SEMICOLON );
			case 91 : return new Token( TokenType.LEFTBRACKET );
			case 93 : return new Token( TokenType.RIGHTBRACKET );
			case 123 : return new Token( TokenType.LEFTBRACE );
			case 125 : return new Token( TokenType.RIGHTBRACE );
			default : throw new LexicalException
				( currentLine(), "ILLEGAL CHARACTER: " + ( char )_currentToken );
		}
	}
	 
	 
	/**
	 * The Tokenizer keeps track of the line number in the input file by counting
	 * the \n's it sees.
	 * @return Number of \n's seen so far, plus one
	 */
	public int currentLine()
	{
		return _lineCounter;
	}
	 
	 
	/**
	 * Give main a file name on the command line, and it will scan the
	 * file for tokens, printing out the type (and value, if appropriate)
	 * of each token it finds.
	 * @param argv[] Array of command-line arguments
	 */
	public static void main ( String argv[] )
	{

	if (argv.length<1) {
	  System.out.println ("Must specify the name of a file to scan.");
	  System.exit (1);
	} 

	Tokenizer tokr = null; // i hate the compiler
	try {
		  tokr = new Tokenizer (argv[0]);
	} catch (java.io.FileNotFoundException e) {
		  System.out.println ("File not found.");
		  System.exit (1);
	}
//		Tokenizer tokr = new Tokenizer();
		 
		// main loop -- just get and print tokens till there aren't any more
		try
		{
			Token t = tokr.getToken();
			 
			while ( t.getType() != TokenType.EOF )
			{
				t.print();
				t = tokr.getToken();
			}
		}
		 
		catch ( LexicalException e )
		{
			System.out.println ( e.getMessage() );
			System.exit ( 1 );
		}
		catch ( InternalException e )
		{
			System.out.println ( "Internal error (in scanner, not in input file):\n "
                     			+ e.getMessage() );
			System.exit ( 1 );
		}
		catch ( IOException e )
		{
			System.out.println ( "IOException from the FileReader used by Tokenizer:\n "
                     			+ e.getMessage() );
			System.exit ( 1 );
		}
		 
	} // end main
	 
	 
} // end class def
