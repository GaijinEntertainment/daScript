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
 * @file Uri.c
 * Holds the RFC 3986 %URI parser implementation.
 * NOTE: This source file includes itself twice.
 */

/* What encodings are enabled? */
#include <uriparser/UriDefsConfig.h>
#if (!defined(URI_PASS_ANSI) && !defined(URI_PASS_UNICODE))
/* Include SELF twice */
# define URI_PASS_ANSI 1
# include "Uri.c"
# undef URI_PASS_ANSI
# define URI_PASS_UNICODE 1
# include "Uri.c"
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
# include <uriparser/UriIp4.h>
#endif



/* Prototypes */
static const URI_CHAR * URI_FUNC(ParseAuthority)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseAuthorityTwo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseHexZero)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseHierPart)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseIpFutLoop)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseIpFutStopGo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseIpLit2)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseIPv6address2)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseMustBeSegmentNzNc)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseOwnHost)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseOwnHost2)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseOwnHostUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseOwnHostUserInfoNz)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseOwnPortUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseOwnUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParsePartHelperTwo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParsePathAbsEmpty)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParsePathAbsNoLeadSlash)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParsePathRootless)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParsePchar)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParsePctEncoded)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParsePctSubUnres)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParsePort)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseQueryFrag)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseSegment)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseSegmentNz)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseSegmentNzNcOrScheme2)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseUriReference)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseUriTail)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseUriTailTwo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static const URI_CHAR * URI_FUNC(ParseZeroMoreSlashSegs)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);

static void URI_FUNC(OnExitOwnHost2)(URI_TYPE(ParserState) * state, const URI_CHAR * first);
static void URI_FUNC(OnExitOwnHostUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first);
static void URI_FUNC(OnExitOwnPortUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first);
static void URI_FUNC(OnExitSegmentNzNcOrScheme2)(URI_TYPE(ParserState) * state, const URI_CHAR * first);
static void URI_FUNC(OnExitPartHelperTwo)(URI_TYPE(ParserState) * state);

static void URI_FUNC(ResetParserState)(URI_TYPE(ParserState) * state);
static void URI_FUNC(ResetUri)(URI_TYPE(Uri) * uri);
static void URI_FUNC(PushPathSegment)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast);
static void URI_FUNC(Stop)(URI_TYPE(ParserState) * state, const URI_CHAR * errorPos);
/*
static void URI_FUNC(StopEx)(URI_TYPE(ParserState) * state, const URI_CHAR * errorPos, int errorCode);
*/
unsigned char URI_FUNC(HexdigToInt)(URI_CHAR hexdig);



static URI_INLINE void URI_FUNC(Stop)(URI_TYPE(ParserState) * state,
		const URI_CHAR * errorPos) {
	URI_FUNC(FreeUriMembers)(state->uri);
	state->errorPos = errorPos;
	state->errorCode = URI_ERROR_SYNTAX;
}


/*
static URI_INLINE void URI_FUNC(StopEx)(URI_TYPE(ParserState) * state,
		const URI_CHAR * errorPos, int errorCode) {
	URI_FUNC(FreeUriMembers)(state->uri);
	state->errorPos = errorPos;
	state->errorCode = errorCode;
}
*/


/*
 * [authority]-><[>[ipLit2][authorityTwo]
 * [authority]->[ownHostUserInfoNz]
 * [authority]-><NULL>
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParseAuthority)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('['):
		{
			const URI_CHAR * const afterIpLit2
					= URI_FUNC(ParseIpLit2)(state, first + 1, afterLast);
			if (afterIpLit2 == NULL) {
				return NULL;
			}
			state->uri->hostText.first = first + 1; /* HOST BEGIN */
			return URI_FUNC(ParseAuthorityTwo)(state, afterIpLit2, afterLast);
		}

	case _UT('!'):
	case _UT('$'):
	case _UT('%'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(':'):
	case _UT(';'):
	case _UT('@'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		state->uri->userInfo.first = first; /* USERINFO BEGIN */
		return URI_FUNC(ParseOwnHostUserInfoNz)(state, first, afterLast);

	default:
		return first;
	}
}

/*
 * [authorityTwo]-><:>[port]
 * [authorityTwo]-><NULL>
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParseAuthorityTwo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT(':'):
		{
			const URI_CHAR * const afterPort = URI_FUNC(ParsePort)(state, first + 1, afterLast);
			if (afterPort == NULL) {
				return NULL;
			}
			state->uri->portText.first = first + 1; /* PORT BEGIN */
			state->uri->portText.afterLast = afterPort; /* PORT END */
			return afterPort;
		}

	default:
		return first;
	}
}



/*
 * [hexZero]->[HEXDIG][hexZero]
 * [hexZero]-><NULL>
 */
static const URI_CHAR * URI_FUNC(ParseHexZero)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
		return URI_FUNC(ParseHexZero)(state, first + 1, afterLast);

	default:
		return first;
	}
}



/*
 * [hierPart]->[pathRootless]
 * [hierPart]-></>[partHelperTwo]
 * [hierPart]-><NULL>
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParseHierPart)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('%'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(':'):
	case _UT(';'):
	case _UT('@'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		return URI_FUNC(ParsePathRootless)(state, first, afterLast);

	case _UT('/'):
		return URI_FUNC(ParsePartHelperTwo)(state, first + 1, afterLast);

	default:
		return first;
	}
}



/*
 * [ipFutLoop]->[subDelims][ipFutStopGo]
 * [ipFutLoop]->[unreserved][ipFutStopGo]
 * [ipFutLoop]-><:>[ipFutStopGo]
 */
static const URI_CHAR * URI_FUNC(ParseIpFutLoop)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(Stop)(state, first);
		return NULL;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(':'):
	case _UT(';'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		return URI_FUNC(ParseIpFutStopGo)(state, first + 1, afterLast);

	default:
		URI_FUNC(Stop)(state, first);
		return NULL;
	}
}



/*
 * [ipFutStopGo]->[ipFutLoop]
 * [ipFutStopGo]-><NULL>
 */
static const URI_CHAR * URI_FUNC(ParseIpFutStopGo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(':'):
	case _UT(';'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		return URI_FUNC(ParseIpFutLoop)(state, first, afterLast);

	default:
		return first;
	}
}



/*
 * [ipFuture]-><v>[HEXDIG][hexZero]<.>[ipFutLoop]
 */
static const URI_CHAR * URI_FUNC(ParseIpFuture)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(Stop)(state, first);
		return NULL;
	}

	/*
	First character has already been
	checked before entering this rule.

	switch (*first) {
	case _UT('v'):
	*/
		if (first + 1 >= afterLast) {
			URI_FUNC(Stop)(state, first + 1);
			return NULL;
		}

		switch (first[1]) {
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
		case _UT('a'):
		case _UT('A'):
		case _UT('b'):
		case _UT('B'):
		case _UT('c'):
		case _UT('C'):
		case _UT('d'):
		case _UT('D'):
		case _UT('e'):
		case _UT('E'):
		case _UT('f'):
		case _UT('F'):
			{
				const URI_CHAR * afterIpFutLoop;
				const URI_CHAR * const afterHexZero
						= URI_FUNC(ParseHexZero)(state, first + 2, afterLast);
				if (afterHexZero == NULL) {
					return NULL;
				}
				if ((afterHexZero >= afterLast)
						|| (*afterHexZero != _UT('.'))) {
					URI_FUNC(Stop)(state, afterHexZero);
					return NULL;
				}
				state->uri->hostText.first = first; /* HOST BEGIN */
				state->uri->hostData.ipFuture.first = first; /* IPFUTURE BEGIN */
				afterIpFutLoop = URI_FUNC(ParseIpFutLoop)(state, afterHexZero + 1, afterLast);
				if (afterIpFutLoop == NULL) {
					return NULL;
				}
				state->uri->hostText.afterLast = afterIpFutLoop; /* HOST END */
				state->uri->hostData.ipFuture.afterLast = afterIpFutLoop; /* IPFUTURE END */
				return afterIpFutLoop;
			}

		default:
			URI_FUNC(Stop)(state, first + 1);
			return NULL;
		}

	/*
	default:
		URI_FUNC(Stop)(state, first);
		return NULL;
	}
	*/
}



/*
 * [ipLit2]->[ipFuture]<]>
 * [ipLit2]->[IPv6address2]
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParseIpLit2)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(Stop)(state, first);
		return NULL;
	}

	switch (*first) {
	case _UT('v'):
		{
			const URI_CHAR * const afterIpFuture
					= URI_FUNC(ParseIpFuture)(state, first, afterLast);
			if (afterIpFuture == NULL) {
				return NULL;
			}
			if ((afterIpFuture >= afterLast)
					|| (*afterIpFuture != _UT(']'))) {
				URI_FUNC(Stop)(state, first);
				return NULL;
			}
			return afterIpFuture + 1;
		}

	case _UT(':'):
	case _UT(']'):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
		state->uri->hostData.ip6 = malloc(1 * sizeof(UriIp6)); /* Freed when stopping on parse error */
		/* TODO NULL check */
		return URI_FUNC(ParseIPv6address2)(state, first, afterLast);

	default:
		URI_FUNC(Stop)(state, first);
		return NULL;
	}
}



/*
 * [IPv6address2]->..<]>
 */
