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
# include "UriQuery.c"
# undef URI_PASS_ANSI
# define URI_PASS_UNICODE 1
# include "UriQuery.c"
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
# include "UriCommon.h"
#endif



static int URI_FUNC(ComposeQueryEngine)(URI_CHAR * dest,
		const URI_TYPE(QueryList) * queryList,
		int maxChars, int * charsWritten, int * charsRequired);

static UriBool URI_FUNC(AppendQueryItem)(URI_TYPE(QueryList) ** prevNext,
		int * itemCount, const URI_CHAR * keyFirst, const URI_CHAR * keyAfterLast,
		const URI_CHAR * valueFirst, const URI_CHAR * valueAfterLast,
		UriBool plusToSpace, UriBreakConversion breakConversion);



int URI_FUNC(ComposeQueryCharsRequired)(const URI_TYPE(QueryList) * queryList,
		int * charsRequired) {
	return URI_FUNC(ComposeQueryEngine)(NULL, queryList, 0, NULL,
			charsRequired);
}



int URI_FUNC(ComposeQuery)(URI_CHAR * dest,
		const URI_TYPE(QueryList) * queryList, int maxChars, int * charsWritten) {
	return URI_FUNC(ComposeQueryEngine)(dest, queryList, maxChars,
			charsWritten, NULL);
}



int URI_FUNC(ComposeQueryEngine)(URI_CHAR * dest,
		const URI_TYPE(QueryList) * queryList,
		int maxChars, int * charsWritten, int * charsRequired) {
	/* TODO */

	while (queryList != NULL) {
		queryList = queryList->next;
	}

	return URI_ERROR_NOT_IMPLEMENTED;
}



UriBool URI_FUNC(AppendQueryItem)(URI_TYPE(QueryList) ** prevNext,
		int * itemCount, const URI_CHAR * keyFirst, const URI_CHAR * keyAfterLast,
		const URI_CHAR * valueFirst, const URI_CHAR * valueAfterLast,
		UriBool plusToSpace, UriBreakConversion breakConversion) {
	const int keyLen = keyAfterLast - keyFirst;
	const int valueLen = valueAfterLast - valueFirst;
	URI_CHAR * key;
	URI_CHAR * value;

	if ((prevNext == NULL) || (itemCount == NULL)
			|| (keyFirst == NULL) || (keyAfterLast == NULL)
			|| (keyFirst > keyAfterLast) || (valueFirst > valueAfterLast)) {
		return URI_TRUE;
	}

	/* Append new empty item */
	*prevNext = malloc(1 * sizeof(URI_TYPE(QueryList)));
	if (*prevNext == NULL) {
		return URI_FALSE; /* Raises malloc error */
	}
	(*prevNext)->next = NULL;


	/* Fill key */
	key = malloc((keyLen + 1) * sizeof(URI_CHAR));
	if (key == NULL) {
		free(*prevNext);
		*prevNext = NULL;
		return URI_FALSE; /* Raises malloc error */
	}

	key[keyLen] = _UT('\0');
	if (keyLen > 0) {
		/* Copy 1:1 */
		memcpy(key, keyFirst, keyLen * sizeof(URI_CHAR));

		/* Unescape */
		uriUnescapeInPlaceExA(key, plusToSpace, breakConversion);
	}
	(*prevNext)->key = key;


	/* Fill value */
	if (valueFirst != NULL) {
		value = malloc((valueLen + 1) * sizeof(URI_CHAR));
		if (value == NULL) {
			free((*prevNext)->key);
			free(*prevNext);
			*prevNext = NULL;
			return URI_FALSE; /* Raises malloc error */
		}

		value[valueLen] = _UT('\0');
		if (valueLen > 0) {
			/* Copy 1:1 */
			memcpy(value, valueFirst, valueLen * sizeof(URI_CHAR));

			/* Unescape */
			uriUnescapeInPlaceExA(value, plusToSpace, breakConversion);
		}
		(*prevNext)->value = value;
	} else {
		value = NULL;
	}
	(*prevNext)->value = value;

	(*itemCount)++;
	return URI_TRUE;
}



void URI_FUNC(FreeQueryList)(URI_TYPE(QueryList) * queryList) {
	while (queryList != NULL) {
		URI_TYPE(QueryList) * nextBackup = queryList->next;
		free(queryList);
		queryList = nextBackup;
	}
}



int URI_FUNC(DissectQueryMalloc)(URI_TYPE(QueryList) ** dest, int * itemCount,
		const URI_CHAR * first, const URI_CHAR * afterLast,
		UriBool plusToSpace, UriBreakConversion breakConversion) {
	const URI_CHAR * walk = first;
	const URI_CHAR * keyFirst = first;
	const URI_CHAR * keyAfterLast = first;
	const URI_CHAR * valueFirst = NULL;
	const URI_CHAR * valueAfterLast = NULL;
	URI_TYPE(QueryList) ** prevNext = dest;
	int nullCounter;
	int * itemsAppended = (itemCount == NULL) ? &nullCounter : itemCount;

	if ((dest == NULL) || (first == NULL) || (afterLast == NULL)) {
		return URI_ERROR_NULL;
	}

	if (first > afterLast) {
		return URI_ERROR_RANGE_INVALID;
	}

	*itemsAppended = 0;

	/* Parse query string */
	for (; walk < afterLast; walk++) {
		switch (*walk) {
		case _UT('&'):
			if (URI_FUNC(AppendQueryItem)(prevNext, itemsAppended,
					keyFirst, keyAfterLast, valueFirst, valueAfterLast,
					plusToSpace, breakConversion)
					== URI_FALSE) {
				/* Free list we built */
				*itemsAppended = 0;
				URI_FUNC(FreeQueryList)(*dest);
				return URI_ERROR_MALLOC;
			}

			/* Make future items children of the current */
			if (prevNext != NULL) {
				prevNext = &((*prevNext)->next);
			}

			if (walk + 1 < afterLast) {
				keyFirst = walk + 1;
				keyAfterLast = walk + 1;
			} else {
				keyFirst = NULL;
				keyAfterLast = NULL;
			}
			valueFirst = NULL;
			valueAfterLast = NULL;
			break;

		case _UT('='):
			keyAfterLast = walk;
			if (walk + 1 < afterLast) {
				valueFirst = walk + 1;
				valueAfterLast = walk + 1;
			}
			break;

		default:
			break;
		}
	}

	if (valueFirst != NULL) {
		/* Must be key/value pair */
		valueAfterLast = walk;
	} else {
		/* Must be key only */
		keyAfterLast = walk;
	}

	if (URI_FUNC(AppendQueryItem)(prevNext, itemsAppended, keyFirst, keyAfterLast,
			valueFirst, valueAfterLast, plusToSpace, breakConversion)
			== URI_FALSE) {
		/* Free list we built */
		*itemsAppended = 0;
		URI_FUNC(FreeQueryList)(*dest);
		return URI_ERROR_MALLOC;
	}

	return URI_SUCCESS;
}



#endif
