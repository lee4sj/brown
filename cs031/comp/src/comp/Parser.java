package comp;

import java.io.FileNotFoundException;
import java.io.IOException;

import comp.nodes.*;

/**
 * Builds a tree of Nodes from a token stream. The syntax tree is rooted at a
 * NodeProgram. Once built, the tree can be used to do semantic analysis and
 * code generation.
 * <p>
 * Parser relies on a tokenizer for Blaise, which is available as class
 * Tokenizer.
 * <p>
 * You will probably want to add a good number of methods to this class to make
 * parsing possible.
 * 
 * @version Fri Nov 7 12:18:59 1997
 * @modified Mark Handy
 */
public class Parser {

    /**
     * Provides input to the Parser.
     */
    private Tokenizer _tokenizer;

    /**
     * The token currently being examined.
     */
    private Token _currentToken;

    /**
     * The next token in the token stream.
     */
    private Token _nextToken;

    /**
     * Opens the file specified and advances the token stream to populate
     * _nextToken.
     * 
     * @param name
     *           Name of file to parse
     */
    public Parser(String name) throws LexicalException, IOException {
        try {
            _tokenizer = new Tokenizer(name);
        } catch (FileNotFoundException e) {
            System.out.println("Error! File " + name + " not found!");
            System.exit(1);
        }
        advanceTokenStream();
    }

    /**
     * Creates a Parser, presumably taking input from stdin.
     */
    public Parser() throws IOException, LexicalException {
        _tokenizer = new Tokenizer();
        advanceTokenStream();
    }

    /**
     * Top-level method, called by client of the Parser to produce the syntax
     * tree.
     * 
     * @return NodeProgram at the root of the syntax tree
     */
    public NodeProgram parse() throws SyntaxException, LexicalException,
            IOException {
        if(_currentToken == null)
            advanceTokenStream();
        return new NodeProgram(listInstr());
    }