static const URI_CHAR * URI_FUNC(ParseIPv6address2)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	int zipperEver = 0;
	int quadsDone = 0;
	int digitCount = 0;
	unsigned char digitHistory[4];
	int ip4OctetsDone = 0;

	unsigned char quadsAfterZipper[14];
	int quadsAfterZipperCount = 0;


	for (;;) {
		if (first >= afterLast) {
			URI_FUNC(Stop)(state, first);
			return NULL;
		}

		/* Inside IPv4 part? */
		if (ip4OctetsDone > 0) {
			/* Eat rest of IPv4 address */
			for (;;) {
				switch (*first) {
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
					if (digitCount == 4) {
						URI_FUNC(Stop)(state, first);
						return NULL;
					}
					digitHistory[digitCount++] = (unsigned char)(9 + *first - _UT('9'));
					break;

				case _UT('.'):
					if ((ip4OctetsDone == 4) /* NOTE! */
							|| (digitCount == 0)
							|| (digitCount == 4)) {
						/* Invalid digit or octet count */
						URI_FUNC(Stop)(state, first);
						return NULL;
					} else if ((digitCount > 1)
							&& (digitHistory[0] == 0)) {
						/* Leading zero */
						URI_FUNC(Stop)(state, first - digitCount);
						return NULL;
					} else if ((digitCount > 2)
							&& (digitHistory[1] == 0)) {
						/* Leading zero */
						URI_FUNC(Stop)(state, first - digitCount + 1);
						return NULL;
					} else if ((digitCount == 3)
							&& (100 * digitHistory[0]
								+ 10 * digitHistory[1]
								+ digitHistory[2] > 255)) {
						/* Octet value too large */
						if (digitHistory[0] > 2) {
							URI_FUNC(Stop)(state, first - 3);
						} else if (digitHistory[1] > 5) {
							URI_FUNC(Stop)(state, first - 2);
						} else {
							URI_FUNC(Stop)(state, first - 1);
						}
						return NULL;
					}

					/* Copy IPv4 octet */
					state->uri->hostData.ip6->data[16 - 4 + ip4OctetsDone] = uriGetOctetValue(digitHistory, digitCount);
					digitCount = 0;
					ip4OctetsDone++;
					break;

				case _UT(']'):
					if ((ip4OctetsDone != 3) /* NOTE! */
							|| (digitCount == 0)
							|| (digitCount == 4)) {
						/* Invalid digit or octet count */
						URI_FUNC(Stop)(state, first);
						return NULL;
					} else if ((digitCount > 1)
							&& (digitHistory[0] == 0)) {
						/* Leading zero */
						URI_FUNC(Stop)(state, first - digitCount);
						return NULL;
					} else if ((digitCount > 2)
							&& (digitHistory[1] == 0)) {
						/* Leading zero */
						URI_FUNC(Stop)(state, first - digitCount + 1);
						return NULL;
					} else if ((digitCount == 3)
							&& (100 * digitHistory[0]
								+ 10 * digitHistory[1]
								+ digitHistory[2] > 255)) {
						/* Octet value too large */
						if (digitHistory[0] > 2) {
							URI_FUNC(Stop)(state, first - 3);
						} else if (digitHistory[1] > 5) {
							URI_FUNC(Stop)(state, first - 2);
						} else {
							URI_FUNC(Stop)(state, first - 1);
						}
						return NULL;
					}

					state->uri->hostText.afterLast = first; /* HOST END */

					/* Copy missing quads right before IPv4 */
					memcpy(state->uri->hostData.ip6->data + 16 - 4 - 2 * quadsAfterZipperCount,
								quadsAfterZipper, 2 * quadsAfterZipperCount);

					/* Copy last IPv4 octet */
					state->uri->hostData.ip6->data[16 - 4 + 3] = uriGetOctetValue(digitHistory, digitCount);

					return first + 1;

				default:
					URI_FUNC(Stop)(state, first);
					return NULL;
				}
				first++;
			}
		} else {
			/* Eat while no dot in sight */
			int letterAmong = 0;
			int walking = 1;
			do {
				switch (*first) {
				case _UT('a'):
				case _UT('b'):
				case _UT('c'):
				case _UT('d'):
				case _UT('e'):
				case _UT('f'):
					letterAmong = 1;
					if (digitCount == 4) {
						URI_FUNC(Stop)(state, first);
						return NULL;
					}
					digitHistory[digitCount] = (unsigned char)(15 + *first - _UT('f'));
					digitCount++;
					break;

				case _UT('A'):
				case _UT('B'):
				case _UT('C'):
				case _UT('D'):
				case _UT('E'):
				case _UT('F'):
					letterAmong = 1;
					if (digitCount == 4) {
						URI_FUNC(Stop)(state, first);
						return NULL;
					}
					digitHistory[digitCount] = (unsigned char)(15 + *first - _UT('F'));
					digitCount++;
					break;

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
					if (digitCount == 4) {
						URI_FUNC(Stop)(state, first);
						return NULL;
					}
					digitHistory[digitCount] = (unsigned char)(9 + *first - _UT('9'));
					digitCount++;
					break;

				case _UT(':'):
					{
						int setZipper = 0;

						/* Too many quads? */
						if (quadsDone > 8 - zipperEver) {
							URI_FUNC(Stop)(state, first);
							return NULL;
						}

						/* "::"? */
						if (first + 1 >= afterLast) {
							URI_FUNC(Stop)(state, first + 1);
							return NULL;
						}
						if (first[1] == _UT(':')) {
							const int resetOffset = 2 * (quadsDone + (digitCount > 0));

							first++;
							if (zipperEver) {
								URI_FUNC(Stop)(state, first);
								return NULL; /* "::.+::" */
							}

							/* Zero everything after zipper */
							memset(state->uri->hostData.ip6->data + resetOffset, 0, 16 - resetOffset);
							setZipper = 1;

							/* ":::+"? */
							if (first + 1 >= afterLast) {
								URI_FUNC(Stop)(state, first + 1);
								return NULL; /* No ']' yet */
							}
							if (first[1] == _UT(':')) {
								URI_FUNC(Stop)(state, first + 1);
								return NULL; /* ":::+ "*/
							}
						}
						if (digitCount > 0) {
							if (zipperEver) {
								uriWriteQuadToDoubleByte(digitHistory, digitCount, quadsAfterZipper + 2 * quadsAfterZipperCount);
								quadsAfterZipperCount++;
							} else {
								uriWriteQuadToDoubleByte(digitHistory, digitCount, state->uri->hostData.ip6->data + 2 * quadsDone);
							}
							quadsDone++;
							digitCount = 0;
						}
						letterAmong = 0;

						if (setZipper) {
							zipperEver = 1;
						}
					}
					break;

				case _UT('.'):
					if ((quadsDone > 6) /* NOTE */
							|| (!zipperEver && (quadsDone < 6))
							|| letterAmong
							|| (digitCount == 0)
							|| (digitCount == 4)) {
						/* Invalid octet before */
						URI_FUNC(Stop)(state, first);
						return NULL;
					} else if ((digitCount > 1)
							&& (digitHistory[0] == 0)) {
						/* Leading zero */
						URI_FUNC(Stop)(state, first - digitCount);
						return NULL;
					} else if ((digitCount > 2)
							&& (digitHistory[1] == 0)) {
						/* Leading zero */
						URI_FUNC(Stop)(state, first - digitCount + 1);
						return NULL;
					} else if ((digitCount == 3)
							&& (100 * digitHistory[0]
								+ 10 * digitHistory[1]
								+ digitHistory[2] > 255)) {
						/* Octet value too large */
						if (digitHistory[0] > 2) {
							URI_FUNC(Stop)(state, first - 3);
						} else if (digitHistory[1] > 5) {
							URI_FUNC(Stop)(state, first - 2);
						} else {
							URI_FUNC(Stop)(state, first - 1);
						}
						return NULL;
					}

					/* Copy first IPv4 octet */
					state->uri->hostData.ip6->data[16 - 4] = uriGetOctetValue(digitHistory, digitCount);
					digitCount = 0;

					/* Switch over to IPv4 loop */
					ip4OctetsDone = 1;
					walking = 0;
					break;

				case _UT(']'):
					/* Too little quads? */
					if (!zipperEver && !((quadsDone == 7) && (digitCount > 0))) {
						URI_FUNC(Stop)(state, first);
						return NULL;
					}

					if (digitCount > 0) {
						if (zipperEver) {
							uriWriteQuadToDoubleByte(digitHistory, digitCount, quadsAfterZipper + 2 * quadsAfterZipperCount);
							quadsAfterZipperCount++;
						} else {
							uriWriteQuadToDoubleByte(digitHistory, digitCount, state->uri->hostData.ip6->data + 2 * quadsDone);
						}
						/*
						quadsDone++;
						digitCount = 0;
						*/
					}

					/* Copy missing quads to the end */
					memcpy(state->uri->hostData.ip6->data + 16 - 2 * quadsAfterZipperCount,
								quadsAfterZipper, 2 * quadsAfterZipperCount);

					state->uri->hostText.afterLast = first; /* HOST END */
					return first + 1; /* Fine */

				default:
					URI_FUNC(Stop)(state, first);
					return NULL;
				}
				first++;

				if (first >= afterLast) {
					URI_FUNC(Stop)(state, first);
					return NULL; /* No ']' yet */
				}
			} while (walking);
		}
	}

	/* We should never get here */
	URI_FUNC(Stop)(state, first);
	return NULL;
}



/*
 * [mustBeSegmentNzNc]->[pctEncoded][mustBeSegmentNzNc]
 * [mustBeSegmentNzNc]->[subDelims][mustBeSegmentNzNc]
 * [mustBeSegmentNzNc]->[unreserved][mustBeSegmentNzNc]
 * [mustBeSegmentNzNc]->[uriTail] // can take <NULL>
 * [mustBeSegmentNzNc]-></>[segment][zeroMoreSlashSegs][uriTail]
 * [mustBeSegmentNzNc]-><@>[mustBeSegmentNzNc]
 */
