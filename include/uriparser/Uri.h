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
 * @file Uri.h
 * Holds the RFC 3986 %URI parser interface.
 * NOTE: This header includes itself twice.
 */

#if (defined(URI_PASS_ANSI) && !defined(URI_TWICE_H_ANSI)) \
	|| (defined(URI_PASS_UNICODE) && !defined(URI_TWICE_H_UNICODE)) \
	|| (!defined(URI_PASS_ANSI) && !defined(URI_PASS_UNICODE))
/* What encodings are enabled? */
#include "UriDefsConfig.h"
#if (!defined(URI_PASS_ANSI) && !defined(URI_PASS_UNICODE))
/* Include SELF twice */
# define URI_PASS_ANSI 1
# include "Uri.h"
# undef URI_PASS_ANSI
# define URI_PASS_UNICODE 1
# include "Uri.h"
# undef URI_PASS_UNICODE
/* Only one pass for each encoding */
#elif (defined(URI_PASS_ANSI) && !defined(URI_TWICE_H_ANSI) \
	&& defined(URI_ENABLE_ANSI)) || (defined(URI_PASS_UNICODE) \
	&& !defined(URI_TWICE_H_UNICODE) && defined(URI_ENABLE_UNICODE))
# ifdef URI_PASS_ANSI
#  define URI_TWICE_H_ANSI 1
#  include "UriDefsAnsi.h"
# else
#  define URI_TWICE_H_UNICODE 1
#  include "UriDefsUnicode.h"
# endif



