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
# include "UriCommon.c"
# undef URI_PASS_ANSI
# define URI_PASS_UNICODE 1
# include "UriCommon.c"
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




/* Used to point to from empty path segments.
 * X.first and X.afterLast must be the same non-NULL value then. */
static const URI_CHAR URI_FUNC(SafeToPointTo) = _UT('X');




void URI_FUNC(ResetUri)(URI_TYPE(Uri) * uri) {
	memset(uri, 0, sizeof(URI_TYPE(Uri)));
}



/* Properly removes "." and ".." path segments */
UriBool URI_FUNC(RemoveDotSegments)(URI_TYPE(Uri) * uri) {
	URI_TYPE(PathSegment) * walker;
	if ((uri == NULL) || (uri->pathHead == NULL)) {
		return URI_TRUE;
	}

	walker = uri->pathHead;
	walker->reserved = NULL; /* Prev pointer */
	do {
		int len = (int)(walker->text.afterLast - walker->text.first);
		switch (len) {
		case 1:
			if ((walker->text.first)[0] == _UT('.')) {
				/* Path "." -> remove this segement */
				URI_TYPE(PathSegment) * const prev = walker->reserved;
				URI_TYPE(PathSegment) * const nextBackup = walker->next;
				if (prev != NULL) {
					/* Not first segment */
					prev->next = walker->next;
				} else {
					/* First segment -> update head pointer */
					uri->pathHead = walker->next;
				}
				if (walker->next != NULL) {
					walker->next->reserved = prev;
				} else {
					/* Last segment -> insert "" segment to represent trailing slash, update tail */
					URI_TYPE(PathSegment) * const segment = malloc(1 * sizeof(URI_TYPE(PathSegment)));
					if (segment == NULL) {
						if (uri->owner) {
							free(walker->text.first);
						}
						free(walker);
						return URI_FALSE; /* Raises malloc error */
					}
					memset(segment, 0, sizeof(URI_TYPE(PathSegment)));
					segment->text.first = &URI_FUNC(SafeToPointTo);
					segment->text.afterLast = &URI_FUNC(SafeToPointTo);
					prev->next = segment;
					uri->pathTail = segment;
				}

				if (uri->owner) {
					free(walker->text.first);
				}
				free(walker);
				walker = nextBackup;
			} else {
				if (walker->next != NULL) {
					walker->next->reserved = walker;
				} else {
					/* Last segment -> update tail */
					uri->pathTail = walker;
				}
				walker = walker->next;
			}
			break;

		case 2:
			if (((walker->text.first)[0] == _UT('.'))
					&& ((walker->text.first)[1] == _UT('.'))) {
				/* Path ".." -> remove this and the previous segment */
				URI_TYPE(PathSegment) * const prev = walker->reserved;
				URI_TYPE(PathSegment) * prevPrev;
				URI_TYPE(PathSegment) * const nextBackup = walker->next;
				if (prev != NULL) {
					/* Not first segment */
					prevPrev = prev->reserved;
					if (prevPrev != NULL) {
						/* Not even prev is the first one */
						prevPrev->next = walker->next;
						if (walker->next != NULL) {
							walker->next->reserved = prevPrev;
						} else {
							/* Last segment -> insert "" segment to represent trailing slash, update tail */
							URI_TYPE(PathSegment) * const segment = malloc(1 * sizeof(URI_TYPE(PathSegment)));
							if (segment == NULL) {
								if (uri->owner) {
									free(walker->text.first);
								}
								free(walker);

								if (uri->owner) {
									free(prev->text.first);
								}
								free(prev);

								return URI_FALSE; /* Raises malloc error */
							}
							memset(segment, 0, sizeof(URI_TYPE(PathSegment)));
							segment->text.first = &URI_FUNC(SafeToPointTo);
							segment->text.afterLast = &URI_FUNC(SafeToPointTo);
							prevPrev->next = segment;
							uri->pathTail = segment;
						}

						if (uri->owner) {
							free(walker->text.first);
						}
						free(walker);

						if (uri->owner) {
							free(prev->text.first);
						}
						free(prev);

						walker = nextBackup;
					} else {
						/* Prev is the first segment */
						uri->pathHead = walker->next;
						if (walker->next != NULL) {
							walker->next->reserved = NULL;
						} else {
							/* Last segment -> update tail */
							uri->pathTail = NULL;
						}

						if (uri->owner) {
							free(walker->text.first);
						}
						free(walker);

						if (uri->owner) {
							free(prev->text.first);
						}
						free(prev);

						walker = nextBackup;
					}
				} else {
					URI_TYPE(PathSegment) * const nextBackup = walker->next;
					/* First segment -> update head pointer */
					uri->pathHead = walker->next;
					if (walker->next != NULL) {
						walker->next->reserved = NULL;
					} else {
						/* Last segment -> update tail */
						uri->pathTail = NULL;
					}

					if (uri->owner) {
						free(walker->text.first);
					}
					free(walker);

					walker = nextBackup;
				}
			} else {
				if (walker->next != NULL) {
					walker->next->reserved = walker;
				} else {
					/* Last segment -> update tail */
					uri->pathTail = walker;
				}
				walker = walker->next;
			}
			break;

		default:
			if (walker->next != NULL) {
				walker->next->reserved = walker;
			} else {
				/* Last segment -> update tail */
				uri->pathTail = walker;
			}
			walker = walker->next;
			break;

		}
	} while (walker != NULL);

	/* Fix path if only one empty segment */
	if ((uri->pathHead != NULL)
			&& (uri->pathHead->next == NULL)
			&& (uri->pathHead->text.first == uri->pathHead->text.afterLast)) {
		free(uri->pathHead);
		uri->pathHead = NULL;
		uri->pathTail = NULL;
	}

	return URI_TRUE;
}