static const URI_CHAR * URI_FUNC(ParseMustBeSegmentNzNc)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(PushPathSegment)(state, state->uri->scheme.first, first); /* SEGMENT BOTH */
		state->uri->scheme.first = NULL; /* Not a scheme, reset */
		return afterLast;
	}

	switch (*first) {
	case _UT('%'):
		{
			const URI_CHAR * const afterPctEncoded
					= URI_FUNC(ParsePctEncoded)(state, first, afterLast);
			if (afterPctEncoded == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseMustBeSegmentNzNc)(state, afterPctEncoded, afterLast);
		}

	case _UT('@'):
	case _UT('!'):
	case _UT('$'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('*'):
	case _UT(','):
	case _UT(';'):
	case _UT('\''):
	case _UT('+'):
	case _UT('='):
	case _UT('-'):
	case _UT('.'):
	case _UT('_'):
	case _UT('~'):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		return URI_FUNC(ParseMustBeSegmentNzNc)(state, first + 1, afterLast);

	case _UT('/'):
		{
			const URI_CHAR * afterZeroMoreSlashSegs;
			const URI_CHAR * afterSegment;
			URI_FUNC(PushPathSegment)(state, state->uri->scheme.first, first); /* SEGMENT BOTH */
			state->uri->scheme.first = NULL; /* Not a scheme, reset */
			afterSegment = URI_FUNC(ParseSegment)(state, first + 1, afterLast);
			if (afterSegment == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(state, first + 1, afterSegment); /* SEGMENT BOTH */
			afterZeroMoreSlashSegs
					= URI_FUNC(ParseZeroMoreSlashSegs)(state, afterSegment, afterLast);
			if (afterZeroMoreSlashSegs == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseUriTail)(state, afterZeroMoreSlashSegs, afterLast);
		}

	default:
		URI_FUNC(PushPathSegment)(state, state->uri->scheme.first, first); /* SEGMENT BOTH */
		state->uri->scheme.first = NULL; /* Not a scheme, reset */
		return URI_FUNC(ParseUriTail)(state, first, afterLast);
	}
}



/*
 * [ownHost]-><[>[ipLit2][authorityTwo]
 * [ownHost]->[ownHost2] // can take <NULL>
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParseOwnHost)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('['):
		{
			const URI_CHAR * const afterIpLit2
					= URI_FUNC(ParseIpLit2)(state, first + 1, afterLast);
			if (afterIpLit2 == NULL) {
				return NULL;
			}
			state->uri->hostText.first = first + 1; /* HOST BEGIN */
			return URI_FUNC(ParseAuthorityTwo)(state, afterIpLit2, afterLast);
		}

	default:
		return URI_FUNC(ParseOwnHost2)(state, first, afterLast);
	}
}



static URI_INLINE void URI_FUNC(OnExitOwnHost2)(URI_TYPE(ParserState) * state, const URI_CHAR * first) {
	state->uri->hostText.afterLast = first; /* HOST END */

	/* Valid IPv4 or just a regname? */
	state->uri->hostData.ip4 = malloc(1 * sizeof(UriIp4)); /* Freed when stopping on parse error */
	/* TODO NULL check */
	if (URI_FUNC(ParseIpFourAddress)(state->uri->hostData.ip4->data,
			state->uri->hostText.first, state->uri->hostText.afterLast)) {
		/* Not IPv4 */
		free(state->uri->hostData.ip4);
		state->uri->hostData.ip4 = NULL;
	}
}



/*
 * [ownHost2]->[authorityTwo] // can take <NULL>
 * [ownHost2]->[pctSubUnres][ownHost2]
 */
static const URI_CHAR * URI_FUNC(ParseOwnHost2)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(OnExitOwnHost2)(state, first);
		return afterLast;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('%'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(';'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		{
			const URI_CHAR * const afterPctSubUnres
					= URI_FUNC(ParsePctSubUnres)(state, first, afterLast);
			if (afterPctSubUnres == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseOwnHost2)(state, afterPctSubUnres, afterLast);
		}

	default:
		URI_FUNC(OnExitOwnHost2)(state, first);
		return URI_FUNC(ParseAuthorityTwo)(state, first, afterLast);
	}
}



static URI_INLINE void URI_FUNC(OnExitOwnHostUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first) {
	state->uri->hostText.first = state->uri->userInfo.first; /* Host instead of userInfo, update */
	state->uri->userInfo.first = NULL; /* Not a userInfo, reset */
	state->uri->hostText.afterLast = first; /* HOST END */

	/* Valid IPv4 or just a regname? */
	state->uri->hostData.ip4 = malloc(1 * sizeof(UriIp4)); /* Freed when stopping on parse error */
	/* TODO NULL check */
	if (URI_FUNC(ParseIpFourAddress)(state->uri->hostData.ip4->data,
			state->uri->hostText.first, state->uri->hostText.afterLast)) {
		/* Not IPv4 */
		free(state->uri->hostData.ip4);
		state->uri->hostData.ip4 = NULL;
	}
}



/*
 * [ownHostUserInfo]->[ownHostUserInfoNz]
 * [ownHostUserInfo]-><NULL>
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParseOwnHostUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(OnExitOwnHostUserInfo)(state, first);
		return afterLast;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('%'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(':'):
	case _UT(';'):
	case _UT('@'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		return URI_FUNC(ParseOwnHostUserInfoNz)(state, first, afterLast);

	default:
		URI_FUNC(OnExitOwnHostUserInfo)(state, first);
		return first;
	}
}



/*
 * [ownHostUserInfoNz]->[pctSubUnres][ownHostUserInfo]
 * [ownHostUserInfoNz]-><:>[ownPortUserInfo]
 * [ownHostUserInfoNz]-><@>[ownHost]
 */
static const URI_CHAR * URI_FUNC(ParseOwnHostUserInfoNz)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(Stop)(state, first);
		return NULL;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('%'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(';'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		{
			const URI_CHAR * const afterPctSubUnres
					= URI_FUNC(ParsePctSubUnres)(state, first, afterLast);
			if (afterPctSubUnres == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseOwnHostUserInfo)(state, afterPctSubUnres, afterLast);
		}

	case _UT(':'):
		state->uri->hostText.afterLast = first; /* HOST END */
		state->uri->portText.first = first + 1; /* PORT BEGIN */
		return URI_FUNC(ParseOwnPortUserInfo)(state, first + 1, afterLast);

	case _UT('@'):
		state->uri->userInfo.afterLast = first; /* USERINFO END */
		state->uri->hostText.first = first + 1; /* HOST BEGIN */
		return URI_FUNC(ParseOwnHost)(state, first + 1, afterLast);

	default:
		URI_FUNC(Stop)(state, first);
		return NULL;
	}
}



static URI_INLINE void URI_FUNC(OnExitOwnPortUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first) {
	state->uri->hostText.first = state->uri->userInfo.first; /* Host instead of userInfo, update */
	state->uri->userInfo.first = NULL; /* Not a userInfo, reset */
	state->uri->portText.afterLast = first; /* PORT END */

	/* Valid IPv4 or just a regname? */
	state->uri->hostData.ip4 = malloc(1 * sizeof(UriIp4)); /* Freed when stopping on parse error */
	/* TODO NULL check */
	if (URI_FUNC(ParseIpFourAddress)(state->uri->hostData.ip4->data,
			state->uri->hostText.first, state->uri->hostText.afterLast)) {
		/* Not IPv4 */
		free(state->uri->hostData.ip4);
		state->uri->hostData.ip4 = NULL;
	}
}



/*
 * [ownPortUserInfo]->[ALPHA][ownUserInfo]
 * [ownPortUserInfo]->[DIGIT][ownPortUserInfo]
 * [ownPortUserInfo]-><.>[ownUserInfo]
 * [ownPortUserInfo]-><_>[ownUserInfo]
 * [ownPortUserInfo]-><~>[ownUserInfo]
 * [ownPortUserInfo]-><->[ownUserInfo]
 * [ownPortUserInfo]-><NULL>
 */
static const URI_CHAR * URI_FUNC(ParseOwnPortUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(OnExitOwnPortUserInfo)(state, first);
		return afterLast;
	}

	switch (*first) {
	case _UT('.'):
	case _UT('_'):
	case _UT('~'):
	case _UT('-'):
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		state->uri->hostText.afterLast = NULL; /* Not a host, reset */
		state->uri->portText.first = NULL; /* Not a port, reset */
		return URI_FUNC(ParseOwnUserInfo)(state, first + 1, afterLast);

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
		return URI_FUNC(ParseOwnPortUserInfo)(state, first + 1, afterLast);

	default:
		URI_FUNC(OnExitOwnPortUserInfo)(state, first);
		return first;
	}
}



/*
 * [ownUserInfo]->[pctSubUnres][ownUserInfo]
 * [ownUserInfo]-><:>[ownUserInfo]
 * [ownUserInfo]-><@>[ownHost]
 */
