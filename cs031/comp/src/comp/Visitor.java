package comp;

import comp.nodes.*;

/**
 * Base interface for classes implementing the visitor pattern. A Visitor
 * implementation must be able to handle every non-abstract Node class.
 */
public interface Visitor {

    /**
     * Handles visiting a NodeInstrDeclArray
     */
    public void handleNodeInstrDeclArray(NodeInstrDeclArray node)
            throws VisitorException;

    /**
     * Handles visiting a NodeInstrDeclSingle
     */
    public void handleNodeInstrDeclSingle(NodeInstrDeclSingle node)
            throws VisitorException;

    /**
     * Handles visiting a NodeExprArrayVar
     */
    public void handleNodeExprArrayVar(NodeExprArrayVar node)
            throws VisitorException;

    /**
     * Handles visiting a NodeExprInt
     */
    public void handleNodeExprInt(NodeExprInt node) 
            throws VisitorException;

    /**
     * Handles visiting a NodeExprMinus
     */
    public void handleNodeExprMinus(NodeExprMinus node) 
            throws VisitorException;

    /**
     * Handles visiting a NodeExprPlus
     */
    public void handleNodeExprPlus(NodeExprPlus node) 
            throws VisitorException;

    /**
     * Handles visiting a NodeExprTimes
     */
    public void handleNodeExprTimes(NodeExprTimes node) 
            throws VisitorException;
    
    /**
     * Handles visiting a NodeExprDivide
     */
    public void handleNodeExprDivide(NodeExprDivide node) 
            throws VisitorException;
    
    /**
     * Handles visiting a NodeExprMod
     */
    public void handleNodeExprMod(NodeExprMod node) 
            throws VisitorException;

    /**
     * Handles visiting a NodeExprVar
     */
    public void handleNodeExprVar(NodeExprVar node) 
            throws VisitorException;

    /**
     * Handles visiting a NodeInstrAssignArray
     */
    public void handleNodeInstrAssignArray(NodeInstrAssignArray node)
            throws VisitorException;

    /**
     * Handles visiting a NodeInstrAssignSingle
     */
    public void handleNodeInstrAssignSingle(NodeInstrAssignSingle node)
            throws VisitorException;

    /**
     * Handles visiting a NodeInstrBlock
     */
    public void handleNodeInstrBlock(NodeInstrBlock node)
            throws VisitorException;

    /**
     * Handles visiting a NodeInstrIfthen
     */
    public void handleNodeInstrIfthen(NodeInstrIfthen node)
            throws VisitorException;

    /**
     * Handles visiting a NodeInstrIfthenelse
     */
    public void handleNodeInstrIfthenelse(NodeInstrIfthenelse node)
            throws VisitorException;

    /**
     * Handles visiting a NodeInstrOutput
     */
    public void handleNodeInstrOutput(NodeInstrOutput node)
            throws VisitorException;
    
    /**
     * Handles visiting a NodeInstrInputArray
     */
    public void handleNodeInstrInputArray(NodeInstrInputArray node)
            throws VisitorException;
    
    /**
     * Handles visiting a NodeInstrInputSingle
     */
    public void handleNodeInstrInputSingle(NodeInstrInputSingle node)
            throws VisitorException;

    /**
     * Handles visiting a NodeInstrWhile
     */
    public void handleNodeInstrWhile(NodeInstrWhile node)
            throws VisitorException;

    /**
     * Handles visiting a NodeListInstrCons
     */
    public void handleNodeListInstrCons(NodeListInstrCons node)
            throws VisitorException;

    /**
     * Handles visiting a NodeListInstrNil
     */
    public void handleNodeListInstrNil(NodeListInstrNil node)
            throws VisitorException;

    /**
     * Handles visiting a NodeProgram
     */
    public void handleNodeProgram(NodeProgram node) 
            throws VisitorException;

    /**
     * Handles visiting a NodeRelationLessThan
     */
    public void handleNodeRelationLessThan(NodeRelationLessThan node)
            throws VisitorException;
    
    /**
     * Handles visiting a NodeRelationLessThanEqual
     */
    public void handleNodeRelationLessThanEqual(NodeRelationLessThanEqual node)
            throws VisitorException;

    /**
     * Handles visiting a NodeRelationEqual
     */
    public void handleNodeRelationEqual(NodeRelationEqual node)
            throws VisitorException;
    
    /**
     * Handles visiting a NodeRelationGreaterThanEqual
     */
    public void handleNodeRelationGreaterThanEqual(NodeRelationGreaterThanEqual node)
            throws VisitorException;
    
    /**
     * Handles visiting a NodeRelationGreaterThan
     */
    public void handleNodeRelationGreaterThan(NodeRelationGreaterThan node)
            throws VisitorException;
    
    /**
     * Handles visiting a NodeRelationAnd
     */
    public void handleNodeRelationAnd(NodeRelationAnd node)
            throws VisitorException;

    /**
     * Handles visiting a NodeRelationOr
     */
    public void handleNodeRelationOr(NodeRelationOr node)
            throws VisitorException;
    
    /**
     * Handles visiting a NodeRelationNot
     */
    public void handleNodeRelationNot(NodeRelationNot node)
            throws VisitorException;
}