	/**
	 * Parses the current token stream to an instruction.
	 * Deals with (Instr) of the grammar.
	 *
	 * @return NodeInstr, the parsed instruction node.
	 * @throws comp.SyntaxException
	 * @throws comp.LexicalException
	 * @throws java.io.IOException
	 */
    private NodeInstr instr() throws SyntaxException, LexicalException,
            IOException{

        // <Inst> ::= <Decl>
        if(_currentToken.getType() == TokenType.INT){
            return decl();
        }

        //<Instr> ::= ID <IDSfx> := <Expr> ;
        else if(_currentToken.getType() == TokenType.ID){
            String name = _currentToken.getName();
            NodeExpr idsfx = null;

            advanceTokenStream();

            //dealing with <IDSfx> part
            if(_currentToken.getType() == TokenType.LEFTBRACKET){
                advanceTokenStream();
                idsfx = expr();
                if(_currentToken.getType() != TokenType.RIGHTBRACKET)
                    die(" ']' expected!");
                advanceTokenStream();
            }

            // := part
            if(_currentToken.getType() != TokenType.ASSIGN)
                die("  ':=' expected!!");

            advanceTokenStream();

            // <Expr> part
            NodeExpr ex = expr();

            // ; part
            if(_currentToken.getType() != TokenType.SEMICOLON)
                die(" ';' expected!");

            advanceTokenStream();

            //return NodeInstrAssign
            if(idsfx == null)
                return new NodeInstrAssignSingle(name, ex);
            else
                return new NodeInstrAssignArray(name, idsfx, ex);

        }

        // <Inst> ::= if (<Expr>) <Instr> <ElseInstr> endif
        else if(_currentToken.getType() == TokenType.IF){
            advanceTokenStream();

            //check (
            if(_currentToken.getType() != TokenType.LEFTPAREN)
                die (" ( expected!");

            advanceTokenStream();


            //check <Expr>
            NodeExpr ex = expr();

            //check )
            if(_currentToken.getType() != TokenType.RIGHTPAREN)
                die(" ) expected!");

            advanceTokenStream();

            //check <Instr>
            NodeInstr inst = instr();

            //check <ElseInstr>
            NodeInstr elseInst = null;
            if(_currentToken.getType() == TokenType.ELSE){
                advanceTokenStream();
                elseInst = instr();
            }

            //check endif
            if(_currentToken.getType() != TokenType.ENDIF)
                die ("endif expected!");
            advanceTokenStream();

            //return NodeInstrIfthen or NodeInstrIfthenElse
            if(elseInst == null)
                return new NodeInstrIfthen(ex, inst);
            else
                return new NodeInstrIfthenelse(ex, inst, elseInst);
        }

        // <Inst> ::= while (<Expr>) <Instr>
        else if(_currentToken.getType() == TokenType.WHILE){
            advanceTokenStream();

            //check (
            if(_currentToken.getType() != TokenType.LEFTPAREN)
                die (" ( expected!");

            advanceTokenStream();

             //check <Expr>
            NodeExpr ex = expr();

            //check )
            if(_currentToken.getType() != TokenType.RIGHTPAREN)
                die(" ) expected!");

            advanceTokenStream();

            //check <Instr>
            NodeInstr inst = instr();

            return new NodeInstrWhile(ex, inst);
        }


        //<Inst> ::= output (<Expr>);
        else if(_currentToken.getType() == TokenType.OUTPUT){
            advanceTokenStream();

            //check (
            if(_currentToken.getType() != TokenType.LEFTPAREN)
                die (" ( expected!");

            advanceTokenStream();

             //check <Expr>
            NodeExpr ex = expr();

            //check )
            if(_currentToken.getType() != TokenType.RIGHTPAREN)
                die(" ) expected!");

            advanceTokenStream();

            //check ;
            if(_currentToken.getType() != TokenType.SEMICOLON)
                die(" ';' expected!");

            advanceTokenStream();

            return new NodeInstrOutput(ex);
        }

        //<Inst> ::= input ( ID <IDSfx> ) ;
        else if(_currentToken.getType() == TokenType.INPUT){
            advanceTokenStream();

            //check (
            if(_currentToken.getType() != TokenType.LEFTPAREN)
                die (" ( expected!");
            advanceTokenStream();

            //check ID
            if(_currentToken.getType() != TokenType.ID)
                die (" ID expected!");

            String name = _currentToken.getName();
            NodeExpr idsfx = null;
            advanceTokenStream();


            //check <IDSfx>
            if(_currentToken.getType() == TokenType.LEFTBRACKET){
                advanceTokenStream();
                idsfx = expr();
                if(_currentToken.getType() != TokenType.RIGHTBRACKET)
                    die(" ']' expected!");
                advanceTokenStream();
            }

            //check )
            if(_currentToken.getType() != TokenType.RIGHTPAREN)
                die(" ) expected!");
            advanceTokenStream();

             //check ;
            if(_currentToken.getType() != TokenType.SEMICOLON)
                die(" ';' expected!");
            advanceTokenStream();


            if(idsfx == null)
                return new NodeInstrInputSingle(name);
            else
                return new NodeInstrInputArray(name, idsfx);
        }

        //<Inst> ::= { <ListInstr> }
        else if(_currentToken.getType() == TokenType.LEFTBRACE)
            return block();

        else
            die("Invalid instruction!");

        return null;
    }

	/**
	 * Parses the current token stream into a block node.
	 * Helper function to instr().
	 *
	 * @return NodeInstrBlock, parsed block node
	 * @throws comp.SyntaxException
	 * @throws comp.LexicalException
	 * @throws java.io.IOException
	 */
    private NodeInstrBlock block() throws SyntaxException, LexicalException,
            IOException {
        advanceTokenStream();
        NodeListInstr inBlock = listInstr();
        return new NodeInstrBlock(inBlock);
    }

	/**
	 * Parses the current token stream into a listInstructions.
	 * Deals with (ListInstr) of the grammar.
	 *
	 * @return	NodeListInstr, parsed node
	 * @throws comp.SyntaxException
	 * @throws comp.LexicalException
	 * @throws java.io.IOException
	 */
    private NodeListInstr listInstr() throws SyntaxException, LexicalException,
            IOException{

		//<ListInstr> ::=
        if(_currentToken.getType() == TokenType.RIGHTBRACE || _currentToken.getType() == TokenType.EOF){
            advanceTokenStream();
            return new NodeListInstrNil();
        }
		//<ListInstr> ::= <Instr> <ListInstr>
        else{
            return new NodeListInstrCons(instr(), listInstr());
        }
    }