static const URI_CHAR * URI_FUNC(ParseOwnUserInfo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(Stop)(state, first);
		return NULL;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('%'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(';'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		{
			const URI_CHAR * const afterPctSubUnres
					= URI_FUNC(ParsePctSubUnres)(state, first, afterLast);
			if (afterPctSubUnres == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseOwnUserInfo)(state, afterPctSubUnres, afterLast);
		}

	case _UT(':'):
		return URI_FUNC(ParseOwnUserInfo)(state, first + 1, afterLast);

	case _UT('@'):
		/* SURE */
		state->uri->userInfo.afterLast = first; /* USERINFO END */
		state->uri->hostText.first = first + 1; /* HOST BEGIN */
		return URI_FUNC(ParseOwnHost)(state, first + 1, afterLast);

	default:
		URI_FUNC(Stop)(state, first);
		return NULL;
	}
}



static URI_INLINE void URI_FUNC(OnExitPartHelperTwo)(URI_TYPE(ParserState) * state) {
	state->uri->absolutePath = URI_TRUE;
}



/*
 * [partHelperTwo]->[pathAbsNoLeadSlash] // can take <NULL>
 * [partHelperTwo]-></>[authority][pathAbsEmpty]
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParsePartHelperTwo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(OnExitPartHelperTwo)(state);
		return afterLast;
	}

	switch (*first) {
	case _UT('/'):
		{
			const URI_CHAR * const afterAuthority
					= URI_FUNC(ParseAuthority)(state, first + 1, afterLast);
			const URI_CHAR * afterPathAbsEmpty;
			if (afterAuthority == NULL) {
				return NULL;
			}
			afterPathAbsEmpty = URI_FUNC(ParsePathAbsEmpty)(state, afterAuthority, afterLast);

			/* Fix path if only one empty segment */
			if ((state->uri->pathHead != NULL)
					&& (state->uri->pathHead->next == NULL)
					&& (state->uri->pathHead->text.first == state->uri->pathHead->text.afterLast)) {
				free(state->uri->pathHead);
				/* TODO deep copy mode? */
				state->uri->pathHead = NULL;
				state->uri->pathTail = NULL;
			}

			return afterPathAbsEmpty;
		}

	default:
		URI_FUNC(OnExitPartHelperTwo)(state);
		return URI_FUNC(ParsePathAbsNoLeadSlash)(state, first, afterLast);
	}
}



/*
 * [pathAbsEmpty]-></>[segment][pathAbsEmpty]
 * [pathAbsEmpty]-><NULL>
 */
static const URI_CHAR * URI_FUNC(ParsePathAbsEmpty)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('/'):
		{
			const URI_CHAR * const afterSegment
					= URI_FUNC(ParseSegment)(state, first + 1, afterLast);
			if (afterSegment == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(state, first + 1, afterSegment); /* SEGMENT BOTH */
			return URI_FUNC(ParsePathAbsEmpty)(state, afterSegment, afterLast);
		}


	default:
		return first;
	}
}



/*
 * [pathAbsNoLeadSlash]->[segmentNz][zeroMoreSlashSegs]
 * [pathAbsNoLeadSlash]-><NULL>
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParsePathAbsNoLeadSlash)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('%'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(':'):
	case _UT(';'):
	case _UT('@'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		{
			const URI_CHAR * const afterSegmentNz
					= URI_FUNC(ParseSegmentNz)(state, first, afterLast);
			if (afterSegmentNz == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(state, first, afterSegmentNz); /* SEGMENT BOTH */
			return URI_FUNC(ParseZeroMoreSlashSegs)(state, afterSegmentNz, afterLast);
		}

	default:
		return first;
	}
}



/*
 * [pathRootless]->[segmentNz][zeroMoreSlashSegs]
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParsePathRootless)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	const URI_CHAR * const afterSegmentNz
			= URI_FUNC(ParseSegmentNz)(state, first, afterLast);
	if (afterSegmentNz == NULL) {
		return NULL;
	} else {
		URI_FUNC(PushPathSegment)(state, first, afterSegmentNz); /* SEGMENT BOTH */
	}
	return URI_FUNC(ParseZeroMoreSlashSegs)(state, afterSegmentNz, afterLast);
}



/*
 * [pchar]->[pctEncoded]
 * [pchar]->[subDelims]
 * [pchar]->[unreserved]
 * [pchar]-><:>
 * [pchar]-><@>
 */
static const URI_CHAR * URI_FUNC(ParsePchar)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(Stop)(state, first);
		return NULL;
	}

	switch (*first) {
	case _UT('%'):
		return URI_FUNC(ParsePctEncoded)(state, first, afterLast);

	case _UT(':'):
	case _UT('@'):
	case _UT('!'):
	case _UT('$'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('*'):
	case _UT(','):
	case _UT(';'):
	case _UT('\''):
	case _UT('+'):
	case _UT('='):
	case _UT('-'):
	case _UT('.'):
	case _UT('_'):
	case _UT('~'):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		return first + 1;

	default:
		URI_FUNC(Stop)(state, first);
		return NULL;
	}
}



/*
 * [pctEncoded]-><%>[HEXDIG][HEXDIG]
 */
static const URI_CHAR * URI_FUNC(ParsePctEncoded)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(Stop)(state, first);
		return NULL;
	}

	/*
	First character has already been
	checked before entering this rule.

	switch (*first) {
	case _UT('%'):
	*/
		if (first + 1 >= afterLast) {
			URI_FUNC(Stop)(state, first + 1);
			return NULL;
		}

		switch (first[1]) {
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
		case _UT('a'):
		case _UT('A'):
		case _UT('b'):
		case _UT('B'):
		case _UT('c'):
		case _UT('C'):
		case _UT('d'):
		case _UT('D'):
		case _UT('e'):
		case _UT('E'):
		case _UT('f'):
		case _UT('F'):
			if (first + 2 >= afterLast) {
				URI_FUNC(Stop)(state, first + 2);
				return NULL;
			}

			switch (first[2]) {
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
			case _UT('a'):
			case _UT('A'):
			case _UT('b'):
			case _UT('B'):
			case _UT('c'):
			case _UT('C'):
			case _UT('d'):
			case _UT('D'):
			case _UT('e'):
			case _UT('E'):
			case _UT('f'):
			case _UT('F'):
				return first + 3;

			default:
				URI_FUNC(Stop)(state, first + 2);
				return NULL;
			}

		default:
			URI_FUNC(Stop)(state, first + 1);
			return NULL;
		}

	/*
	default:
		URI_FUNC(Stop)(state, first);
		return NULL;
	}
	*/
}



/*
 * [pctSubUnres]->[pctEncoded]
 * [pctSubUnres]->[subDelims]
 * [pctSubUnres]->[unreserved]
 */
static const URI_CHAR * URI_FUNC(ParsePctSubUnres)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(Stop)(state, first);
		return NULL;
	}

	switch (*first) {
	case _UT('%'):
		return URI_FUNC(ParsePctEncoded)(state, first, afterLast);

	case _UT('!'):
	case _UT('$'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('*'):
	case _UT(','):
	case _UT(';'):
	case _UT('\''):
	case _UT('+'):
	case _UT('='):
	case _UT('-'):
	case _UT('.'):
	case _UT('_'):
	case _UT('~'):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		return first + 1;

	default:
		URI_FUNC(Stop)(state, first);
		return NULL;
	}
}



/*
 * [port]->[DIGIT][port]
 * [port]-><NULL>
 */
static const URI_CHAR * URI_FUNC(ParsePort)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
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
		return URI_FUNC(ParsePort)(state, first + 1, afterLast);

	default:
		return first;
	}
}



/*
 * [queryFrag]->[pchar][queryFrag]
 * [queryFrag]-></>[queryFrag]
 * [queryFrag]-><?>[queryFrag]
 * [queryFrag]-><NULL>
 */
static const URI_CHAR * URI_FUNC(ParseQueryFrag)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('%'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(':'):
	case _UT(';'):
	case _UT('@'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		{
			const URI_CHAR * const afterPchar
					= URI_FUNC(ParsePchar)(state, first, afterLast);
			if (afterPchar == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseQueryFrag)(state, afterPchar, afterLast);
		}

	case _UT('/'):
	case _UT('?'):
		return URI_FUNC(ParseQueryFrag)(state, first + 1, afterLast);

	default:
		return first;
	}
}



/*
 * [segment]->[pchar][segment]
 * [segment]-><NULL>
 */
static const URI_CHAR * URI_FUNC(ParseSegment)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('!'):
	case _UT('$'):
	case _UT('%'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('-'):
	case _UT('*'):
	case _UT(','):
	case _UT('.'):
	case _UT(':'):
	case _UT(';'):
	case _UT('@'):
	case _UT('\''):
	case _UT('_'):
	case _UT('~'):
	case _UT('+'):
	case _UT('='):
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
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		{
			const URI_CHAR * const afterPchar
					= URI_FUNC(ParsePchar)(state, first, afterLast);
			if (afterPchar == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseSegment)(state, afterPchar, afterLast);
		}

	default:
		return first;
	}
}



/*
 * [segmentNz]->[pchar][segment]
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParseSegmentNz)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	const URI_CHAR * const afterPchar
			= URI_FUNC(ParsePchar)(state, first, afterLast);
	if (afterPchar == NULL) {
		return NULL;
	}
	return URI_FUNC(ParseSegment)(state, afterPchar, afterLast);
}



static URI_INLINE void URI_FUNC(OnExitSegmentNzNcOrScheme2)(URI_TYPE(ParserState) * state, const URI_CHAR * first) {
	URI_FUNC(PushPathSegment)(state, state->uri->scheme.first, first); /* SEGMENT BOTH */
	state->uri->scheme.first = NULL; /* Not a scheme, reset */
}



