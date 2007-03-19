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
 * @file UriTwice.h
 * Holds the RFC 3986 %URI parser interface.
 */

#if (defined(URI_PASS_ANSI) && !defined(URI_TWICE_H_ANSI)) \
	|| (defined(URI_PASS_UNICODE) && !defined(URI_TWICE_H_UNICODE))
#ifdef URI_PASS_ANSI
# define URI_TWICE_H_ANSI 1
#else
# define URI_TWICE_H_UNICODE 1
#endif



#ifdef URI_PASS_ANSI
# include "UriAnsi.h"
#else
# include "UriUnicode.h"
#endif



/**
 * Specifies a range of characters within a string.
 * The range includes all characters from <code>first<code>
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

	void * reserved; /**< Not used */
} URI_TYPE(PathSegment);



/**
 * Holds strcutured host information.
 * This is either a IPv4, IPv6, plain
 * text for IPvFutere or all zero for
 * a registered name.
 */
typedef union URI_TYPE(HostDataUnion) {
	UriIp4 * ip4; /**< IPv4 address */
	UriIp6 * ip6; /**< IPv6 address */
	URI_TYPE(TextRange) ipFuture; /**< IPvFuture address */
} URI_TYPE(HostData);



/**
 * Represents a state of the %URI parser.
 * Missing components can be NULL to reflect
 * a components absence.
 */
typedef struct URI_TYPE(ParserStruct) {
	URI_TYPE(TextRange) scheme; /**< Scheme (e.g. "http") */
	URI_TYPE(TextRange) userInfo; /**< User info (e.g. "user:pass") */
	URI_TYPE(TextRange) hostText; /**< Host text (set for all hosts) */
	URI_TYPE(HostData) hostData; /**< Structured host type specific data */
	URI_TYPE(TextRange) portText; /**< Port (e.g. "80") */
	URI_TYPE(PathSegment) * pathHead; /**< Head of a linked list of path segments */
	URI_TYPE(PathSegment) * pathTail; /**< Tail of the list behind pathHead */
	URI_TYPE(TextRange) query; /**< Query without leading "?" */
	URI_TYPE(TextRange) fragment; /**< Query without leading "#" */

	const URI_CHAR * errorPos; /**< Pointer to position in case of a parsing error */
	UriBool absolutePath; /**< Absolute path flag, meaningless if absolute %URI */

	void * reserved; /**< Not used */
} URI_TYPE(Parser);



#ifdef URI_TESTING
UriBool URI_FUNC(ParseIpSix)(const URI_CHAR * text);
#endif



/**
 * Parses a RFC 3986 URI.
 *
 * @param parser	Parser state data
 * @param first		Pointer to the first character to parse
 * @param afterLast	Pointer to the character after the last to parse
 * @return			URI_SUCCESS on success, URI_ERROR otherwise
 */
UriBool URI_FUNC(ParseUriEx)(URI_TYPE(Parser) * parser, const URI_CHAR * first,
		const URI_CHAR * afterLast);



/**
 * Parses a RFC 3986 %URI.
 *
 * @param parser	Parser state data
 * @param text		Text to parse
 * @return			URI_SUCCESS on success, URI_ERROR otherwise
 */
UriBool URI_FUNC(ParseUri)(URI_TYPE(Parser) * parser, const URI_CHAR * text);



/**
 * Frees the the memory associated with all members
 * of the parse state structure. Note that the parser
 * itself is not freed, only its members.
 *
 * @param parser	Parser whose members should be freed
 */
void URI_FUNC(FreeMembers)(URI_TYPE(Parser) * parser);



#endif /* URI_TWICE_H_ANSI and URI_TWICE_H_UNICODE */
