package comp;

import java.io.IOException;

import comp.nodes.NodeProgram;

/**
 * Main-method class for Compiler. Handles the five main steps needed to
 * compile:
 *
 * <ol>
 * <li>Parse the input to a syntax tree</li>
 * <li>Check the semantics</li>
 * <li>Print out the syntax tree (for debuggging)</li>
 * <li>Generate the code</li>
 * <li>Output the code to the file</li>
 * </ol>
 * 
 * @version Fall 2007
 * @author The TAs
 */
public class Compiler {

    /**
     * No instantiation is allowed, so disable the default constructor.
     */
    private Compiler() throws NoSuchMethodException {
        throw new NoSuchMethodException("Instantiation of Compiler not allowed.");
    }
    
    /**
     * Creates a png file to serve as a visual representation of the syntax tree.
     * @param program The syntax tree be viewed.
     * @throws VisitorException
     * @throws IOException
     */
    private static void viewParseTree(NodeProgram program) throws VisitorException, IOException{
    	GraphvizVisitor greVis = new GraphvizVisitor("parse_tree.gv.txt");
        
        if (program != null)
        	program.visit(greVis);
        
        greVis.finish();
        
        Runtime.getRuntime().exec("dot -Tpng -o graph.png parse_tree.gv.txt");
    }

    /**
     * Executes the compiler on a Blaise file, saving to a MAL file.
     * 
     * @param args Blaise file (input), MAL file (output)
     * @throws IOException
     *             If something goes wrong reading Blaise file or writing MAL
     *             file
     * @throws LexicalException
     *             If input contains lexical errors
     * @throws SyntaxException
     *             If input fails syntax checking
     * @throws VisitorException
     *             If something goes wrong while visiting the syntax tree
     */
    public static void main(String args[]) throws LexicalException,
            IOException, SyntaxException, VisitorException {

        if ((args.length < 2) || (args[0] == null) || (args[1] == null)) {
            System.err.println("\n"
                    + "Usage: java comp.Compiler <blaise file> <mal file>\n");
            System.exit(1);
        }

        // pass these files to the Tokenizer and MIPSCodeGenerator
        String blaiseFilename = args[0];
        String malFilename = args[1];

        // set up your compiler here.

        // 1. parse the input
        Parser parser = new Parser(blaiseFilename);
        NodeProgram program = parser.parse();

        // 2. check semantics
        SemanticVisitor semVisitor = new SemanticVisitor();
        if (program != null)
            program.visit(semVisitor);

        // 3. print out the syntax tree
        if (program != null)
            program.visit(new PrintingVisitor());
        
        // 4. Create a png file showing the syntax tree
        viewParseTree(program);
        
        // 5. generate the code
        MIPSCodeGenerator codegen = new MIPSCodeGenerator();

        if (program != null)
            program.visit(new CodeGenVisitor(codegen, semVisitor.getDeclList()));

        // 6. output the code to the file
        codegen.writeToFile(malFilename);

        System.out.println("\nCompilation was successful.\n");

    }

}