/*
 * [segmentNzNcOrScheme2]->[ALPHA][segmentNzNcOrScheme2]
 * [segmentNzNcOrScheme2]->[DIGIT][segmentNzNcOrScheme2]
 * [segmentNzNcOrScheme2]->[pctEncoded][mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]->[uriTail] // can take <NULL>
 * [segmentNzNcOrScheme2]-><!>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><$>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><&>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><(>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><)>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><*>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><,>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><.>[segmentNzNcOrScheme2]
 * [segmentNzNcOrScheme2]-></>[segment][zeroMoreSlashSegs][uriTail]
 * [segmentNzNcOrScheme2]-><:>[hierPart][uriTail]
 * [segmentNzNcOrScheme2]-><;>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><@>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><_>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><~>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><+>[segmentNzNcOrScheme2]
 * [segmentNzNcOrScheme2]-><=>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><'>[mustBeSegmentNzNc]
 * [segmentNzNcOrScheme2]-><->[segmentNzNcOrScheme2]
 */
static const URI_CHAR * URI_FUNC(ParseSegmentNzNcOrScheme2)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		URI_FUNC(OnExitSegmentNzNcOrScheme2)(state, first);
		return afterLast;
	}

	switch (*first) {
	case _UT('.'):
	case _UT('+'):
	case _UT('-'):
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
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
		return URI_FUNC(ParseSegmentNzNcOrScheme2)(state, first + 1, afterLast);

	case _UT('%'):
		{
			const URI_CHAR * const afterPctEncoded
					= URI_FUNC(ParsePctEncoded)(state, first, afterLast);
			if (afterPctEncoded == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseMustBeSegmentNzNc)(state, afterPctEncoded, afterLast);
		}

	case _UT('!'):
	case _UT('$'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('*'):
	case _UT(','):
	case _UT(';'):
	case _UT('@'):
	case _UT('_'):
	case _UT('~'):
	case _UT('='):
	case _UT('\''):
		return URI_FUNC(ParseMustBeSegmentNzNc)(state, first + 1, afterLast);

	case _UT('/'):
		{
			const URI_CHAR * afterZeroMoreSlashSegs;
			const URI_CHAR * const afterSegment
					= URI_FUNC(ParseSegment)(state, first + 1, afterLast);
			if (afterSegment == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(state, state->uri->scheme.first, first); /* SEGMENT BOTH */
			state->uri->scheme.first = NULL; /* Not a scheme, reset */
			URI_FUNC(PushPathSegment)(state, first + 1, afterSegment); /* SEGMENT BOTH */
			afterZeroMoreSlashSegs
					= URI_FUNC(ParseZeroMoreSlashSegs)(state, afterSegment, afterLast);
			if (afterZeroMoreSlashSegs == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseUriTail)(state, afterZeroMoreSlashSegs, afterLast);
		}

	case _UT(':'):
		{
			const URI_CHAR * const afterHierPart
					= URI_FUNC(ParseHierPart)(state, first + 1, afterLast);
			state->uri->scheme.afterLast = first; /* SCHEME END */
			if (afterHierPart == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseUriTail)(state, afterHierPart, afterLast);
		}

	default:
		URI_FUNC(OnExitSegmentNzNcOrScheme2)(state, first);
		return URI_FUNC(ParseUriTail)(state, first, afterLast);
	}
}



/*
 * [uriReference]->[ALPHA][segmentNzNcOrScheme2]
 * [uriReference]->[DIGIT][mustBeSegmentNzNc]
 * [uriReference]->[pctEncoded][mustBeSegmentNzNc]
 * [uriReference]->[subDelims][mustBeSegmentNzNc]
 * [uriReference]->[uriTail] // can take <NULL>
 * [uriReference]-><.>[mustBeSegmentNzNc]
 * [uriReference]-></>[partHelperTwo][uriTail]
 * [uriReference]-><@>[mustBeSegmentNzNc]
 * [uriReference]-><_>[mustBeSegmentNzNc]
 * [uriReference]-><~>[mustBeSegmentNzNc]
 * [uriReference]-><->[mustBeSegmentNzNc]
 */
static const URI_CHAR * URI_FUNC(ParseUriReference)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('a'):
	case _UT('A'):
	case _UT('b'):
	case _UT('B'):
	case _UT('c'):
	case _UT('C'):
	case _UT('d'):
	case _UT('D'):
	case _UT('e'):
	case _UT('E'):
	case _UT('f'):
	case _UT('F'):
	case _UT('g'):
	case _UT('G'):
	case _UT('h'):
	case _UT('H'):
	case _UT('i'):
	case _UT('I'):
	case _UT('j'):
	case _UT('J'):
	case _UT('k'):
	case _UT('K'):
	case _UT('l'):
	case _UT('L'):
	case _UT('m'):
	case _UT('M'):
	case _UT('n'):
	case _UT('N'):
	case _UT('o'):
	case _UT('O'):
	case _UT('p'):
	case _UT('P'):
	case _UT('q'):
	case _UT('Q'):
	case _UT('r'):
	case _UT('R'):
	case _UT('s'):
	case _UT('S'):
	case _UT('t'):
	case _UT('T'):
	case _UT('u'):
	case _UT('U'):
	case _UT('v'):
	case _UT('V'):
	case _UT('w'):
	case _UT('W'):
	case _UT('x'):
	case _UT('X'):
	case _UT('y'):
	case _UT('Y'):
	case _UT('z'):
	case _UT('Z'):
		state->uri->scheme.first = first; /* SCHEME BEGIN */
		return URI_FUNC(ParseSegmentNzNcOrScheme2)(state, first + 1, afterLast);

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
	case _UT('!'):
	case _UT('$'):
	case _UT('&'):
	case _UT('('):
	case _UT(')'):
	case _UT('*'):
	case _UT(','):
	case _UT(';'):
	case _UT('\''):
	case _UT('+'):
	case _UT('='):
	case _UT('.'):
	case _UT('_'):
	case _UT('~'):
	case _UT('-'):
	case _UT('@'):
		state->uri->scheme.first = first; /* SCHEME BEGIN */
		return URI_FUNC(ParseMustBeSegmentNzNc)(state, first + 1, afterLast);

	case _UT('%'):
		{
			const URI_CHAR * const afterPctEncoded
					= URI_FUNC(ParsePctEncoded)(state, first, afterLast);
			if (afterPctEncoded == NULL) {
				return NULL;
			}
			state->uri->scheme.first = first; /* SCHEME BEGIN */
			return URI_FUNC(ParseMustBeSegmentNzNc)(state, afterPctEncoded, afterLast);
		}

	case _UT('/'):
		{
			const URI_CHAR * const afterPartHelperTwo
					= URI_FUNC(ParsePartHelperTwo)(state, first + 1, afterLast);
			if (afterPartHelperTwo == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseUriTail)(state, afterPartHelperTwo, afterLast);
		}

	default:
		return URI_FUNC(ParseUriTail)(state, first, afterLast);
	}
}



/*
 * [uriTail]-><#>[queryFrag]
 * [uriTail]-><?>[queryFrag][uriTailTwo]
 * [uriTail]-><NULL>
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParseUriTail)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('#'):
		{
			const URI_CHAR * const afterQueryFrag = URI_FUNC(ParseQueryFrag)(state, first + 1, afterLast);
			if (afterQueryFrag == NULL) {
				return NULL;
			}
			state->uri->fragment.first = first + 1; /* FRAGMENT BEGIN */
			state->uri->fragment.afterLast = afterQueryFrag; /* FRAGMENT END */
			return afterQueryFrag;
		}

	case _UT('?'):
		{
			const URI_CHAR * const afterQueryFrag
					= URI_FUNC(ParseQueryFrag)(state, first + 1, afterLast);
			if (afterQueryFrag == NULL) {
				return NULL;
			}
			state->uri->query.first = first + 1; /* QUERY BEGIN */
			state->uri->query.afterLast = afterQueryFrag; /* QUERY END */
			return URI_FUNC(ParseUriTailTwo)(state, afterQueryFrag, afterLast);
		}

	default:
		return first;
	}
}



/*
 * [uriTailTwo]-><#>[queryFrag]
 * [uriTailTwo]-><NULL>
 */
static URI_INLINE const URI_CHAR * URI_FUNC(ParseUriTailTwo)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('#'):
		{
			const URI_CHAR * const afterQueryFrag = URI_FUNC(ParseQueryFrag)(state, first + 1, afterLast);
			if (afterQueryFrag == NULL) {
				return NULL;
			}
			state->uri->fragment.first = first + 1; /* FRAGMENT BEGIN */
			state->uri->fragment.afterLast = afterQueryFrag; /* FRAGMENT END */
			return afterQueryFrag;
		}

	default:
		return first;
	}
}



/*
 * [zeroMoreSlashSegs]-></>[segment][zeroMoreSlashSegs]
 * [zeroMoreSlashSegs]-><NULL>
 */
static const URI_CHAR * URI_FUNC(ParseZeroMoreSlashSegs)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('/'):
		{
			const URI_CHAR * const afterSegment
					= URI_FUNC(ParseSegment)(state, first + 1, afterLast);
			if (afterSegment == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(state, first + 1, afterSegment); /* SEGMENT BOTH */
			return URI_FUNC(ParseZeroMoreSlashSegs)(state, afterSegment, afterLast);
		}

	default:
		return first;
	}
}



static URI_INLINE void URI_FUNC(ResetParserState)(URI_TYPE(ParserState) * state) {
	URI_TYPE(Uri) * const uriBackup = state->uri;
	memset(state, 0, sizeof(URI_TYPE(ParserState)));
	state->uri = uriBackup;
}



static URI_INLINE void URI_FUNC(ResetUri)(URI_TYPE(Uri) * uri) {
	memset(uri, 0, sizeof(URI_TYPE(Uri)));
}