	/**
	 * Parses the current token stream into an expression.
	 * Deals with (Expr) and (ExprSfx) of the grammar;
	 *
	 * @return NodeExpr, parsed expression node
	 * @throws comp.SyntaxException
	 * @throws comp.LexicalException
	 * @throws java.io.IOException
	 */
    private NodeExpr expr() throws SyntaxException, LexicalException,
            IOException{

        NodeExpr fact = factor();
        NodeExpr exprSfx = null;

        //<ExprSfx ::= + <Expr>
        if(_currentToken.getType() == TokenType.PLUS){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeExprPlus(fact, exprSfx);
        }

        //<ExprSfx ::= - <Expr>
        else if(_currentToken.getType() == TokenType.MINUS){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeExprMinus(fact, exprSfx);
        }

        //<ExprSfx ::= * <Expr>
        else if(_currentToken.getType() == TokenType.TIMES){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeExprTimes(fact, exprSfx);
        }

        //<ExprSfx ::= / <Expr>
        else if(_currentToken.getType() == TokenType.DIVIDE){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeExprDivide(fact, exprSfx);
        }

        //<ExprSfx ::= % <Expr>
        else if(_currentToken.getType() == TokenType.MODULUS){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeExprMod(fact, exprSfx);
        }

        //<ExprSfx ::= < <Expr>
        else if(_currentToken.getType() == TokenType.LESSTHAN){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeRelationLessThan(fact, exprSfx);
        }

        //<ExprSfx ::= <= <Expr>
        else if(_currentToken.getType() == TokenType.LESSEQ){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeRelationLessThanEqual(fact, exprSfx);
        }

        //<ExprSfx ::= > <Expr>
        else if(_currentToken.getType() == TokenType.GREATERTHAN){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeRelationGreaterThan(fact, exprSfx);
        }

        //<ExprSfx ::= >= <Expr>
        else if(_currentToken.getType() == TokenType.GREATEREQ){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeRelationGreaterThanEqual(fact, exprSfx);
        }

        //<ExprSfx ::= == <Expr>
        else if(_currentToken.getType() == TokenType.EQUAL){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeRelationEqual(fact, exprSfx);
        }

        //<ExprSfx ::= && <Expr>
        else if(_currentToken.getType() == TokenType.AND){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeRelationAnd(fact, exprSfx);
        }

        //<ExprSfx ::= || <Expr>
        else if(_currentToken.getType() == TokenType.OR){
            advanceTokenStream();
            exprSfx = expr();
            return new NodeRelationOr(fact, exprSfx);
        }

        //<ExprSfx ::=
        return fact;
    }

	/**
	 * Parses the current token stream into a factor.
	 * Deals with (Factor) of the grammar;
	 *
	 * @return NodeExpr
	 * @throws comp.SyntaxException
	 * @throws comp.LexicalException
	 * @throws java.io.IOException
	 */
    private NodeExpr factor() throws SyntaxException, LexicalException,
            IOException{

        //<Factor> ::= ID <IDSfx>
        if(_currentToken.getType() == TokenType.ID){
            String name = _currentToken.getName();
            advanceTokenStream();

            //<IDSfx>
            if(_currentToken.getType() == TokenType.LEFTBRACKET){
                advanceTokenStream();
                NodeExpr idsfx = expr();
                if(_currentToken.getType() != TokenType.RIGHTBRACKET)
                    die(" ']' expected!");
                advanceTokenStream();

                return new NodeExprArrayVar(name, idsfx);
            }
            
            else
                return new NodeExprVar(name);
        }

	//<Factor> ::= INTEGER
        else if(_currentToken.getType() == TokenType.INTEGER){
            int val = _currentToken.getValue();
            advanceTokenStream();
            return new NodeExprInt(val);
        }

	//<Factor> ::= ( <Expr> )
        else if(_currentToken.getType() == TokenType.LEFTPAREN){
            advanceTokenStream();
            NodeExpr ex = expr();

            //check )
            if(_currentToken.getType() != TokenType.RIGHTPAREN)
                die(" ')' expected!");
            advanceTokenStream();

            return ex;
        }

	//<Factor> ::= ! ( <Expr> )
        else if(_currentToken.getType() == TokenType.NOT){
            advanceTokenStream();

            //check (
            if(_currentToken.getType() != TokenType.LEFTPAREN)
                die(" '(' expected!");
            advanceTokenStream();

            NodeExpr ex = expr();

            //check )
            if(_currentToken.getType() != TokenType.RIGHTPAREN)
                die(" ')' expected!");
            advanceTokenStream();

            return new NodeRelationNot(ex);
        }

        else
            die("Unexpected expression!");

        return null;
    }




