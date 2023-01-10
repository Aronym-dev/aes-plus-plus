#ifndef AESPP_MATH_HPP
#define AESPP_MATH_HPP

namespace AES_MATH
{
	unsigned char galoisMultiplication(unsigned char a, unsigned char b)
	{
		// Extracted from Wikipedia: https://en.m.wikipedia.org/wiki/Finite_field_arithmetic
		unsigned char c = 0; // Accumulator for the product of the multiplication
		while (a != 0 && b != 0)
		{
			if (b & 1)
			{
				// if the polynomial for b has a constant term, add the corresponding a to c
				c ^= a; // addition in GF(2^m) is an XOR of the polynomial coefficients
			}
			if (a & 0x80)
			{
				// GF modulo: if a has a nonzero term x^7, then must be reduced when it becomes x^8
				a = (a << 1) ^ 0x11b; // subtract (XOR) the primitive polynomial x^8 + x^4 + x^3 + x + 1 (0b1_0001_1011) – you can change it but it must be irreducible
			}
			else
			{
				a <<= 1; // equivalent to a*x
			}
			b >>= 1;
		}
	
		return c;
	}
} // namespace AES_MATH

#endif // AESPP_MATH_HPP