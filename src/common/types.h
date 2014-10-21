/* xoreos-tools - Tools to help with xoreos development
 *
 * xoreos-tools is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos-tools is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos-tools. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file common/types.h
 *  Low-level type definitions to handle fixed width types portably.
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef HAVE_STDINT_H
	#include <stdint.h>
#endif // HAVE_STDINT_H
#ifdef HAVE_INTTYPES_H
	#include <inttypes.h>
#endif // HAVE_INTTYPES_H
#ifdef HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H

#if defined (HAVE_STDINT_H)
	typedef int8_t int8;
	typedef uint8_t uint8;
	typedef int16_t int16;
	typedef uint16_t uint16;
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int64_t int64;
	typedef uint64_t uint64;
#elif defined (HAVE_INTTYPES_H)
	typedef int8_t int8;
	typedef uint8_t uint8;
	typedef int_16_t int16;
	typedef uint16_t uint16;
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int64_t int64;
	typedef uint64_t uint64;
#elif defined (HAVE_SYS_TYPES_H)
	typedef int8_t int8;
	typedef u_int8_t uint8;
	typedef int16_t int16;
	typedef u_int16_t uint16;
	typedef int32_t int32;
	typedef u_int32_t uint32;
	typedef int64_t int64;
	typedef u_int64_t uint64;
#else
	// TODO: Add a fall-back type detection to the configure script
	#error No way to derive fixed-width variable types
#endif

#ifndef PRId64
	#define PRId64   "lld"
	#define Cd64(x)  ((signed long long) (x))
#else
	#define Cd64(x)  ((int64) (x))
#endif

#ifndef PRIu64
	#define PRIu64   "llu"
	#define Cu64(x)  ((unsigned long long) (x))
#else
	#define Cu64(x)  ((uint64) (x))
#endif

typedef uint8 byte;
typedef unsigned int uint;

#endif // COMMON_TYPES_H
