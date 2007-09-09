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

/* What encodings are enabled? */
#include <uriparser/UriDefsConfig.h>
#if (!defined(URI_PASS_ANSI) && !defined(URI_PASS_UNICODE))
/* Include SELF twice */
# define URI_PASS_ANSI 1
# include "UriFile.c"
# undef URI_PASS_ANSI
# define URI_PASS_UNICODE 1
# include "UriFile.c"
# undef URI_PASS_UNICODE
#else
# ifdef URI_PASS_ANSI
#  include <uriparser/UriDefsAnsi.h>
# else
#  include <uriparser/UriDefsUnicode.h>
#  include <wchar.h>
# endif



#ifndef URI_DOXYGEN
# include <uriparser/Uri.h>
#endif



static URI_INLINE int URI_FUNC(FilenameToUriString)(const URI_CHAR * filename,
		URI_CHAR * uriString, UriBool unix) {
	const URI_CHAR * const prefix = unix ? _UT("file://") : _UT("file:///");
	const int prefixLen = unix ? 7 : 8;
	URI_CHAR * walker = uriString + prefixLen;

	/* Copy and escape on the fly */
	memcpy(uriString, prefix, prefixLen * sizeof(URI_CHAR));
	URI_FUNC(Escape)(filename, uriString + prefixLen, URI_FALSE, URI_FALSE);
	
	/* Convert backslashes to forward slashes */
	if (!unix) {
		while (walker[0] != _UT('\0')) {
			if (walker[0] == _UT('\\')) {
				walker[0] = _UT('/');
			}
			walker++;
		}
	}

	return URI_SUCCESS;
}



static URI_INLINE int URI_FUNC(UriStringToFilename)(const URI_CHAR * uriString,
		URI_CHAR * filename, UriBool unix) {
	const URI_CHAR * const prefix = unix ? _UT("file://") : _UT("file:///");
	const int prefixLen = unix ? 7 : 8;
	URI_CHAR * walker = filename;
	size_t charsToCopy;
	
	if (URI_STRNCMP(uriString, prefix, prefixLen)) {
		return URI_ERROR_SYNTAX;
	}
	charsToCopy = URI_STRLEN(uriString + prefixLen) + 1;
	memcpy(filename, uriString + prefixLen, charsToCopy * sizeof(URI_CHAR));
	URI_FUNC(UnescapeInPlaceEx)(filename, URI_FALSE, URI_BR_DONT_TOUCH);
	
	/* Convert forward slashes to backslashes */
	if (!unix) {
		while (walker[0] != _UT('\0')) {
			if (walker[0] == _UT('/')) {
				walker[0] = _UT('\\');
			}
			walker++;
		}
	}

	return URI_SUCCESS;
}



int URI_FUNC(UnixFilenameToUriString)(const URI_CHAR * filename, URI_CHAR * uriString) {
	return URI_FUNC(FilenameToUriString)(filename, uriString, URI_TRUE);
}



int URI_FUNC(WindowsFilenameToUriString)(const URI_CHAR * filename, URI_CHAR * uriString) {
	return URI_FUNC(FilenameToUriString)(filename, uriString, URI_FALSE);
}



int URI_FUNC(UriStringToUnixFilename)(const URI_CHAR * uriString, URI_CHAR * filename) {
	return URI_FUNC(UriStringToFilename)(uriString, filename, URI_TRUE);	
}



int URI_FUNC(UriStringToWindowsFilename)(const URI_CHAR * uriString, URI_CHAR * filename) {
	return URI_FUNC(UriStringToFilename)(uriString, filename, URI_FALSE);
}



#endif
