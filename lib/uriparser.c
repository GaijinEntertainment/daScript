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

#include <uriparser/UriConfig.h>

#ifdef URI_ENABLE_LEGACY



#include <uriparser.h>
#include <uriparser/Uri.h>

/* For atoi */
#include <stdlib.h>



int URIParserInit() {
	/* Nothing to do */
	return 0;
}



void URIParserCleanup() {
	/* Nothing to do */
}



static URI_INLINE void uriMallocCopy(char ** dest, const char * first, const char * afterLast) {
	if ((first != NULL) && (afterLast != NULL)) {
		const int len = (int)(afterLast - first);
		*dest = malloc((len + 1) * sizeof(char));
		memcpy(*dest, first, len);
		(*dest)[len] = '\0';
	} else {
		*dest = malloc(1 * sizeof(char));
		(*dest)[0] = '\0';
	}
}



static URI_INLINE void uriMallocCopyPrepend(char ** dest, const char * first, const char * afterLast, char prefix) {
	if ((first != NULL) && (afterLast != NULL)) {
		const int len = (int)(afterLast - first);
		*dest = malloc((len + 2) * sizeof(char));
		(*dest)[0] = prefix;
		memcpy((*dest) + 1, first, len);
		(*dest)[len + 1] = '\0';
	} else {
		*dest = malloc(1 * sizeof(char));
		(*dest[0]) = '\0';
	}
}



static URI_INLINE void uriMallocCopyAppend(char ** dest, const char * first, const char * afterLast, char suffix) {
	if ((first != NULL) && (afterLast != NULL)) {
		const int len = (int)(afterLast - first);
		*dest = malloc((len + 2) * sizeof(char));
		memcpy(*dest, first, len);
		(*dest)[len] = suffix;
		(*dest)[len + 1] = '\0';
	} else {
		*dest = malloc(1 * sizeof(char));
		(*dest)[0] = '\0';
	}
}



static URI_INLINE unsigned char uriMakePort(const char * first, const char * afterLast) {
	const int len = (int)(afterLast - first);
	char * text = malloc((len + 1) * sizeof(char));
	int res;
	memcpy(text, first, len);
	text[len] = '\0';
	res = atoi(text);
	free(text);
	return (unsigned char)res;
}



static URI_INLINE void uriMakePathString(char ** destPath,
		const UriPathSegmentA * pathHead, UriBool prependSlash) {
	int strLenSum = 0;
	int segCount = 0;
	const UriPathSegmentA * segWalk = pathHead;
	char * pathWalk; /* Init later */
	int fullPathLen; /* Init later */

	while (segWalk != NULL) {
		strLenSum += (int)(segWalk->afterLast - segWalk->first);
		segCount++;
		segWalk = segWalk->next;
	}
	fullPathLen = prependSlash + strLenSum + (segCount - 1);
	*destPath = malloc((fullPathLen + 1) * sizeof(char));
	pathWalk = *destPath;
	segWalk = pathHead;
	if (prependSlash) {
		pathWalk[0]= '/';
		pathWalk++;
	}
	if (segWalk != NULL) {
		for (;;) {
			const int len = (int)(segWalk->afterLast - segWalk->first);
			memcpy(pathWalk, segWalk->first, len);
			segWalk = segWalk->next;
			if (segWalk == NULL) {
				pathWalk[len] = '\0';
				break;
			} else {
				pathWalk[len] = '/';
				pathWalk += (len + 1);
			}
		}
	}
}



int URIParseString(URI * uri, const char * str) {
	UriParserA parser;
	if (URI_SUCCESS != uriParseUriA(&parser, str)) {
		return 1;
	}
	if (uri == NULL) {
		/* No output needed */
		uriFreeMembersA(&parser);
		return 0;
	}

	/* URI type */
	uri->utype = (parser.schemeFirst == NULL) ? URIRelativeRef : URIURI;

	/* Host type */
	if (parser.hostFirst == NULL) {
		/* Just to clone 0.2.1 behavior */
		uri->htype = IPv4Address;
	} else {
		if (parser.ip4 != NULL) {
			uri->htype = IPv4Address;
		} else if (parser.ip6 != NULL) {
			uri->htype = IPv6Address;
		} else if (parser.ipFutureFirst != NULL) {
			uri->htype = IPvFuture;
		} else {
			uri->htype = RegName;
		}
	}

	/* Path type */
	if (parser.hostFirst != NULL) {
		uri->ptype = PathAbEmpty;
	} else if (parser.absolutePath) {
		uri->ptype = PathAbsolute;
	} else {
		if (parser.schemeFirst != NULL) {
			uri->ptype = PathRootless;
		} else {
			uri->ptype = PathNoScheme;
		}
	}

	/* Port presence */
	uri->hasPort = (parser.portFirst != NULL);

	/* Scheme */
	uriMallocCopyAppend(&uri->scheme, parser.schemeFirst, parser.schemeAfterLast, ':');

	/* User info */
	uriMallocCopy(&uri->userinfo, parser.userInfoFirst, parser.userInfoAfterLast);

	/* Host */
	uriMallocCopy(&uri->host, parser.hostFirst, parser.hostAfterLast);

	/* Port */
	if (parser.portFirst != NULL) {
		uri->port = uriMakePort(parser.portFirst, parser.portAfterLast);
	}

	/* Path */
	uriMakePathString(&uri->path, parser.pathHead,
			(parser.hostFirst != NULL) || parser.absolutePath);

	/* Query */
	uriMallocCopyPrepend(&uri->query, parser.queryFirst, parser.queryAfterLast, '?');

	/* Fragment */
	uriMallocCopyPrepend(&uri->fragment, parser.fragmentFirst, parser.fragmentAfterLast, '#');

	uriFreeMembersA(&parser);
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



#endif /* URI_ENABLE_LEGACY */
