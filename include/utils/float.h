/* Copyright (C) 1992-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _IEEE754_H
#define _IEEE754_H 1

#ifdef  _WIN64
#define WINDOWS
#include <Windows.h>
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
#define IS_LITTLE_ENDIAN
#else
#define IS_BIG_ENDIAN
#endif
#endif 

#ifdef unix
#include <features.h>
#include <endian.h>
#if	__BYTE_ORDER == __LITTLE_ENDIAN
#define IS_LITTLE_ENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#define IS_BIG_ENDIAN
#endif 
#endif

#include <ostream>

union ieee754_float
{
    float f;

    /* This is the IEEE 754 single-precision format.  */
    struct
    {
#ifdef IS_LITTLE_ENDIAN
        unsigned int mantissa:23;           // The remaining 23 bits are used to represent the mantissa
        unsigned int exponent:8;            // The subsequent 8 bits are assigned to the exponent
        unsigned int sign:1;                // The first bit, is assigned to the sign
#endif				/* Little endian.  */
#ifdef IS_BIG_ENDIAN
        // Big endians represent the numbers similarly, but in the reverse direction.

        unsigned int sign:1;
        unsigned int exponent:8;
        unsigned int mantissa:23;
#endif				/* Big endian.  */

        // Implementation specific features: computation errors results (e.g., divide by zero).
        bool NaN() const {
            return exponent == 255 && mantissa != 0;
        }
        // Maximum exponent to zero mantissa is used to represent the infinity
        bool isInfinity() const {
            return exponent == 255 && mantissa == 0;
        }
        // Zero mantissa and zero exponent represent the actual zero value.
        bool isZero() const {
            return mantissa == 0 && exponent == 0;
        }
    } parts;

};

/**
 * Putting the float inside a data structure which, by using a union with a float and bit for a struct, is able to
 * get which bits are which
 */
#ifdef WINDOWS
#define declare_printable_float(name, val)   ieee754_float name; name.f = (val)
#else
#define declare_printable_float(name, val)   ieee754_float name = { .f = (val) }
#endif 
/**
 * Operator printing the main representations when the exponent number is not zero (so, considering only the normalized cases)
 * // if (0 < classe.parts.exponent && classe.parts.exponent < 255)
 *
 * @param os        Where to print the informations as output
 * @param classe    Bitwise representation:
 * @return
 */
std::ostream &operator<<(std::ostream &os, ieee754_float &classe);

#define IEEE754_FLOAT_BIAS	0x7f /* Added to exponent.  */
#endif