static URI_INLINE void URI_FUNC(PushPathSegment)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	URI_TYPE(PathSegment) * segment = malloc(1 * sizeof(URI_TYPE(PathSegment)));
	/* TODO NULL check */
	memset(segment, 0, sizeof(URI_TYPE(PathSegment)));
	segment->text.first = first;
	segment->text.afterLast = afterLast;

	if (state->uri->pathHead == NULL) {
		state->uri->pathHead = segment;
		state->uri->pathTail = segment;
	} else {
		state->uri->pathTail->next = segment;
		state->uri->pathTail = segment;
	}
}



int URI_FUNC(ParseUriEx)(URI_TYPE(ParserState) * state, const URI_CHAR * first, const URI_CHAR * afterLast) {
	const URI_CHAR * afterUriReference;
	URI_TYPE(Uri) * uri;

	/* Check params */
	if ((state == NULL) || (state == NULL) || (state == NULL)) {
		return URI_ERROR_NULL;
	}
	uri = state->uri;

	/* Init parser */
	URI_FUNC(ResetParserState)(state);
	URI_FUNC(ResetUri)(uri);

	/* Parse */
	afterUriReference = URI_FUNC(ParseUriReference)(state, first, afterLast);
	if (afterUriReference == NULL) {
		return state->errorCode;
	}
	if (afterUriReference != afterLast) {
		return URI_ERROR_SYNTAX;
	}
	return URI_SUCCESS;
}



int URI_FUNC(ParseUri)(URI_TYPE(ParserState) * state, const URI_CHAR * text) {
	if ((state == NULL) || (text == NULL)) {
		return URI_ERROR_NULL;
	}
	return URI_FUNC(ParseUriEx)(state, text, text + URI_STRLEN(text));
}



void URI_FUNC(FreeUriMembers)(URI_TYPE(Uri) * uri) {
	if (uri == NULL) {
		return;
	}
	if (uri->hostData.ip4 != NULL) {
		free(uri->hostData.ip4);
		uri->hostData.ip4 = NULL;
	}
	if (uri->hostData.ip6 != NULL) {
		free(uri->hostData.ip6);
		uri->hostData.ip6 = NULL;
	}
	if (uri->pathHead != NULL) {
		URI_TYPE(PathSegment) * segWalk = uri->pathHead;
		while (segWalk != NULL) {
			URI_TYPE(PathSegment) * const next = segWalk->next;
			free(segWalk);
			segWalk = next;
		}
		uri->pathHead = NULL;
		uri->pathTail = NULL;
	}
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



const URI_CHAR * URI_FUNC(UnescapeInPlace)(URI_CHAR * inout) {
	URI_CHAR * read = inout;
	URI_CHAR * write = inout;

	if (inout == NULL) {
		return NULL;
	}

	for (;;) {
		switch (read[0]) {
		case _UT('\0'):
			if (read > write) {
				write[0] = _UT('\0');
			}
			return write;

		case _UT('%'):
			switch (read[1]) {
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
			case _UT('a'):
			case _UT('b'):
			case _UT('c'):
			case _UT('d'):
			case _UT('e'):
			case _UT('f'):
			case _UT('A'):
			case _UT('B'):
			case _UT('C'):
			case _UT('D'):
			case _UT('E'):
			case _UT('F'):
				switch (read[2]) {
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
				case _UT('a'):
				case _UT('b'):
				case _UT('c'):
				case _UT('d'):
				case _UT('e'):
				case _UT('f'):
				case _UT('A'):
				case _UT('B'):
				case _UT('C'):
				case _UT('D'):
				case _UT('E'):
				case _UT('F'):
					{
						/* Percent group found, make single char of that */
						const unsigned char left = URI_FUNC(HexdigToInt)(read[1]);
						const unsigned char right = URI_FUNC(HexdigToInt)(read[2]);
						write[0] = (URI_CHAR)(16 * left + right);
						read += 3;
						write++;
					}
					break;

				default:
					/* Copy two chars unmodified and */
					/* look at this char again */
					if (read > write) {
						write[0] = read[0];
						write[1] = read[1];
					}
					read += 2;
					write += 2;
				}
				break;

			default:
				/* Copy one char unmodified and */
				/* look at this char again */
				if (read > write) {
					write[0] = read[0];
				}
				read++;
				write++;
			}
			break;

		default:
			/* Copy one char unmodified */
			if (read > write) {
				write[0] = read[0];
			}
			read++;
			write++;
		}
	}
}



/* TODO shallow or deep copy? */
static void URI_FUNC(CopyTextRange)(URI_TYPE(TextRange) * dest,
		const URI_TYPE(TextRange) * source) {
	int charsToCopy;
	int bytesToCopy;
	URI_CHAR * dup;
	if ((source == NULL) || (dest == NULL)) {
		return;
	}
	charsToCopy = (source->afterLast - source->first);
	bytesToCopy = charsToCopy * sizeof(URI_CHAR);
	dup = malloc(bytesToCopy);
	/* TODO NULL check */
	memcpy(dup, source->first, bytesToCopy);
	dest->first = dup;
	dest->afterLast = dup + charsToCopy;
}



/* TODO */
static UriBool URI_FUNC(IsHostSet)(const URI_TYPE(Uri) * uri) {
	return (uri != NULL)
			&& ((uri->hostText.first != NULL)
				|| (uri->hostData.ip4 != NULL)
				|| (uri->hostData.ip6 != NULL)
				|| (uri->hostData.ipFuture.first != NULL)
			);
}

/* TODO */
static void URI_FUNC(CopyPath)(URI_TYPE(Uri) * dest,
		const URI_TYPE(Uri) * source) {
	if (source->pathHead == NULL) {
		/* No path component */
		dest->pathHead = NULL;
		dest->pathTail = NULL;
	} else {
		/* Copy list but not the text contained */
		URI_TYPE(PathSegment) * sourceWalker = source->pathHead;
		URI_TYPE(PathSegment) * destPrev = NULL;
		do {
			URI_TYPE(PathSegment) * cur = malloc(sizeof(URI_TYPE(PathSegment)));
			/* TODO NULL check */
			cur->text = sourceWalker->text;
			if (destPrev == NULL) {
				/* First segment ever */
				dest->pathHead = cur;
			} else {
				destPrev->next = cur;
			}
			destPrev = cur;
			sourceWalker = sourceWalker->next;
		} while (sourceWalker != NULL);
		dest->pathTail = destPrev;
		dest->pathTail->next = NULL;
	}
}

/* TODO */
static const URI_CHAR URI_FUNC(SafeToPointTo);

/* TODO */
static void URI_FUNC(RemoveDotSegments)(URI_TYPE(Uri) * uri) {
	URI_TYPE(PathSegment) * walker;
	if ((uri == NULL) || (uri->pathHead == NULL)) {
		return;
	}

	walker = uri->pathHead;
	walker->reserved = NULL; /* Prev pointer */
	do {
		int len = (walker->text.afterLast - walker->text.first);
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
					/* TODO NULL check */
					memset(segment, 0, sizeof(URI_TYPE(PathSegment)));
					segment->text.first = URI_FUNC(SafeToPointTo);
					segment->text.afterLast = URI_FUNC(SafeToPointTo);
					prev->next = segment;
					uri->pathTail = segment;
				}
				free(walker); /* TODO Free text in deep copy mode */
				walker = nextBackup;
			} else {
				if (walker->next != NULL) {
					walker->next->reserved = walker;
				} else {
					/* Last segment -> update tail */
					URI_TYPE(PathSegment) * const prev = walker->reserved;
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
							/* TODO NULL check */
							memset(segment, 0, sizeof(URI_TYPE(PathSegment)));
							segment->text.first = URI_FUNC(SafeToPointTo);
							segment->text.afterLast = URI_FUNC(SafeToPointTo);
							prevPrev->next = segment;
							uri->pathTail = segment;
						}
						free(walker); /* TODO Free text in deep copy mode */
						free(prev); /* TODO Free text in deep copy mode */
						walker = nextBackup;
					} else {
						/* Prev is the first segment */
						uri->pathHead = walker->next;
						/* TODO tail? */
						if (walker->next != NULL) {
							walker->next->reserved = NULL;
						} else {
							/* Last segment -> update tail */
							uri->pathTail = NULL;
						}
						free(walker); /* TODO Free text in deep copy mode */
						free(prev); /* TODO Free text in deep copy mode */
						walker = nextBackup;
					}
				} else {
					URI_TYPE(PathSegment) * const nextBackup = walker->next;
					/* First segment -> update head pointer */
					uri->pathHead = walker->next;
					/* TODO tail? */
					if (walker->next != NULL) {
						walker->next->reserved = NULL;
					} else {
						/* Last segment -> update tail */
						uri->pathTail = NULL;
					}
					free(walker); /* TODO Free text in deep copy mode */
					walker = nextBackup;
				}
			} else {
				if (walker->next != NULL) {
					walker->next->reserved = walker;
				} else {
					/* Last segment -> update tail */
					URI_TYPE(PathSegment) * const prev = walker->reserved;
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
				URI_TYPE(PathSegment) * const prev = walker->reserved;
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
		/* TODO deep copy mode? */
		uri->pathHead = NULL;
		uri->pathTail = NULL;
	}
}

