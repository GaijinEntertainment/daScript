/*
 * uriparser - RFC 3986 URI parsing library
 *
 * Copyright (C) 2007, Weijia Song <songweijia@gmail.com>
 * Copyright (C) 2007, Sebastian Pipping <webmaster@hartwork.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 *     * Neither the name of the <ORGANIZATION> nor the names of its
 *       contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file UriIndependent.h
 * Holds definitions independent of the encoding pass.
 */

#ifndef URI_INDEPENDENT_H
#define URI_INDEPENDENT_H 1



/* Version helper macro */
#define URI_ANSI_TO_UNICODE(x) L##x



/* Version */
#define URI_VER_MAJOR           0
#define URI_VER_MINOR           3
#define URI_VER_RELEASE         0
#define URI_VER_SUFFIX_ANSI     "rc1"
#define URI_VER_SUFFIX_UNICODE  URI_ANSI_TO_UNICODE(URI_VER_SUFFIX_ANSI)



/* More version helper macros */
#define URI_INT_TO_ANSI_HELPER(x) #x
#define URI_INT_TO_ANSI(x) URI_INT_TO_ANSI_HELPER(x)

#define URI_INT_TO_UNICODE_HELPER(x) URI_ANSI_TO_UNICODE(#x)
#define URI_INT_TO_UNICODE(x) URI_INT_TO_UNICODE_HELPER(x)

#define URI_VER_ANSI_HELPER(ma, mi, r, s) \
	URI_INT_TO_ANSI(ma) "." \
	URI_INT_TO_ANSI(mi) "." \
	URI_INT_TO_ANSI(r) \
	s

#define URI_VER_UNICODE_HELPER(ma, mi, r, s) \
	URI_INT_TO_UNICODE(ma) L"." \
	URI_INT_TO_UNICODE(mi) L"." \
	URI_INT_TO_UNICODE(r) \
	s



/* Full version strings */
#define URI_VER_ANSI     URI_VER_ANSI_HELPER(URI_VER_MAJOR, URI_VER_MINOR, URI_VER_RELEASE, URI_VER_SUFFIX_ANSI)
#define URI_VER_UNICODE  URI_VER_UNICODE_HELPER(URI_VER_MAJOR, URI_VER_MINOR, URI_VER_RELEASE, URI_VER_SUFFIX_UNICODE)



/* Unused parameter macro */
#ifdef __GNUC__
# define URI_UNUSED(x) unused_##x __attribute__((unused))
#else
# define URI_UNUSED(x) x
#endif



/* Function inlining, not ANSI/ISO C! */
#if defined(__INTEL_COMPILER)
/* Intel C/C++ */
/* http://predef.sourceforge.net/precomp.html#sec20 */
/* http://www.intel.com/support/performancetools/c/windows/sb/CS-007751.htm#2 */
# define URI_INLINE __force_inline
#elif defined(_MSC_VER)
/* Microsoft Visual C++ */
/* http://predef.sourceforge.net/precomp.html#sec32 */
/* http://msdn2.microsoft.com/en-us/library/ms882281.aspx */
# define URI_INLINE __forceinline
#elif (__GNUC__ >= 4)
/* GCC C/C++ 4.x.x */
/* http://predef.sourceforge.net/precomp.html#sec13 */
# define URI_INLINE __attribute__((always_inline))
#elif (__STDC_VERSION__ >= 199901L)
/* C99, "inline" is a keyword */
# define URI_INLINE inline
#else
/* No inlining */
# define URI_INLINE
#endif



#define UriBool      int

#define URI_TRUE     1
#define URI_FALSE    0

#define URI_SUCCESS  0
#define URI_ERROR    1



#ifndef URI_DOXYGEN
/* For NULL */
# include <stdio.h>

/* For wchar_t */
# include <ctype.h>

/* For strlen, memset, memcpy */
# include <string.h>

/* For malloc */
# include <malloc.h>
#endif /* URI_DOXYGEN */



typedef struct UriIp4Struct {
	unsigned char data[4];
} UriIp4;

typedef struct UriIp6Struct {
	unsigned char data[16];
} UriIp6;



void uriWriteQuadToDoubleByte(const unsigned char * hexDigits, int digitCount,
		unsigned char * output);
unsigned char uriGetOctetValue(const unsigned char * digits, int digitCount);



#endif /* URI_INDEPENDENT_H */