#ifdef __cplusplus
extern "C" {
#endif



#ifndef URI_DOXYGEN
# include "UriBase.h"
#endif



/**
 * Specifies a range of characters within a string.
 * The range includes all characters from <code>first</code>
 * to one before <code>afterLast</code>. So if both are
 * non-NULL the difference is the length of the text range.
 */
typedef struct URI_TYPE(TextRangeStruct) {
	const URI_CHAR * first; /**< Pointer to first character */
	const URI_CHAR * afterLast; /**< Pointer to character after the last one still in */
} URI_TYPE(TextRange);



/**
 * Represents a segment within a %URI path.
 * More precisely it is a node in a linked
 * list of path segments.
 */
typedef struct URI_TYPE(PathSegmentStruct) {
	URI_TYPE(TextRange) text; /**< Path segment name */
	struct URI_TYPE(PathSegmentStruct) * next; /**< Pointer to the next path segment in the list, can be NULL if last already */

	void * reserved; /**< Reserved to the parser */
} URI_TYPE(PathSegment);



/**
 * Holds structured host information.
 * This is either a IPv4, IPv6, plain
 * text for IPvFuture or all zero for
 * a registered name.
 */
typedef struct URI_TYPE(HostDataStruct) {
	UriIp4 * ip4; /**< IPv4 address */
	UriIp6 * ip6; /**< IPv6 address */
	URI_TYPE(TextRange) ipFuture; /**< IPvFuture address */
} URI_TYPE(HostData);



/**
 * Represents an RFC 3986 %URI.
 * Missing components can be {NULL, NULL} ranges.
 */
typedef struct URI_TYPE(UriStruct) {
	URI_TYPE(TextRange) scheme; /**< Scheme (e.g. "http") */
	URI_TYPE(TextRange) userInfo; /**< User info (e.g. "user:pass") */
	URI_TYPE(TextRange) hostText; /**< Host text (set for all hosts, excluding square brackets) */
	URI_TYPE(HostData) hostData; /**< Structured host type specific data */
	URI_TYPE(TextRange) portText; /**< Port (e.g. "80") */
	URI_TYPE(PathSegment) * pathHead; /**< Head of a linked list of path segments */
	URI_TYPE(PathSegment) * pathTail; /**< Tail of the list behind pathHead */
	URI_TYPE(TextRange) query; /**< Query without leading "?" */
	URI_TYPE(TextRange) fragment; /**< Query without leading "#" */
	UriBool absolutePath; /**< Absolute path flag, meaningless if %URI is absolute */

	/* TODO reserved deep copy flag? */
} URI_TYPE(Uri);



/**
 * Represents a state of the %URI parser.
 * Missing components can be NULL to reflect
 * a components absence.
 */
typedef struct URI_TYPE(ParserStateStruct) {
	URI_TYPE(Uri) * uri; /**< Plug in the %URI structure to be filled while parsing here */
	int errorCode; /**< Code identifying the occured error */
	const URI_CHAR * errorPos; /**< Pointer to position in case of a syntax error */

	void * reserved; /**< Reserved to the parser */
} URI_TYPE(ParserState);



/**
 * Parses a RFC 3986 URI.
 *
 * @param state		Parser state with set output %URI, must not be NULL
 * @param first		Pointer to the first character to parse, must not be NULL
 * @param afterLast	Pointer to the character after the last to parse, must not be NULL
 * @return			0 on success, error code otherwise
 */
int URI_FUNC(ParseUriEx)(URI_TYPE(ParserState) * state,
		const URI_CHAR * first, const URI_CHAR * afterLast);



/**
 * Parses a RFC 3986 %URI.
 *
 * @param state		Parser state with set output %URI, must not be NULL
 * @param text		Text to parse, must not be NULL
 * @return			0 on success, error code otherwise
 */
int URI_FUNC(ParseUri)(URI_TYPE(ParserState) * state,
		const URI_CHAR * text);



/**
 * Frees all memory associated with the members
 * of the %URI structure. Note that the structure
 * itself is not freed, only its members.
 *
 * @param uri	%URI structure whose members should be freed
 */
void URI_FUNC(FreeUriMembers)(URI_TYPE(Uri) * uri);



/**
 * Percent-encodes all unreserved characters from the input string and
 * writes the encoded version to the output string.
 * Be sure to allocate <u>3 times</u> the space of the input buffer for
 * the output buffer for <c>normalizeBreaks == URI_FALSE</c> and <u>6 times</u>
 * the space for <c>normalizeBreaks == URI_FALSE</c>
 * (since e.g. "\x0d" becomes "%0D%0A" in that case)
 *
 * @param in				Text source
 * @param out				Encoded text destination
 * @param spaceToPlus		Wether to convert ' ' to '+' or not
 * @param normalizeBreaks	Wether to convert CR and LF to CR-LF or not.
 * @return					Position of terminator in output string
 */
const URI_CHAR * URI_FUNC(Escape)(const URI_CHAR * in, URI_CHAR * out,
		UriBool spaceToPlus, UriBool normalizeBreaks);



/**
 * Unescapes percent-encoded groups in a given string.
 * E.g. "%20" will become " ". Unescaping is done in place.
 * The return value will be point to the new position
 * of the terminating zero. Use this value to get the new
 * length of the string. NULL is only returned if <code>inout</code>
 * is NULL.
 *
 * @param inout			Text to unescape/decode
 * @param plusToSpace	Whether to convert '+' to ' ' or not
 * @return				Pointer to new position of the terminating zero
 */
const URI_CHAR * URI_FUNC(UnescapeInPlaceEx)(URI_CHAR * inout,
		UriBool plusToSpace);



/**
 * Unescapes percent-encoded groups in a given string.
 * E.g. "%20" will become " ". Unescaping is done in place.
 * The return value will be point to the new position
 * of the terminating zero. Use this value to get the new
 * length of the string. NULL is only returned if <code>inout</code>
 * is NULL.
 * NOTE: '+' is not decoded to ' '. Use UnescapeInPlaceEx for that instead.
 *
 * @param inout			Text to unescape/decode
 * @return				Pointer to new position of the terminating zero
 */
const URI_CHAR * URI_FUNC(UnescapeInPlace)(URI_CHAR * inout);



/**
 * Performs reference resolution as described in
 * <a href="http://tools.ietf.org/html/rfc3986#section-5.2.2">section 5.2.2 of RFC 3986</a>.
 *
 * @param absoluteDest		Result %URI
 * @param relativeSource	Reference to resolve
 * @param absoluteBase		Base %URI to apply
 * @return					Error code or 0 on success
 */
int URI_FUNC(AddBaseUri)(URI_TYPE(Uri) * absoluteDest,
		const URI_TYPE(Uri) * relativeSource,
		const URI_TYPE(Uri) * absoluteBase);



/**
 * Checks two URIs for equivalence. Comparison is done
 * the naive way, without prior normalization.
 * NOTE: Two <code>NULL</code> URIs are equal as well.
 *
 * @param a		First %URI
 * @param b		Second %URI
 * @return		<code>URI_TRUE</code> when equal, <code>URI_FAlSE</code> else
 */
UriBool URI_FUNC(EqualsUri)(const URI_TYPE(Uri) * a, const URI_TYPE(Uri) * b);



/**
 * Calculates the number of characters needed to store the
 * string representation of the given %URI excluding the
 * terminator.
 *
 * @param uri				%URI to measure
 * @param charsRequired		Length of the string representation in characters <u>excluding</u> terminator
 * @return					Error code or 0 on success
 */
int URI_FUNC(ToStringCharsRequired)(const URI_TYPE(Uri) * uri,
		int * charsRequired);



/**
 * Converts a %URI structure back to text as described in
 * <a href="http://tools.ietf.org/html/rfc3986#section-5.3">section 5.3 of RFC 3986</a>.
 *
 * @param dest			Output destination
 * @param uri			%URI to convert
 * @param maxChars		Maximum number of characters to copy <u>including</u> terminator
 * @param charsWritten	Number of characters written, can be lower than maxChars even if the %URI is too long!
 * @return				Error code or 0 on success
 */
int URI_FUNC(ToString)(URI_CHAR * dest, const URI_TYPE(Uri) * uri, int maxChars, int * charsWritten);



/* TODO
int URI_FUNC(NormalizeSyntaxMaskRequired)(const URI_TYPE(Uri) * uri);
int URI_FUNC(NormalizeSyntaxEx)(URI_TYPE(Uri) * uri, int mask);
int URI_FUNC(NormalizeSyntax)(URI_TYPE(Uri) * uri);
*/



#ifdef __cplusplus
}
#endif



#endif
#endif
