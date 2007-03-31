/*
 * uriparser - RFC 3986 URI parsing library
 *
 * Copyright (C) 2007, Weijia Song <songweijia@gmail.com>
 * Copyright (C) 2007, Sebastian Pipping <webmaster@hartwork.org>
 * All rights reserved.
 *
 * Redistribution  and use in source and binary forms, with or without
 * modification,  are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions   of  source  code  must  retain  the   above
 *       copyright  notice, this list of conditions and the  following
 *       disclaimer.
 *
 *     * Redistributions  in  binary  form must  reproduce  the  above
 *       copyright  notice, this list of conditions and the  following
 *       disclaimer   in  the  documentation  and/or  other  materials
 *       provided with the distribution.
 *
 *     * Neither  the name of the <ORGANIZATION> nor the names of  its
 *       contributors  may  be  used to endorse  or  promote  products
 *       derived  from  this software without specific  prior  written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT  NOT
 * LIMITED  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS
 * FOR  A  PARTICULAR  PURPOSE ARE DISCLAIMED. IN NO EVENT  SHALL  THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL,    SPECIAL,   EXEMPLARY,   OR   CONSEQUENTIAL   DAMAGES
 * (INCLUDING,  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES;  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT  LIABILITY,  OR  TORT (INCLUDING  NEGLIGENCE  OR  OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file UriBase.h
 * Holds definitions independent of the encoding pass.
 */

#ifndef URI_INDEPENDENT_H
#define URI_INDEPENDENT_H 1



/* Version helper macro */
#define URI_ANSI_TO_UNICODE(x) L##x



/* Version */
#define URI_VER_MAJOR           0
#define URI_VER_MINOR           3
#define URI_VER_RELEASE         2
#define URI_VER_SUFFIX_ANSI     ""
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



typedef int UriBool;

#define URI_TRUE     1
#define URI_FALSE    0



/* Error codes */
#define URI_SUCCESS         0
#define URI_ERROR_SYNTAX    1
#define URI_ERROR_NULL      2 /* One of the params passed was NULL although it mustn't be */
#define URI_ERROR_MALLOC    3 /* Not used yet, TODO */



#ifndef URI_DOXYGEN
# include <stdio.h> /* For NULL */
# include <ctype.h> /* For wchar_t */
# include <string.h> /* For strlen, memset, memcpy */
# include <stdlib.h> /* For malloc */
#endif /* URI_DOXYGEN */



/**
 * Holds an IPv4 address.
 */
typedef struct UriIp4Struct {
	unsigned char data[4]; /**< Each octet in one byte */
} UriIp4;



/**
 * Holds an IPv6 address.
 */
typedef struct UriIp6Struct {
	unsigned char data[16]; /**< Each quad in two bytes */
} UriIp6;



void uriWriteQuadToDoubleByte(const unsigned char * hexDigits, int digitCount,
		unsigned char * output);
unsigned char uriGetOctetValue(const unsigned char * digits, int digitCount);



#endif /* URI_INDEPENDENT_H */
