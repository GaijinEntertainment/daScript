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
 * @file uriparser.h
 * Holds the legacy code interface.
 */

#ifndef URI_URIPARSER_H
#define URI_URIPARSER_H



#include <uriparser/UriDefsConfig.h>

#ifdef URI_ENABLE_LEGACY



#define	DEFAULT_MAX_LEN    64

#define	MAX_URI_SCHEME     DEFAULT_MAX_LEN
#define	MAX_URI_USERINFO   256
#define	MAX_URI_HOST       256
#define	MAX_URI_PATH       1024
#define MAX_URI_QUERY      4096
#define	MAX_URI_FRAGMENT   DEFAULT_MAX_LEN



/**
 * Specifies a URI's type.
 * A %URI can either be absolute or relative.
 *
 * @deprecated	Will be removed for version 0.5.0
 */
enum URIType {
	URIURI, /**< Absolute %URI */
	URIRelativeRef /**< Relative %URI */
};



/**
 * Specifies a URI's host type.
 * The host in a %URI can be one of iPv4, IPv6,
 * IPvFuture or a registered name.
 *
 * @deprecated	Will be removed for version 0.5.0
 */
enum HOSTType {
	IPv4Address, /**< IPv4 address */
	IPv6Address, /**< IPv6 address */
	IPvFuture, /**< Future IP version address */
	RegName /**< Registered name */
};



/**
 * Specifies the type of a URI's path.
 *
 * @deprecated	Will be removed for version 0.5.0
 */
enum PathType {
	PathNoScheme, /**< Non-root path of a relative URI */
	PathAbsolute, /**< Relative path starting at root level */
	PathRootless, /**< Non-root path directly starting after the scheme of an absolute %URI */
	PathAbEmpty /**< Possibly empty path after authority part */
};



/**
 * Represents an RFC 3986 %URI.
 * Components must not be NULL. Use a malloced
 * empty string to flag a component as missing.
 *
 * @deprecated	Will be removed for version 0.5.0
 */
typedef struct uri_struct {
    enum URIType utype; /**< Type of the %URI */
    enum HOSTType htype; /**< Type of the host, meaningless if host missing */
    enum PathType ptype; /**< Type of the path */
    int hasPort; /**< Port presence flag */
    char * scheme; /**< Scheme if present (e.g. "http:") */
    char * userinfo; /**< User info if present (e.g. "user:pass") */
    char * host; /**< Host if present (also set for IPs) */
    unsigned short port; /**< Port number, meaningless if hasPort == 0 */
    char * path; /**< Path (e.g. "/one/two/") */
    char * query; /**< Query if present (e.g. "?test") */
    char * fragment; /**< Fragment if present (e.g. "#about") */
} URI;



/**
 * Initializes uriparser.
 * Call before any call of URIParseString.
 *
 * @return		0 for success, other number else
 * @deprecated	Will be removed for version 0.5.0
 */
int URIParserInit();



/**
 * Cleans up after uriparser.
 * Call when no more parsing will be done.
 *
 * @deprecated	Will be removed for version 0.5.0
 */
void URIParserCleanup();



/**
 * Parses a string and builds an URI structure
 * of that in possible. All non-NULL string
 * members of that %URI have to be manually
 * freed later.
 *
 * @param uri	Output destination
 * @param str	Input string to parse
 * @return		0 for success, other number else
 * @deprecated	Will be removed for version 0.5.0
 */
int URIParseString(URI * uri, const char * str);



/**
 * Frees the memory occupied by the members of a URI struct.
 * Call when when a URI instance is no longer needed.
 *
 * @param uri	URI struct to "empty"
 * @deprecated	Will be removed for version 0.5.0
 */
void URIFree(URI * uri);



#endif /* URI_ENABLE_LEGACY */

#endif /* URI_URIPARSER_H */
