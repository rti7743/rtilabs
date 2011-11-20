/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Diagnostics;
using System.Collections;

using PerCederberg.Grammatica.Parser;

using Zanetti.Arithmetic;

namespace Zanetti.Parser
{
	/// <summary>
	/// ExpressionConstructor ÇÃäTóvÇÃê‡ñæÇ≈Ç∑ÅB
	/// </summary>
	internal class ExpressionConstructor : ZPredicationAnalyzer
	{
		private Stack _stack;
		private Expression _result;
		private int _numOfParams;

		public ExpressionConstructor() {
			_stack = new Stack();
		}
		public Expression Result {
			get {
				return _result;
			}
		}
		public int NumOfParams {
			get {
				return _numOfParams;
			}
		}

		public override Node ExitRoot(Production node) {
			Debug.Assert(_stack.Count==1);
			_result = (Expression)_stack.Pop();
			return base.ExitRoot (node);
		}


		public override void ChildConstant(Production node, Node child) {
			_stack.Push(new ConstantExpression(Double.Parse(((Token)child).GetImage())));
			base.ChildConstant(node, child);
		}
		public override void ChildParameter(Production node, Node child) {
			if(child.GetId()==(int)ZPredicationConstants.NUMBER) {
				int t = Int32.Parse(((Token)child).GetImage());
				_numOfParams = Math.Max(t+1, _numOfParams);
				_stack.Push(new ParameterExpression(t));
			}
		}


		public override void EnterFunction(Production node) {
			_stack.Push(node);
			base.EnterFunction(node);
		}
		public override void ChildFunction(Production node, Node child) {
			if(child.GetId()==(int)ZPredicationConstants.IDENTIFIER)
				_stack.Push(child);
		}

		public override Node ExitFunction(Production node) {
			ArrayList ar = new ArrayList();
			object t = _stack.Pop();
			PrimitiveIndicator type = PrimitiveIndicator.Close;
			string img = null;
			bool inverse = false;
			while(node!=t) {
				if(t is Token) {
					if(((Token)t).GetId()==(int)ZPredicationConstants.IDENTIFIER) {
						img = ((Token)t).GetImage();
						type = ParsePrimitiveType(img);
					}
					/*
					else if(((Token)t).GetId()==(int)ZPredicationConstants.MINUS)
						inverse = true;
					*/
				}
				else {
					Debug.Assert(t is Expression);
					ar.Add(t);
				}
				t = _stack.Pop();
			}

			Expression newexpr = null;
			if(ar.Count>0) {
				ar.Reverse();
				Expression[] args = (Expression[])ar.ToArray(typeof(Expression));
				if(type==PrimitiveIndicator.LAST)
					newexpr = new FunctionExpression(img, args);
				else
					newexpr = new PrimitiveExpression(type, args);
			}
			else {
				if(type==PrimitiveIndicator.LAST)
					newexpr = new FunctionExpression(img, null);
				else
					newexpr = new PrimitiveExpression(type);
			}

			if(inverse) newexpr = new UnaryOpExpression(UnaryOpExpressionType.Inverse, newexpr);

			_stack.Push(newexpr);
			return base.ExitFunction (node);
		}

		public override void Enter(Node node) {
			ZPredicationConstants t = (ZPredicationConstants)node.GetId();
			if(t==ZPredicationConstants.ARITH1_OP || t==ZPredicationConstants.ARITH2_OP || t==ZPredicationConstants.COMP_OP || t==ZPredicationConstants.LOGICAL_OP)
				_stack.Push(((Token)node).GetImage());
			else if(t==ZPredicationConstants.EXPR || t==ZPredicationConstants.L_EXPR || t==ZPredicationConstants.V_EXPR || t==ZPredicationConstants.T_EXPR)
				_stack.Push(node);
			else
				base.Enter (node);
		}
		public override Node Exit(Node node) {
			ZPredicationConstants id = (ZPredicationConstants)node.GetId();
			if(id==ZPredicationConstants.EXPR || id==ZPredicationConstants.L_EXPR || id==ZPredicationConstants.V_EXPR || id==ZPredicationConstants.T_EXPR) {
				object t = _stack.Pop();
				Expression expr = null;
				BinaryOpExpressionType type = BinaryOpExpressionType.And;
				while(t != node) {
					if(t is Expression) {
						if(expr==null)
							expr = (Expression)t;
						else
							expr = new BinaryOpExpression(type, (Expression)t, expr);
					}
					else if(t is String) {
						type = ParseBinaryOpType((string)t);
					}
					t = _stack.Pop();
				}

				_stack.Push(expr);
				return node;
			}
			else
				return base.Exit (node);
		}

		private static BinaryOpExpressionType ParseBinaryOpType(string value) {
			if(value=="&&" || value=="and")
				return BinaryOpExpressionType.And;
			else if(value=="||" || value=="or")
				return BinaryOpExpressionType.Or;
			else if(value==":=")
				return BinaryOpExpressionType.Predication;

			else if(value=="+")
				return BinaryOpExpressionType.Plus;
			else if(value=="-")
				return BinaryOpExpressionType.Minus;
			else if(value=="*")
				return BinaryOpExpressionType.Multiply;
			else if(value=="/")
				return BinaryOpExpressionType.Divide;
		
			else if(value=="==")
				return BinaryOpExpressionType.Eq;
			else if(value=="!=")
				return BinaryOpExpressionType.NotEq;
			else if(value=="<")
				return BinaryOpExpressionType.Smaller;
			else if(value==">")
				return BinaryOpExpressionType.Greater;
			else if(value=="<=")
				return BinaryOpExpressionType.SmallerEq;
			else if(value==">=")
				return BinaryOpExpressionType.GreaterEq;
			else
				throw new ArgumentException(value + " is invalid operator");
		 }

		private static PrimitiveIndicator ParsePrimitiveType(string value) {
			value = value.ToLower();
			if(value=="open")
				return PrimitiveIndicator.Open;
			else if(value=="close")
				return PrimitiveIndicator.Close;
			else if(value=="high")
				return PrimitiveIndicator.High;
			else if(value=="low")
				return PrimitiveIndicator.Low;
			else if(value=="volume")
				return PrimitiveIndicator.Volume;
			else if(value=="cl")
				return PrimitiveIndicator.CreditLong;
			else if(value=="cs")
				return PrimitiveIndicator.CreditShort;
			else
				return PrimitiveIndicator.LAST;
		}

	}
}
