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

#ifndef URI_URIPARSER_H
#define URI_URIPARSER_H



#include <uriparser/UriConfig.h>

#ifdef URI_ENABLE_LEGACY



#define	DEFAULT_MAX_LEN    64
#define	MAX_URI_SCHEME     DEFAULT_MAX_LEN
#define	MAX_URI_USERINFO   256
#define	MAX_URI_HOST       256
#define	MAX_URI_PATH       1024
#define MAX_URI_QUERY      4096
#define	MAX_URI_FRAGMENT   DEFAULT_MAX_LEN



enum URIType{
	URIURI,
	URIRelativeRef
};



enum HOSTType{
	IPv4Address,
	IPv6Address,
	IPvFuture,
	RegName
};



enum PathType{
	PathNoScheme,
	PathAbsolute,
	PathRootless,
	PathAbEmpty
};



typedef struct uri_struct {
    enum URIType utype;
    enum HOSTType htype;
    enum PathType ptype;
    int hasPort;
    char * scheme;
    char * userinfo;
    char * host;
    unsigned short port;
    char * path;
    char * query;
    char * fragment;
} URI;



/**
 * Initializes uriparser.
 * Call before any call of URIParseString.
 *
 * @return	0 for success, other number else
 */
int URIParserInit();



/**
 * Cleans up after uriparser.
 * Call when no more parsing will be done.
 */
void URIParserCleanup();



/**
 * Parses a string and builds an URI structure
 * of that in possible. All non-NULL string
 * members of that URI have to be manually
 * freed later.
 *
 * @param uri	Output destination
 * @param str	Input string to parse
 * @return		0 for success, other number else
 */
int URIParseString(URI * uri, const char * str);



/**
 * Frees the memory occupied by the members of a URI struct.
 * Call when when a URI instance is no longer needed.
 *
 * @param uri	URI struct to "empty"
 */
void URIFree(URI * uri);



#endif /* URI_ENABLE_LEGACY */

#endif /* URI_URIPARSER_H */
