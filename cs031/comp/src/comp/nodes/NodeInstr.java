package comp.nodes;
 
 
/**
 * A typing superclass to make sure that a node that should be an
 * instruction is, in fact, an instruction.  Anything of type
 * NodeInstr will actually be of class, for example, NodeInstrWhile,
 * or another subclass of NodeInstr.  
 *   
 * @version   Tue Nov 18 23:24:04 1997
 * @author Mark Handy
 */                
public abstract class NodeInstr extends Node
{ }
 
