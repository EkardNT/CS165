#ifndef BIGINTEGERUTILS_H
#define BIGINTEGERUTILS_H

/*
Code modified from the Matt McCutchen's BigInt library,
which is in the public domain.
See: https://mattmccutchen.net/bigint/
*/

#include "BigInteger.h"
#include <string>
#include <iostream>

/* This file provides:
 * - Convenient std::string <-> BigUnsigned/BigInteger conversion routines
 * - std::ostream << operators for BigUnsigned/BigInteger */

// std::string conversion routines.  Base 10 only.
std::string bigUnsignedToString(const BigUnsigned &x);
std::string bigIntegerToString(const BigInteger &x);
BigUnsigned stringToBigUnsigned(const std::string &s);
BigInteger stringToBigInteger(const std::string &s);

// Outputs x to os, obeying the flags `dec', `hex', `bin', and `showbase'.
std::ostream &operator <<(std::ostream &os, const BigUnsigned &x);

// Outputs x to os, obeying the flags `dec', `hex', `bin', and `showbase'.
// My somewhat arbitrary policy: a negative sign comes before a base indicator (like -0xFF).
std::ostream &operator <<(std::ostream &os, const BigInteger &x);

#endif
