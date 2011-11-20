/*
 * ZPredicationParser.cs
 * 
 * THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT!
 */

using System.IO;

using PerCederberg.Grammatica.Parser;

namespace Zanetti.Parser {

    /**
     * <remarks>A token stream parser.</remarks>
     */
    internal class ZPredicationParser : RecursiveDescentParser {

        /**
         * <summary>An enumeration with the generated production node
         * identity constants.</summary>
         */
        private enum SynteticPatterns {
            SUBPRODUCTION_1 = 3001,
            SUBPRODUCTION_2 = 3002,
            SUBPRODUCTION_3 = 3003,
            SUBPRODUCTION_4 = 3004,
            SUBPRODUCTION_5 = 3005
        }

        /**
         * <summary>Creates a new parser.</summary>
         * 
         * <param name='input'>the input stream to read from</param>
         * 
         * <exception cref='ParserCreationException'>if the parser
         * couldn't be initialized correctly</exception>
         */
        public ZPredicationParser(TextReader input)
            : base(new ZPredicationTokenizer(input)) {

            CreatePatterns();
        }

        /**
         * <summary>Creates a new parser.</summary>
         * 
         * <param name='input'>the input stream to read from</param>
         * 
         * <param name='analyzer'>the analyzer to parse with</param>
         * 
         * <exception cref='ParserCreationException'>if the parser
         * couldn't be initialized correctly</exception>
         */
        public ZPredicationParser(TextReader input, Analyzer analyzer)
            : base(new ZPredicationTokenizer(input), analyzer) {

            CreatePatterns();
        }

        /**
         * <summary>Initializes the parser by creating all the production
         * patterns.</summary>
         * 
         * <exception cref='ParserCreationException'>if the parser
         * couldn't be initialized correctly</exception>
         */
        private void CreatePatterns() {
            ProductionPattern             pattern;
            ProductionPatternAlternative  alt;

            pattern = new ProductionPattern((int) ZPredicationConstants.ROOT,
                                            "root");
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.EXPR, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.EXPR,
                                            "expr");
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.L_EXPR, 1, 1);
            alt.AddProduction((int) SynteticPatterns.SUBPRODUCTION_1, 0, -1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.L_EXPR,
                                            "l_expr");
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.V_EXPR, 1, 1);
            alt.AddProduction((int) SynteticPatterns.SUBPRODUCTION_2, 0, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.V_EXPR,
                                            "v_expr");
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.T_EXPR, 1, 1);
            alt.AddProduction((int) SynteticPatterns.SUBPRODUCTION_3, 0, -1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.T_EXPR,
                                            "t_expr");
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.F_EXPR, 1, 1);
            alt.AddProduction((int) SynteticPatterns.SUBPRODUCTION_4, 0, -1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.F_EXPR,
                                            "f_expr");
            alt = new ProductionPatternAlternative();
            alt.AddToken((int) ZPredicationConstants.LEFT_PAREN, 1, 1);
            alt.AddProduction((int) ZPredicationConstants.EXPR, 1, 1);
            alt.AddToken((int) ZPredicationConstants.RIGHT_PAREN, 1, 1);
            pattern.AddAlternative(alt);
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.TERM, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.TERM,
                                            "term");
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.FUNCTION, 1, 1);
            pattern.AddAlternative(alt);
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.CONSTANT, 1, 1);
            pattern.AddAlternative(alt);
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.PARAMETER, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.FUNCTION,
                                            "function");
            alt = new ProductionPatternAlternative();
            alt.AddToken((int) ZPredicationConstants.IDENTIFIER, 1, 1);
            alt.AddToken((int) ZPredicationConstants.LEFT_PAREN, 1, 1);
            alt.AddProduction((int) ZPredicationConstants.L_EXPR_LIST, 0, 1);
            alt.AddToken((int) ZPredicationConstants.RIGHT_PAREN, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.L_EXPR_LIST,
                                            "l_expr_list");
            alt = new ProductionPatternAlternative();
            alt.AddProduction((int) ZPredicationConstants.L_EXPR, 1, 1);
            alt.AddProduction((int) SynteticPatterns.SUBPRODUCTION_5, 0, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.CONSTANT,
                                            "constant");
            alt = new ProductionPatternAlternative();
            alt.AddToken((int) ZPredicationConstants.NUMBER, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) ZPredicationConstants.PARAMETER,
                                            "parameter");
            alt = new ProductionPatternAlternative();
            alt.AddToken((int) ZPredicationConstants.LEFT_PAREN_M, 1, 1);
            alt.AddToken((int) ZPredicationConstants.NUMBER, 1, 1);
            alt.AddToken((int) ZPredicationConstants.RIGHT_PAREN_M, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) SynteticPatterns.SUBPRODUCTION_1,
                                            "Subproduction1");
            pattern.SetSyntetic(true);
            alt = new ProductionPatternAlternative();
            alt.AddToken((int) ZPredicationConstants.LOGICAL_OP, 1, 1);
            alt.AddProduction((int) ZPredicationConstants.L_EXPR, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) SynteticPatterns.SUBPRODUCTION_2,
                                            "Subproduction2");
            pattern.SetSyntetic(true);
            alt = new ProductionPatternAlternative();
            alt.AddToken((int) ZPredicationConstants.COMP_OP, 1, 1);
            alt.AddProduction((int) ZPredicationConstants.V_EXPR, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) SynteticPatterns.SUBPRODUCTION_3,
                                            "Subproduction3");
            pattern.SetSyntetic(true);
            alt = new ProductionPatternAlternative();
            alt.AddToken((int) ZPredicationConstants.ARITH1_OP, 1, 1);
            alt.AddProduction((int) ZPredicationConstants.T_EXPR, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) SynteticPatterns.SUBPRODUCTION_4,
                                            "Subproduction4");
            pattern.SetSyntetic(true);
            alt = new ProductionPatternAlternative();
            alt.AddToken((int) ZPredicationConstants.ARITH2_OP, 1, 1);
            alt.AddProduction((int) ZPredicationConstants.F_EXPR, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);

            pattern = new ProductionPattern((int) SynteticPatterns.SUBPRODUCTION_5,
                                            "Subproduction5");
            pattern.SetSyntetic(true);
            alt = new ProductionPatternAlternative();
            alt.AddToken((int) ZPredicationConstants.COMMA, 1, 1);
            alt.AddProduction((int) ZPredicationConstants.L_EXPR_LIST, 1, 1);
            pattern.AddAlternative(alt);
            AddPattern(pattern);
        }
    }
}
