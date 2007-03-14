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

#ifdef URI_TWICE_H_ENABLE



#ifdef URI_ANSI
# include "UriAnsi.h"
#else
# include "UriUnicode.h"
#endif



URI_TYPE(PathSegment) {
	const URI_CHAR * first;
	const URI_CHAR * afterLast;

	URI_TYPE(PathSegment) * next;

	void * reserved;
};



URI_TYPE(Parser) {
	const URI_CHAR * schemeFirst;
	const URI_CHAR * schemeAfterLast;

	const URI_CHAR * userInfoFirst;
	const URI_CHAR * userInfoAfterLast;
	const URI_CHAR * hostFirst;
	const URI_CHAR * hostAfterLast;
	const URI_CHAR * portFirst;
	const URI_CHAR * portAfterLast;

	URI_TYPE(PathSegment) * pathHead;
	URI_TYPE(PathSegment) * pathTail;

	const URI_CHAR * queryFirst;
	const URI_CHAR * queryAfterLast;
	const URI_CHAR * fragmentFirst;
	const URI_CHAR * fragmentAfterLast;

	void * reserved;
};



#ifdef URI_TESTING
UriBool URI_FUNC(ParseIpSix)(const URI_CHAR * text);
#endif

UriBool URI_FUNC(ParseUriEx)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
UriBool URI_FUNC(ParseUri)(URI_TYPE(Parser) * parser, const URI_CHAR * text);



#endif /* URI_TWICE_H_ENABLE */
