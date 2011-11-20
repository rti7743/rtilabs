/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using Travis.PKI;

namespace Zanetti
{
	/// <summary>
	/// PublicKey ÇÃäTóvÇÃê‡ñæÇ≈Ç∑ÅB
	/// </summary>
	internal class ZPublicKey
	{
		public static RSAPublicKey PubKeyForExtensionKit {
			get {
				return new RSAPublicKey(new BigInteger(EXPONENT, 16), new BigInteger(MODULUS, 16));
			}
		}

		private const string MODULUS = "C72F06709DEE9F0E0A6281C4DF91A423C6BFF781BAD00980DF19FB9731F85783";
		private const string EXPONENT = "21";

	}
}
