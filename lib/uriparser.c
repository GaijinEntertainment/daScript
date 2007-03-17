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

#ifndef URI_NO_LEGACY



#include <uriparser.h>
#include <uriparser/Uri.h>
#include <malloc.h>



int URIParserInit() {
	/* Nothing to do */
	return 0;
}



void URIParserCleanup() {
	/* Nothing to do */
}



void uriMallocCopyHelper(char ** dest, const char * first, const char * afterLast) {
	if ((first != NULL) && (afterLast != NULL)) {
		const int len = (int)(afterLast - first);
		*dest = malloc((len + 1) * sizeof(char));
		memcpy(*dest, first, len);
		*dest[len] = '\0';
	} else {
		*dest = NULL;
	}
}



int URIParseString(URI * uri, const char * str) {
	int res;
	UriParserA parser;

	if (uriParseUriA(&parser, str) != URI_SUCCESS) {
		return 1;
	}
	if (uri == NULL) {
		return 0;
	}

	uri->utype = (parser.schemeFirst == NULL) ? URIRelativeRef : URIURI;

	if (parser.ip4 != NULL) {
		uri->htype = IPv4Address;
	} else if (parser.ip6 != NULL) {
		uri->htype = IPv6Address;
	} else if (parser.ipFutureFirst != NULL) {
		uri->htype = IPvFuture;
	} else {
		uri->htype = RegName;
	}

	/* uri->ptype = XXX; */
	uri->hasPort = (parser.portFirst != NULL);
	uriMallocCopyHelper(&uri->scheme, parser.schemeFirst, parser.schemeAfterLast);
	uriMallocCopyHelper(&uri->userinfo, parser.userInfoFirst, parser.userInfoAfterLast);
	uriMallocCopyHelper(&uri->host, parser.hostFirst, parser.hostAfterLast);
	uriMallocCopyHelper(&uri->port, parser.portFirst, parser.portAfterLast);
	/* uri->path = XXX; */
	uriMallocCopyHelper(&uri->query, parser.queryFirst, parser.queryAfterLast);
	uriMallocCopyHelper(&uri->fragment, parser.fragmentFirst, parser.fragmentAfterLast);

	return 0;
}



void URIFree(URI * uri) {
	if (uri == NULL) {
		return;
	}

	if (uri->scheme != NULL) {
		free(uri->scheme);
		uri->scheme = NULL;
	}
	if (uri->userinfo != NULL) {
		free(uri->userinfo);
		uri->userinfo = NULL;
	}
	if (uri->host != NULL) {
		free(uri->host);
		uri->host = NULL;
	}
	if (uri->path != NULL) {
		free(uri->path);
		uri->path = NULL;
	}
	if (uri->query != NULL) {
		free(uri->query);
		uri->query = NULL;
	}
	if (uri->fragment != NULL) {
		free(uri->fragment);
		uri->fragment = NULL;
	}
}



#endif /* URI_NO_LEGACY */
