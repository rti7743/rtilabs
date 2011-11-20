/*
 * ZPredicationTokenizer.cs
 * 
 * THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT!
 */

using System.IO;

using PerCederberg.Grammatica.Parser;

namespace Zanetti.Parser {

    /**
     * <remarks>A character stream tokenizer.</remarks>
     */
    internal class ZPredicationTokenizer : Tokenizer {

        /**
         * <summary>Creates a new tokenizer for the specified input
         * stream.</summary>
         * 
         * <param name='input'>the input stream to read</param>
         * 
         * <exception cref='ParserCreationException'>if the tokenizer
         * couldn't be initialized correctly</exception>
         */
        public ZPredicationTokenizer(TextReader input)
            : base(input) {

            CreatePatterns();
        }

        /**
         * <summary>Initializes the tokenizer by creating all the token
         * patterns.</summary>
         * 
         * <exception cref='ParserCreationException'>if the tokenizer
         * couldn't be initialized correctly</exception>
         */
        private void CreatePatterns() {
            TokenPattern  pattern;

            pattern = new TokenPattern((int) ZPredicationConstants.LEFT_PAREN,
                                       "LEFT_PAREN",
                                       TokenPattern.PatternType.STRING,
                                       "(");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.RIGHT_PAREN,
                                       "RIGHT_PAREN",
                                       TokenPattern.PatternType.STRING,
                                       ")");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.LEFT_PAREN_M,
                                       "LEFT_PAREN_M",
                                       TokenPattern.PatternType.STRING,
                                       "{");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.RIGHT_PAREN_M,
                                       "RIGHT_PAREN_M",
                                       TokenPattern.PatternType.STRING,
                                       "}");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.PREDICATION_OP,
                                       "PREDICATION_OP",
                                       TokenPattern.PatternType.STRING,
                                       ":=");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.COMMA,
                                       "COMMA",
                                       TokenPattern.PatternType.STRING,
                                       ",");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.LOGICAL_OP,
                                       "LOGICAL_OP",
                                       TokenPattern.PatternType.REGEXP,
                                       "&&|\\|\\||and|or");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.DEF_MARK,
                                       "DEF_MARK",
                                       TokenPattern.PatternType.STRING,
                                       "def");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.COMP_OP,
                                       "COMP_OP",
                                       TokenPattern.PatternType.REGEXP,
                                       ">|>=|<|<=|==|!=");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.ARITH1_OP,
                                       "ARITH1_OP",
                                       TokenPattern.PatternType.REGEXP,
                                       "\\+|\\-");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.ARITH2_OP,
                                       "ARITH2_OP",
                                       TokenPattern.PatternType.REGEXP,
                                       "\\*|\\/");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.NUMBER,
                                       "NUMBER",
                                       TokenPattern.PatternType.REGEXP,
                                       "\\-?[0-9]+(\\.[0-9]+)?");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.IDENTIFIER,
                                       "IDENTIFIER",
                                       TokenPattern.PatternType.REGEXP,
                                       "[a-zA-Z][a-zA-Z0-9_]*");
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.WHITESPACE,
                                       "WHITESPACE",
                                       TokenPattern.PatternType.REGEXP,
                                       "[ \\t\\n\\r]+");
            pattern.SetIgnore();
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.SINGLE_LINE_COMMENT,
                                       "SINGLE_LINE_COMMENT",
                                       TokenPattern.PatternType.REGEXP,
                                       "//.*");
            pattern.SetIgnore();
            AddPattern(pattern);

            pattern = new TokenPattern((int) ZPredicationConstants.MULTI_LINE_COMMENT,
                                       "MULTI_LINE_COMMENT",
                                       TokenPattern.PatternType.REGEXP,
                                       "/\\*([^*]|\\*[^/])*\\*/");
            pattern.SetIgnore();
            AddPattern(pattern);
        }
    }
}