/* TODO */
static void URI_FUNC(CopyAuthority)(URI_TYPE(Uri) * dest,
		const URI_TYPE(Uri) * source) {
	/* TODO shallow or deep? */

	/* Copy userInfo */
	dest->userInfo = source->userInfo;

	/* Copy hostText */
	dest->hostText = source->hostText;

	/* Copy hostData */
	if (source->hostData.ip4 != NULL) {
		dest->hostData.ip4 = malloc(sizeof(UriIp4));
		/* TODO NULL check */
		*(dest->hostData.ip4) = *(source->hostData.ip4);
		dest->hostData.ip6 = NULL;
		dest->hostData.ipFuture.first = NULL;
		dest->hostData.ipFuture.afterLast = NULL;
	} else if (source->hostData.ip6 != NULL) {
		dest->hostData.ip4 = NULL;
		dest->hostData.ip6 = malloc(sizeof(UriIp6));
		/* TODO NULL check */
		*(dest->hostData.ip6) = *(source->hostData.ip6);
		dest->hostData.ipFuture.first = NULL;
		dest->hostData.ipFuture.afterLast = NULL;
	} else {
		dest->hostData.ip4 = NULL;
		dest->hostData.ip6 = NULL;
		dest->hostData.ipFuture = source->hostData.ipFuture;
	}

	/* Copy portText */
	dest->portText = source->portText;
}

/* static const URI_CHAR * const URI_FUNC(Dot) = _UT("."); */

/* TODO */
static void URI_FUNC(MergePath)(URI_TYPE(Uri) * absWork,
		const URI_TYPE(Uri) * relAppend) {
	URI_TYPE(PathSegment) * sourceWalker;
	URI_TYPE(PathSegment) * destPrev;
	if ((absWork->pathHead == NULL) || (relAppend->pathHead == NULL)) {
		return;
	}

	/* Replace last segment ("" if trailing slash) with first of append chain */
	absWork->pathTail->text.first = relAppend->pathHead->text.first;
	absWork->pathTail->text.afterLast = relAppend->pathHead->text.afterLast;

	sourceWalker = relAppend->pathHead->next;
	if (sourceWalker == NULL) {
		return;
	}
	destPrev = absWork->pathTail;

	for (;;) {
		URI_TYPE(PathSegment) * const dup = malloc(sizeof(URI_TYPE(PathSegment)));
		/* TODO NULL check */
		dup->text = sourceWalker->text;
		destPrev->next = dup;

		if (sourceWalker->next == NULL) {
			absWork->pathTail = dup;
			absWork->pathTail->next = NULL;
			break;
		}
		destPrev = dup;
		sourceWalker = sourceWalker->next;
	}
}



/* TODO */
int URI_FUNC(AddBase)(URI_TYPE(Uri) * absDest,
		const URI_TYPE(Uri) * relSource,
		const URI_TYPE(Uri) * absBase) {
	if ((absDest == NULL) || (relSource == NULL) || (absBase == NULL)) {
		return 1; /* TODO */
	}
	URI_FUNC(ResetUri)(absDest);

	/*
	TODO
	relSource relative
	absBase absolute

	SHALLOW wherever possible!
	*/

	/* [01/32]	if defined(R.scheme) then */
				if (relSource->scheme.first != NULL) {
	/* [02/32]		T.scheme = R.scheme; */
					absDest->scheme = relSource->scheme;
	/* [03/32]		T.authority = R.authority; */
					URI_FUNC(CopyAuthority)(absDest, relSource);
	/* [04/32]		T.path = remove_dot_segments(R.path); */
					URI_FUNC(CopyPath)(absDest, relSource);
					URI_FUNC(RemoveDotSegments)(absDest);
	/* [05/32]		T.query = R.query; */
					absDest->query = relSource->query;
	/* [06/32]	else */
				} else {
	/* [07/32]		if defined(R.authority) then */
					if (URI_FUNC(IsHostSet)(relSource)) {
	/* [08/32]			T.authority = R.authority; */
						URI_FUNC(CopyAuthority)(absDest, relSource);
	/* [09/32]			T.path = remove_dot_segments(R.path); */
						URI_FUNC(CopyPath)(absDest, relSource);
						URI_FUNC(RemoveDotSegments)(absDest);
	/* [10/32]			T.query = R.query; */
						absDest->query = relSource->query;
	/* [11/32]		else */
					} else {
	/* [12/32]			if (R.path == "") then */
						if (relSource->pathHead == NULL) {
	/* [13/32]				T.path = Base.path; */
							URI_FUNC(CopyPath)(absDest, absBase);
	/* [14/32]				if defined(R.query) then */
							if (relSource->query.first != NULL) {
	/* [15/32]					T.query = R.query; */
								absDest->query = relSource->query;
	/* [16/32]				else */
							} else {
	/* [17/32]					T.query = Base.query; */
								absDest->query = absBase->query;
	/* [18/32]				endif; */
							}
	/* [19/32]			else */
						} else {
	/* [20/32]				if (R.path starts-with "/") then */
							if (relSource->absolutePath) {
	/* [21/32]					T.path = remove_dot_segments(R.path); */
								URI_FUNC(CopyPath)(absDest, relSource);
								URI_FUNC(RemoveDotSegments)(absDest);
	/* [22/32]				else */
							} else {
	/* [23/32]					T.path = merge(Base.path, R.path); */
								URI_FUNC(CopyPath)(absDest, absBase);
								URI_FUNC(MergePath)(absDest, relSource);
	/* [24/32]					T.path = remove_dot_segments(T.path); */
								URI_FUNC(RemoveDotSegments)(absDest);
	/* [25/32]				endif; */
							}
	/* [26/32]				T.query = R.query; */
							absDest->query = relSource->query;
	/* [27/32]			endif; */
						}
	/* [28/32]			T.authority = Base.authority; */
					URI_FUNC(CopyAuthority)(absDest, absBase);
	/* [29/32]		endif; */
					}
	/* [30/32]		T.scheme = Base.scheme; */
					absDest->scheme = absBase->scheme;
	/* [31/32]	endif; */
				}
	/* [32/32]	T.fragment = R.fragment; */
				absDest->fragment = relSource->fragment;

	return 0; /* TODO */
}


/* TODO */
static int URI_FUNC(CompareRange)(const URI_TYPE(TextRange) * a,
		const URI_TYPE(TextRange) * b) {
	int diff;

	/* NOTE: Both NULL means equal! */
	if ((a == NULL) || (b == NULL)) {
		return ((a == NULL) && (b == NULL)) ? URI_TRUE : URI_FALSE;
	}

	diff = ((a->afterLast - a->first) - (b->afterLast - b->first));
	if (diff > 0) {
		return 1;
	} else if (diff < 0) {
		return -1;
	}

	return URI_STRNCMP(a->first, b->first, (a->afterLast - a->first));
}


/* TODO */
UriBool URI_FUNC(Equals)(URI_TYPE(Uri) * a,
		const URI_TYPE(Uri) * b) {
	/* NOTE: Both NULL means equal! */
	if ((a == NULL) || (b == NULL)) {
		return ((a == NULL) && (b == NULL)) ? URI_TRUE : URI_FALSE;
	}

	/* scheme */
	if (URI_FUNC(CompareRange)(&(a->scheme), &(b->scheme))) {
		return URI_FALSE;
	}

	/* absolutePath */
	if ((a->scheme.first == NULL)&& (a->absolutePath != b->absolutePath)) {
		return URI_FALSE;
	}

	/* userInfo */
	if (URI_FUNC(CompareRange)(&(a->userInfo), &(b->userInfo))) {
		return URI_FALSE;
	}

	/* Host */
	if (((a->hostData.ip4 == NULL) != (b->hostData.ip4 == NULL))
			|| ((a->hostData.ip6 == NULL) != (b->hostData.ip6 == NULL))
			|| ((a->hostData.ipFuture.first == NULL)
				!= (b->hostData.ipFuture.first == NULL))) {
		return URI_FALSE;
	}
	if (a->hostData.ip4 != NULL) {
		if (memcmp(a->hostData.ip4->data, b->hostData.ip4->data, 4)) {
			return URI_FALSE;
		}
	}
	if (a->hostData.ip6 != NULL) {
		if (memcmp(a->hostData.ip6->data, b->hostData.ip6->data, 16)) {
			return URI_FALSE;
		}
	}
	if (a->hostData.ipFuture.first != NULL) {
		if (URI_FUNC(CompareRange)(&(a->hostData.ipFuture), &(b->hostData.ipFuture))) {
			return URI_FALSE;
		}
	}
	if ((a->hostData.ip4 == NULL)
			&& (a->hostData.ip6 == NULL)
			&& (a->hostData.ipFuture.first == NULL)) {
		if (URI_FUNC(CompareRange)(&(a->hostText), &(b->hostText))) {
			return URI_FALSE;
		}
	}

	/* portText */
	if (URI_FUNC(CompareRange)(&(a->portText), &(b->portText))) {
		return URI_FALSE;
	}

	/* Path */
	if ((a->pathHead == NULL) != (b->pathHead == NULL)) {
		return URI_FALSE;
	}
	if (a->pathHead != NULL) {
		URI_TYPE(PathSegment) * walkA = a->pathHead;
		URI_TYPE(PathSegment) * walkB = b->pathHead;
		do {
			if (URI_FUNC(CompareRange)(&(walkA->text), &(walkB->text))) {
				return URI_FALSE;
			}
			if ((walkA->next == NULL) != (walkB->next == NULL)) {
				return URI_FALSE;
			}
			walkA = walkA->next;
			walkB = walkB->next;
		} while (walkA != NULL);
	}

	/* query */
	if (URI_FUNC(CompareRange)(&(a->query), &(b->query))) {
		return URI_FALSE;
	}

	/* fragment */
	if (URI_FUNC(CompareRange)(&(a->fragment), &(b->fragment))) {
		return URI_FALSE;
	}

	return URI_TRUE; /* Equal*/
}



int URI_FUNC(ToStringCharsRequired)(const URI_TYPE(Uri) * uri) {
	return 0; /* TODO */
}

