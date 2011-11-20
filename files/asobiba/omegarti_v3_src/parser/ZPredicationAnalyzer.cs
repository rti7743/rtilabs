/*
 * ZPredicationAnalyzer.cs
 * 
 * THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT!
 */

using PerCederberg.Grammatica.Parser;

namespace Zanetti.Parser {

    /**
     * <remarks>A class providing callback methods for the
     * parser.</remarks>
     */
    internal abstract class ZPredicationAnalyzer : Analyzer {

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public override void Enter(Node node) {
            switch (node.GetId()) {
            case (int) ZPredicationConstants.LEFT_PAREN:
                EnterLeftParen((Token) node);
                break;
            case (int) ZPredicationConstants.RIGHT_PAREN:
                EnterRightParen((Token) node);
                break;
            case (int) ZPredicationConstants.LEFT_PAREN_M:
                EnterLeftParenM((Token) node);
                break;
            case (int) ZPredicationConstants.RIGHT_PAREN_M:
                EnterRightParenM((Token) node);
                break;
            case (int) ZPredicationConstants.PREDICATION_OP:
                EnterPredicationOp((Token) node);
                break;
            case (int) ZPredicationConstants.COMMA:
                EnterComma((Token) node);
                break;
            case (int) ZPredicationConstants.LOGICAL_OP:
                EnterLogicalOp((Token) node);
                break;
            case (int) ZPredicationConstants.DEF_MARK:
                EnterDefMark((Token) node);
                break;
            case (int) ZPredicationConstants.COMP_OP:
                EnterCompOp((Token) node);
                break;
            case (int) ZPredicationConstants.ARITH1_OP:
                EnterArith1Op((Token) node);
                break;
            case (int) ZPredicationConstants.ARITH2_OP:
                EnterArith2Op((Token) node);
                break;
            case (int) ZPredicationConstants.NUMBER:
                EnterNumber((Token) node);
                break;
            case (int) ZPredicationConstants.IDENTIFIER:
                EnterIdentifier((Token) node);
                break;
            case (int) ZPredicationConstants.ROOT:
                EnterRoot((Production) node);
                break;
            case (int) ZPredicationConstants.EXPR:
                EnterExpr((Production) node);
                break;
            case (int) ZPredicationConstants.L_EXPR:
                EnterLExpr((Production) node);
                break;
            case (int) ZPredicationConstants.V_EXPR:
                EnterVExpr((Production) node);
                break;
            case (int) ZPredicationConstants.T_EXPR:
                EnterTExpr((Production) node);
                break;
            case (int) ZPredicationConstants.F_EXPR:
                EnterFExpr((Production) node);
                break;
            case (int) ZPredicationConstants.TERM:
                EnterTerm((Production) node);
                break;
            case (int) ZPredicationConstants.FUNCTION:
                EnterFunction((Production) node);
                break;
            case (int) ZPredicationConstants.L_EXPR_LIST:
                EnterLExprList((Production) node);
                break;
            case (int) ZPredicationConstants.CONSTANT:
                EnterConstant((Production) node);
                break;
            case (int) ZPredicationConstants.PARAMETER:
                EnterParameter((Production) node);
                break;
            }
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public override Node Exit(Node node) {
            switch (node.GetId()) {
            case (int) ZPredicationConstants.LEFT_PAREN:
                return ExitLeftParen((Token) node);
            case (int) ZPredicationConstants.RIGHT_PAREN:
                return ExitRightParen((Token) node);
            case (int) ZPredicationConstants.LEFT_PAREN_M:
                return ExitLeftParenM((Token) node);
            case (int) ZPredicationConstants.RIGHT_PAREN_M:
                return ExitRightParenM((Token) node);
            case (int) ZPredicationConstants.PREDICATION_OP:
                return ExitPredicationOp((Token) node);
            case (int) ZPredicationConstants.COMMA:
                return ExitComma((Token) node);
            case (int) ZPredicationConstants.LOGICAL_OP:
                return ExitLogicalOp((Token) node);
            case (int) ZPredicationConstants.DEF_MARK:
                return ExitDefMark((Token) node);
            case (int) ZPredicationConstants.COMP_OP:
                return ExitCompOp((Token) node);
            case (int) ZPredicationConstants.ARITH1_OP:
                return ExitArith1Op((Token) node);
            case (int) ZPredicationConstants.ARITH2_OP:
                return ExitArith2Op((Token) node);
            case (int) ZPredicationConstants.NUMBER:
                return ExitNumber((Token) node);
            case (int) ZPredicationConstants.IDENTIFIER:
                return ExitIdentifier((Token) node);
            case (int) ZPredicationConstants.ROOT:
                return ExitRoot((Production) node);
            case (int) ZPredicationConstants.EXPR:
                return ExitExpr((Production) node);
            case (int) ZPredicationConstants.L_EXPR:
                return ExitLExpr((Production) node);
            case (int) ZPredicationConstants.V_EXPR:
                return ExitVExpr((Production) node);
            case (int) ZPredicationConstants.T_EXPR:
                return ExitTExpr((Production) node);
            case (int) ZPredicationConstants.F_EXPR:
                return ExitFExpr((Production) node);
            case (int) ZPredicationConstants.TERM:
                return ExitTerm((Production) node);
            case (int) ZPredicationConstants.FUNCTION:
                return ExitFunction((Production) node);
            case (int) ZPredicationConstants.L_EXPR_LIST:
                return ExitLExprList((Production) node);
            case (int) ZPredicationConstants.CONSTANT:
                return ExitConstant((Production) node);
            case (int) ZPredicationConstants.PARAMETER:
                return ExitParameter((Production) node);
            }
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public override void Child(Production node, Node child) {
            switch (node.GetId()) {
            case (int) ZPredicationConstants.ROOT:
                ChildRoot(node, child);
                break;
            case (int) ZPredicationConstants.EXPR:
                ChildExpr(node, child);
                break;
            case (int) ZPredicationConstants.L_EXPR:
                ChildLExpr(node, child);
                break;
            case (int) ZPredicationConstants.V_EXPR:
                ChildVExpr(node, child);
                break;
            case (int) ZPredicationConstants.T_EXPR:
                ChildTExpr(node, child);
                break;
            case (int) ZPredicationConstants.F_EXPR:
                ChildFExpr(node, child);
                break;
            case (int) ZPredicationConstants.TERM:
                ChildTerm(node, child);
                break;
            case (int) ZPredicationConstants.FUNCTION:
                ChildFunction(node, child);
                break;
            case (int) ZPredicationConstants.L_EXPR_LIST:
                ChildLExprList(node, child);
                break;
            case (int) ZPredicationConstants.CONSTANT:
                ChildConstant(node, child);
                break;
            case (int) ZPredicationConstants.PARAMETER:
                ChildParameter(node, child);
                break;
            }
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterLeftParen(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitLeftParen(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterRightParen(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitRightParen(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterLeftParenM(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitLeftParenM(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterRightParenM(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitRightParenM(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterPredicationOp(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitPredicationOp(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterComma(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitComma(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterLogicalOp(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitLogicalOp(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterDefMark(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitDefMark(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterCompOp(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitCompOp(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterArith1Op(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitArith1Op(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterArith2Op(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitArith2Op(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterNumber(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitNumber(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterIdentifier(Token node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitIdentifier(Token node) {
            return node;
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterRoot(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitRoot(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildRoot(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterExpr(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitExpr(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildExpr(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterLExpr(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitLExpr(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildLExpr(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterVExpr(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitVExpr(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildVExpr(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterTExpr(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitTExpr(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildTExpr(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterFExpr(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitFExpr(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildFExpr(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterTerm(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitTerm(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildTerm(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterFunction(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitFunction(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildFunction(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterLExprList(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitLExprList(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildLExprList(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterConstant(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitConstant(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildConstant(Production node, Node child) {
            node.AddChild(child);
        }

        /**
         * <summary>Called when entering a parse tree node.</summary>
         * 
         * <param name='node'>the node being entered</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void EnterParameter(Production node) {
        }

        /**
         * <summary>Called when exiting a parse tree node.</summary>
         * 
         * <param name='node'>the node being exited</param>
         * 
         * <returns>the node to add to the parse tree, or
         *          null if no parse tree should be created</returns>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual Node ExitParameter(Production node) {
            return node;
        }

        /**
         * <summary>Called when adding a child to a parse tree
         * node.</summary>
         * 
         * <param name='node'>the parent node</param>
         * <param name='child'>the child node, or null</param>
         * 
         * <exception cref='ParseException'>if the node analysis
         * discovered errors</exception>
         */
        public virtual void ChildParameter(Production node, Node child) {
            node.AddChild(child);
        }
    }
}
