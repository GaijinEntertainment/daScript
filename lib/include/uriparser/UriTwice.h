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



typedef struct URI_TYPE(PathSegmentStruct) {
	const URI_CHAR * first;
	const URI_CHAR * afterLast;

	struct URI_TYPE(PathSegmentStruct) * next;

	void * reserved;
} URI_TYPE(PathSegment);



typedef struct URI_TYPE(ParserStruct) {
	const URI_CHAR * schemeFirst;
	const URI_CHAR * schemeAfterLast;

	const URI_CHAR * userInfoFirst;
	const URI_CHAR * userInfoAfterLast;

	/* TODO Make struct/union combo of this? */
	const URI_CHAR * hostFirst; /* Set for all host types, */
	const URI_CHAR * hostAfterLast; /* not just regname */
	UriIp4 * ip4;
	UriIp6 * ip6;
	const URI_CHAR * ipFutureFirst;
	const URI_CHAR * ipFutureAfterLast;

	const URI_CHAR * portFirst;
	const URI_CHAR * portAfterLast;
	URI_TYPE(PathSegment) * pathHead;
	URI_TYPE(PathSegment) * pathTail;
	const URI_CHAR * queryFirst;
	const URI_CHAR * queryAfterLast;
	const URI_CHAR * fragmentFirst;
	const URI_CHAR * fragmentAfterLast;

	const URI_CHAR * errorPos;
	UriBool absolutePath;

	void * reserved;
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
 * Parses a RFC 3986 URI.
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