	/**
	 * Parses the current token stream into a declaration.
	 *
	 * @return	NodeInstrDecl, the declaration node.
	 * @throws comp.SyntaxException
	 * @throws comp.LexicalException
	 * @throws java.io.IOException
	 */
    public NodeInstrDecl decl() throws SyntaxException, LexicalException,
            IOException{



		//get name for the id
        advanceTokenStream();
        String name = _currentToken.getName();
        if(_nextToken.getType() == TokenType.INTEGER){
            advanceTokenStream();
            name.concat(Integer.toString(_currentToken.getValue()));
        }
        advanceTokenStream();


        //if an array is declared
        if(_currentToken.getType() == TokenType.LEFTBRACKET){
            advanceTokenStream();

            if(_currentToken.getType() != TokenType.INTEGER)
                die("expected Integer!");

            int val = _currentToken.getValue(); 
            advanceTokenStream();

            if(_currentToken.getType() != TokenType.RIGHTBRACKET)
                die("expected right bracket!");

            advanceTokenStream();

            if(_currentToken.getType() != TokenType.SEMICOLON)
                die(" ';' expected!");

            advanceTokenStream();

            return new NodeInstrDeclArray(name, val);
        }


        //if a single id is declared
        else{
            if(_currentToken.getType() != TokenType.SEMICOLON)
                die(" ';' expected!");

            advanceTokenStream();
            
            return new NodeInstrDeclSingle(name);
        }
    }


    /**
     * Set _currentToken to _nextToken and set _nextToken to the next token from
     * _tokenizer.
     */
    private void advanceTokenStream() throws LexicalException, IOException {
        _currentToken = _nextToken;

        // if we are not at the end of the file, read a token
        if (_currentToken == null || _currentToken.getType() != TokenType.EOF)
            _nextToken = _tokenizer.getToken();
    }

    /**
     * A convenience method to save writing the following pattern over and over
     * again: make sure the next token is of the required type and, if so,
     * advance. If the token is not of the required type, do not advance.
     * <p>
     * After eatToken is called, _currentToken is the token that was just eaten
     * (so it has the type specified in the call to eatToken).
     * <p>
     * Examples: <code>
     * if (eatToken(TokenType.ID))
     *   // do something with _currentToken.getName(), which eatToken(.) sets
     * else
     *   // error condition: "Identifier expected"
     * 
     * if (!eatToken(TokenType.LEFTPAREN))
     *   // error condition: "if-condition must begin with left parenthesis"
     * </code>
     * 
     * @param type
     *           Token will be eaten if it has this type.
     * @return True if current token has the specified type and stream was
     *         advanced.
     */
    private boolean eatToken(TokenType type) throws LexicalException,
            IOException {
        if (type == _nextToken.getType()) {
            advanceTokenStream();
            return true;
        } else {
            return false;
        }
    }

    /**
     * Throws a syntax exception with the specified message
     * 
     * @param message
     *           Message of the exception
     * @throws SyntaxException
     */
    private void die(String message) throws SyntaxException {
        throw new SyntaxException(_tokenizer.currentLine(), message);
    }

    /**
     * Creates a parser that reads from stdin (for testing). You must provide
     * two tokens before it will print the first one.
     */
    public static void main(String args[]) throws IOException, LexicalException {
        Parser p = new Parser();
        while (true) {
            if (p.eatToken(TokenType.INTEGER)) {
                System.out.println("Integer: " + p._currentToken.getValue());
            }
            else if (p.eatToken(TokenType.ID)) {
                System.out.println("Identifier:" + p._currentToken.getName());
            }
            else {
                p.advanceTokenStream();
                System.out.println(p._currentToken.getType());
            }
        }
    }

}