unsigned char URI_FUNC(HexdigToInt)(URI_CHAR hexdig) {
	switch (hexdig) {
	case _UT('0'):
	case _UT('1'):
	case _UT('2'):
	case _UT('3'):
	case _UT('4'):
	case _UT('5'):
	case _UT('6'):
	case _UT('7'):
	case _UT('8'):
	case _UT('9'):
		return (unsigned char)(9 + hexdig - _UT('9'));

	case _UT('a'):
	case _UT('b'):
	case _UT('c'):
	case _UT('d'):
	case _UT('e'):
	case _UT('f'):
		return (unsigned char)(15 + hexdig - _UT('f'));

	case _UT('A'):
	case _UT('B'):
	case _UT('C'):
	case _UT('D'):
	case _UT('E'):
	case _UT('F'):
		return (unsigned char)(15 + hexdig - _UT('F'));

	default:
		return 0;
	}
}



URI_CHAR URI_FUNC(HexToLetter)(unsigned int value) {
	switch (value) {
	case  0: return _UT('0');
	case  1: return _UT('1');
	case  2: return _UT('2');
	case  3: return _UT('3');
	case  4: return _UT('4');
	case  5: return _UT('5');
	case  6: return _UT('6');
	case  7: return _UT('7');
	case  8: return _UT('8');
	case  9: return _UT('9');

	/* Uppercase recommended in section 2.1. of RFC 3986 *
	 * http://tools.ietf.org/html/rfc3986#section-2.1    */
	case 10: return _UT('A');
	case 11: return _UT('B');
	case 12: return _UT('C');
	case 13: return _UT('D');
	case 14: return _UT('E');
	default: return _UT('F');
	}
}



/* Checks if a URI has the host component set. */
UriBool URI_FUNC(IsHostSet)(const URI_TYPE(Uri) * uri) {
	return (uri != NULL)
			&& ((uri->hostText.first != NULL)
				|| (uri->hostData.ip4 != NULL)
				|| (uri->hostData.ip6 != NULL)
				|| (uri->hostData.ipFuture.first != NULL)
			);
}



#endif
