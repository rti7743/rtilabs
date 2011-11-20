/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;

namespace Zanetti.Arithmetic
{
	//Tree

	internal abstract class Expression {
		public abstract object Apply(ExpressionVisitor visitor);
	}

	internal class ConstantExpression : Expression {
		private double _value;
		public double Value {
			get {
				return _value;
			}
		}
		public ConstantExpression(double value) {
			_value = value;
		}

		public override object Apply(ExpressionVisitor visitor) {
			return visitor.Constant(this);
		}
	}
	
	internal class ParameterExpression : Expression {
		private int _index;
		public int Index {
			get {
				return _index;
			}
		}

		public ParameterExpression(int index) {
			_index = index;
		}

		public override object Apply(ExpressionVisitor visitor) {
			return visitor.Parameter(this);
		}
	}


	internal class PrimitiveExpression : Expression {
		private PrimitiveIndicator _type;
		private Expression[] _args;

		public PrimitiveExpression(PrimitiveIndicator t) {
			_type = t;
		}
		public PrimitiveExpression(PrimitiveIndicator t, Expression[] args) {
			_type = t;
			_args = args;
		}
		public PrimitiveIndicator Type {
			get {
				return _type;
			}
		}
		public Expression[] Args {
			get {
				return _args;
			}
		}
		public override object Apply(ExpressionVisitor visitor) {
			return visitor.Primitive(this);
		}
	}

	internal enum BinaryOpExpressionType {
		And,
		Or,
		Predication,

		Plus,
		Minus,
		Multiply,
		Divide,
		
		Eq,
		NotEq,
		Smaller,
		Greater,
		SmallerEq,
		GreaterEq
	}

	internal class BinaryOpExpression : Expression {
		private BinaryOpExpressionType _type;
		private Expression _left;
		private Expression _right;

		public BinaryOpExpression(BinaryOpExpressionType t, Expression left, Expression right) {
			_type = t;
			_left = left;
			_right = right;
		}
		public BinaryOpExpressionType Type {
			get {
				return _type;
			}
		}
		public Expression Left {
			get{
				return _left;
			}
		}
		public Expression Right {
			get {
				return _right;
			}
		}
		public override object Apply(ExpressionVisitor visitor) {
			return visitor.BinaryOp(this);
		}
	}

	internal enum UnaryOpExpressionType {
		Inverse
	}

	internal class UnaryOpExpression : Expression {

		private UnaryOpExpressionType _type;
		private Expression _expr;

		public UnaryOpExpression(UnaryOpExpressionType t, Expression expr) {
			_type = t;
			_expr = expr;
		}
		public UnaryOpExpressionType Type {
			get {
				return _type;
			}
		}
		public Expression Content {
			get{
				return _expr;
			}
		}
		public override object Apply(ExpressionVisitor visitor) {
			return visitor.UnaryOp(this);
		}
	}

	internal class FunctionExpression : Expression {
		private string _name;
		private Expression[] _args;

		public enum FECategory {
			Unknown,
			Library,
			Indicator
		}
		
		private FECategory _FECategory;
		private int _laneID;
		public FECategory Category {
			get {
				return _FECategory;
			}
			set {
				_FECategory = value;
			}
		}
		public int LaneID {
			get {
				return _laneID;
			}
			set {
				_laneID = value;
			}
		}


		public FunctionExpression(string name, Expression[] args) {
			_name = name;
			_args = args;
			_FECategory = FECategory.Unknown;
		}
		public string Name {
			get {
				return _name;
			}
		}
		public Expression[] Args {
			get {
				return _args;
			}
		}
		public override object Apply(ExpressionVisitor visitor) {
			return visitor.Function(this);
		}
	}



}
