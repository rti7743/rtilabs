/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;

namespace Zanetti.Arithmetic
{
	internal abstract class ExpressionVisitor {
		public abstract object Constant(ConstantExpression expr);
		public abstract object Parameter(ParameterExpression expr);
		public abstract object Primitive(PrimitiveExpression expr);
		public abstract object UnaryOp(UnaryOpExpression expr); 
		public abstract object BinaryOp(BinaryOpExpression expr); 
		public abstract object Function(FunctionExpression expr);
	}

	internal abstract class BasicVisitor : ExpressionVisitor {
		protected string _functionName;

		public string FunctionName {
			get {
				return _functionName;
			}
		}

		protected BasicVisitor(string fn) {
			_functionName = fn;
		}

		protected void ThrowArithmeticException(string detail) {
			String msg = String.Format("{0} ÇÃï]âøíÜÇ…ÉGÉâÅ[Ç™î≠ê∂ÇµÇ‹ÇµÇΩÅB\n", _functionName);
			if(detail!=null) msg += detail;
			throw new ZArithmeticException(msg);
		}
	}

	internal class ZArithmeticException : Exception {
		public ZArithmeticException(string msg) : base(msg) {
		}
	}
	internal class TypeMismatchException : Exception {
		public TypeMismatchException(string msg) : base(msg) {}
	}

}