int URI_FUNC(ToString)(URI_CHAR * dest, const URI_TYPE(Uri) * uri, int maxChars) {
	int charsWritten = 0;
	if ((dest == NULL) || (uri == NULL) || (maxChars < 1)) {
		return 0;
	}
	maxChars--; /* So we don't have to substract 1 for '\0' all the time */

	/* [01/19]	result = "" */
				dest[0] = _UT('\0');
	/* [02/19]	if defined(scheme) then */
				if (uri->scheme.first != NULL) {
	/* [03/19]		append scheme to result; */
					const int charsToWrite
							= uri->scheme.afterLast - uri->scheme.first;
					if (charsWritten + charsToWrite < maxChars) {
						memcpy(dest + charsWritten, uri->scheme.first,
								charsToWrite * sizeof(URI_CHAR));
						charsWritten += charsToWrite;
					}
	/* [04/19]		append ":" to result; */
					if (charsWritten + 1 < maxChars) {
						memcpy(dest + charsWritten, _UT(":"),
								1 * sizeof(URI_CHAR));
						charsWritten += 1;
					}
	/* [05/19]	endif; */
				}
	/* [06/19]	if defined(authority) then */
				if (URI_FUNC(IsHostSet)(uri)) {
	/* [07/19]		append "//" to result; */
					if (charsWritten + 2 < maxChars) {
						memcpy(dest + charsWritten, _UT("//"),
								2 * sizeof(URI_CHAR));
						charsWritten += 2;
					}
	/* [08/19]		append authority to result; */
					/* UserInfo */
					if (uri->userInfo.first != NULL) {
						const int charsToWrite = (uri->userInfo.afterLast - uri->userInfo.first);
						if (charsWritten + charsToWrite < maxChars) {
							memcpy(dest + charsWritten, uri->userInfo.first,
									charsToWrite * sizeof(URI_CHAR));
							charsWritten += charsToWrite;
						}

						if (charsWritten + 1 < maxChars) {
							memcpy(dest + charsWritten, _UT("@"),
									1 * sizeof(URI_CHAR));
							charsWritten += 1;
						}
					}

					/* Host */
					if (uri->hostData.ip4 != NULL) {
						/* IPv4 */
						int i = 0;
						for (; i < 4; i++) {
							const unsigned char value = uri->hostData.ip4->data[i];
							const int charsToWrite = (value > 99) ? 3 : ((value > 9) ? 2 : 1);
							if (charsWritten + charsToWrite < maxChars) {
								URI_CHAR text[4];
								URI_ITOA(value, text, 10);
								memcpy(dest + charsWritten, text, charsToWrite * sizeof(URI_CHAR));
								charsWritten += charsToWrite;
							}
							if (i < 3) {
								if (charsWritten + 1 < maxChars) {
									memcpy(dest + charsWritten, _UT("."),
											1 * sizeof(URI_CHAR));
									charsWritten += 1;
								}
							}
						}
					} else if (uri->hostData.ip6 != NULL) {
						/* IPv6 */
						int i = 0;
						if (charsWritten + 1 < maxChars) {
							memcpy(dest + charsWritten, _UT("["),
									1 * sizeof(URI_CHAR));
							charsWritten += 1;
						}
						for (; i < 16; i++) {
							const unsigned char value = uri->hostData.ip6->data[i];
							if (charsWritten + 2 < maxChars) {
								URI_CHAR text[3];
								URI_ITOA(value, text, 16);
								memcpy(dest + charsWritten, text, 2 * sizeof(URI_CHAR));
								charsWritten += 2;
							}
							if (((i & 1) == 1) && (i < 15)) {
								if (charsWritten + 1 < maxChars) {
									memcpy(dest + charsWritten, _UT(":"),
											1 * sizeof(URI_CHAR));
									charsWritten += 1;
								}
							}
						}
						if (charsWritten + 1 < maxChars) {
							memcpy(dest + charsWritten, _UT("]"),
									1 * sizeof(URI_CHAR));
							charsWritten += 1;
						}
					} else if (uri->hostData.ipFuture.first != NULL) {
						/* IPvFuture */
						const int charsToWrite = (uri->hostData.ipFuture.afterLast
								- uri->hostData.ipFuture.first);
						if (charsWritten + 1 < maxChars) {
							memcpy(dest + charsWritten, _UT("["),
									1 * sizeof(URI_CHAR));
							charsWritten += 1;
						}
						if (charsWritten + charsToWrite < maxChars) {
							memcpy(dest + charsWritten, uri->hostData.ipFuture.first, charsToWrite * sizeof(URI_CHAR));
							charsWritten += charsToWrite;
						}
						if (charsWritten + 1 < maxChars) {
							memcpy(dest + charsWritten, _UT("]"),
									1 * sizeof(URI_CHAR));
							charsWritten += 1;
						}
					} else if (uri->hostText.first != NULL) {
						/* Regname */
						const int charsToWrite = (uri->hostText.afterLast - uri->hostText.first);
						if (charsWritten + charsToWrite < maxChars) {
							memcpy(dest + charsWritten, uri->hostText.first,
									charsToWrite * sizeof(URI_CHAR));
							charsWritten += charsToWrite;
						}
					}

					/* Port */
					if (uri->portText.first != NULL) {
						const int charsToWrite = (uri->portText.afterLast - uri->portText.first);
						if (charsWritten + 1 < maxChars) {
								memcpy(dest + charsWritten, _UT(":"),
										1 * sizeof(URI_CHAR));
								charsWritten += 1;
						}
						if (charsWritten + charsToWrite < maxChars) {
							memcpy(dest + charsWritten, uri->portText.first,
									charsToWrite * sizeof(URI_CHAR));
							charsWritten += charsToWrite;
						}
					}

					/* Slash between authority and path */
					if (charsWritten + 1 < maxChars) {
							memcpy(dest + charsWritten, _UT("/"),
									1 * sizeof(URI_CHAR));
							charsWritten += 1;
					}
	/* [09/19]	endif; */
				}
	/* [10/19]	append path to result; */
				if ((uri->scheme.first == NULL) && uri->absolutePath) {
					if (charsWritten + 1 < maxChars) {
							memcpy(dest + charsWritten, _UT("/"),
									1 * sizeof(URI_CHAR));
							charsWritten += 1;
					}
				}
				if (uri->pathHead != NULL) {
					URI_TYPE(PathSegment) * walker = uri->pathHead;
					do {
						const int charsToWrite = (walker->text.afterLast - walker->text.first);
						if (charsWritten + charsToWrite < maxChars) {
							memcpy(dest + charsWritten, walker->text.first,
									charsToWrite * sizeof(URI_CHAR));
							charsWritten += charsToWrite;
						}

						if (walker->next != NULL) {
							/* Not last segment -> append slash */
							if (charsWritten + 1 < maxChars) {
								memcpy(dest + charsWritten, _UT("/"),
										1 * sizeof(URI_CHAR));
								charsWritten += 1;
							}
						}

						walker = walker->next;
					} while (walker != NULL);
				}
	/* [11/19]	if defined(query) then */
				if (uri->query.first != NULL) {
	/* [12/19]		append "?" to result; */
					if (charsWritten + 1 < maxChars) {
						memcpy(dest + charsWritten, _UT("?"),
								1 * sizeof(URI_CHAR));
						charsWritten += 1;
					}
	/* [13/19]		append query to result; */
					{
						const int charsToWrite
								= uri->query.afterLast - uri->query.first;
						if (charsWritten + charsToWrite < maxChars) {
							memcpy(dest + charsWritten, uri->query.first,
									charsToWrite * sizeof(URI_CHAR));
							charsWritten += charsToWrite;
						}
					}
	/* [14/19]	endif; */
				}
	/* [15/19]	if defined(fragment) then */
				if (uri->fragment.first != NULL) {
	/* [16/19]		append "#" to result; */
					if (charsWritten + 1 < maxChars) {
						memcpy(dest + charsWritten, _UT("#"),
								1 * sizeof(URI_CHAR));
						charsWritten += 1;
					}
	/* [17/19]		append fragment to result; */
					{
						const int charsToWrite
								= uri->fragment.afterLast - uri->fragment.first;
						if (charsWritten + charsToWrite < maxChars) {
							memcpy(dest + charsWritten, uri->fragment.first,
									charsToWrite * sizeof(URI_CHAR));
							charsWritten += charsToWrite;
						}
					}
	/* [18/19]	endif; */
				}
	/* [19/19]	return result; */
				dest[charsWritten] = _UT('\0');
				return charsWritten + 1;
}



UriBool URI_FUNC(_TESTING_ONLY_ParseIpSix)(const URI_CHAR * text) {
	URI_TYPE(Uri) uri;
	URI_TYPE(ParserState) parser;
	const URI_CHAR * const afterIpSix = text + URI_STRLEN(text);
	const URI_CHAR * res;

	URI_FUNC(ResetParserState)(&parser);
	URI_FUNC(ResetUri)(&uri);
	parser.uri = &uri;
	parser.uri->hostData.ip6 = malloc(1 * sizeof(UriIp6));
	/* TODO NULL check */
	res = URI_FUNC(ParseIPv6address2)(&parser, text, afterIpSix);
	URI_FUNC(FreeUriMembers)(&uri);
	return res == afterIpSix ? URI_TRUE : URI_FALSE;
}



UriBool URI_FUNC(_TESTING_ONLY_ParseIpFour)(const URI_CHAR * text) {
	unsigned char octets[4];
	int res = URI_FUNC(ParseIpFourAddress)(octets, text, text + URI_STRLEN(text));
	return (res == URI_SUCCESS) ? URI_TRUE : URI_FALSE;
}



#endif
