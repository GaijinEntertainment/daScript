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

#ifdef URI_TWICE_C_ENABLE



/* For prototypes */
#define URI_TWICE_H_ENABLE
#include <uriparser/UriTwice.h>
#undef  URI_TWICE_H_ENABLE


#ifdef URI_ANSI
# include <uriparser/UriAnsi.h>
#else
# include <uriparser/UriUnicode.h>
#endif



/* For NULL */
#include <stdio.h>

/* For strlen, memset */
#include <string.h>

/* For malloc */
#include <malloc.h>



/* Prototypes */
const URI_CHAR * URI_FUNC(ParseAuthority)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseAuthorityTwo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseHexZero)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseHierPart)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseIpFutLoop)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseIpFutStopGo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseIpLit2)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseIPv6address2)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseMustBeSegmentNzNc)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseOwnHost)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseOwnHost2)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseOwnHostUserInfo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseOwnHostUserInfoNz)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseOwnPortUserInfo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseOwnUserInfo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParsePartHelperTwo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParsePathAbsEmpty)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParsePathAbsNoLeadSlash)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParsePathRootless)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParsePchar)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParsePctEncoded)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParsePctSubUnres)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParsePort)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseQueryFrag)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseSegment)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseSegmentNz)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseSegmentNzNcOrScheme2)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseUriReference)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseUriTail)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseUriTailTwo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);
const URI_CHAR * URI_FUNC(ParseZeroMoreSlashSegs)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);


void URI_FUNC(Reset)(URI_TYPE(Parser) * parser);

void URI_FUNC(PushPathSegment)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast);


/*
 * [authority]-><[>[ipLit2][authorityTwo]
 * [authority]->[ownHostUserInfoNz]
 * [authority]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParseAuthority)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('['):
		{
			const URI_CHAR * const afterIpLit2
					= URI_FUNC(ParseIpLit2)(parser, first + 1, afterLast);
			if (afterIpLit2 == NULL) {
				return NULL;
			}
			parser->hostFirst = first + 1; /* HOST BEGIN */
			return URI_FUNC(ParseAuthorityTwo)(parser, afterIpLit2, afterLast);
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
		return URI_FUNC(ParseOwnHostUserInfoNz)(parser, first, afterLast);

	default:
		return first;
	}
}

/*
 * [authorityTwo]-><:>[port]
 * [authorityTwo]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParseAuthorityTwo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT(':'):
		{
			const URI_CHAR * const afterPort = URI_FUNC(ParsePort)(parser, first + 1, afterLast);
			if (afterPort == NULL) {
				return NULL;
			}
			parser->portFirst = first + 1; /* PORT BEGIN */
			parser->portAfterLast = afterPort; /* PORT END */
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
const URI_CHAR * URI_FUNC(ParseHexZero)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
		return URI_FUNC(ParseHexZero)(parser, first + 1, afterLast);

	default:
		return first;
	}
}



/*
 * [hierPart]->[pathRootless]
 * [hierPart]-></>[partHelperTwo]
 * [hierPart]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParseHierPart)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
		return URI_FUNC(ParsePathRootless)(parser, first, afterLast);

	case _UT('/'):
		return URI_FUNC(ParsePartHelperTwo)(parser, first + 1, afterLast);

	default:
		return first;
	}
}



/*
 * [ipFutLoop]->[subDelims][ipFutStopGo]
 * [ipFutLoop]->[unreserved][ipFutStopGo]
 * [ipFutLoop]-><:>[ipFutStopGo]
 */
const URI_CHAR * URI_FUNC(ParseIpFutLoop)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
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
		return URI_FUNC(ParseIpFutStopGo)(parser, first + 1, afterLast);

	default:
		return NULL;
	}
}



/*
 * [ipFutStopGo]->[ipFutLoop]
 * [ipFutStopGo]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParseIpFutStopGo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
		return URI_FUNC(ParseIpFutLoop)(parser, first, afterLast);

	default:
		return first;
	}
}



/*
 * [ipFuture]-><v>[HEXDIG][hexZero]<.>[ipFutLoop]
 */
const URI_CHAR * URI_FUNC(ParseIpFuture)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return NULL;
	}

	switch (*first) {
	case _UT('v'):
		if (first + 1 >= afterLast) {
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
						= URI_FUNC(ParseHexZero)(parser, first + 2, afterLast);
				if ((afterHexZero == NULL)
						|| (afterHexZero >= afterLast)
						|| (*afterHexZero != _UT('.'))) {
					return NULL;
				}
				parser->hostFirst = first; /* HOST BEGIN */
				afterIpFutLoop = URI_FUNC(ParseIpFutLoop)(parser, afterHexZero + 1, afterLast);
				if (afterIpFutLoop == NULL) {
					return NULL;
				}
				parser->hostAfterLast = afterIpFutLoop; /* HOST END */
				return afterIpFutLoop;
			}

		default:
			return NULL;
		}

	default:
		return NULL;
	}
}



/*
 * [ipLit2]->[ipFuture]<]>
 * [ipLit2]->[IPv6address2]
 */
const URI_CHAR * URI_FUNC(ParseIpLit2)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return NULL;
	}

	switch (*first) {
	case _UT('v'):
		{
			const URI_CHAR * const afterIpFuture
					= URI_FUNC(ParseIpFuture)(parser, first, afterLast);
			if ((afterIpFuture == NULL)
					|| (afterIpFuture >= afterLast)
					|| (*afterIpFuture != _UT(']'))) {
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
		return URI_FUNC(ParseIPv6address2)(parser, first, afterLast);

	default:
		return NULL;
	}
}



/*
 * [IPv6address2]->..<]>
 */
const URI_CHAR * URI_FUNC(ParseIPv6address2)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
#ifdef _DEBUG
	const URI_CHAR * const firstBackupForDebug = first;
#endif
	int zipperEver = 0;
	int quadsDone = 0;
	int digitCount = 0;
	int digitHistory[4];
	int ip4OctetsDone = 0;

	for (;;) {
		if (first >= afterLast) {
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
						return NULL;
					}
					digitHistory[digitCount++] = 9 + *first - _UT('9');
					break;

				case _UT('.'):
					if ((ip4OctetsDone == 4)
							|| (digitCount == 0)
							|| (digitCount == 4)
							|| ((digitCount > 1)
								&& (digitHistory[0] == 0))
							|| ((digitCount > 2)
								&& (digitHistory[1] == 0))
							|| ((digitCount == 3)
								&& (100 * digitHistory[0]
									+ 10 * digitHistory[1]
									+ digitHistory[2] > 255))) {
						return NULL;
					}
					digitCount = 0;
					ip4OctetsDone++;
					break;

				case _UT(']'):
					if ((ip4OctetsDone != 3)
							|| (digitCount == 0)
							|| (digitCount == 4)
							|| ((digitCount > 1)
								&& (digitHistory[0] == 0))
							|| ((digitCount > 2)
								&& (digitHistory[1] == 0))
							|| ((digitCount == 3)
								&& (100 * digitHistory[0]
									+ 10 * digitHistory[1]
									+ digitHistory[2] > 255))) {
						return NULL;
					}
					parser->hostAfterLast = first; /* HOST END */
					return first + 1;

				default:
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
					letterAmong = 1;
					if (digitCount == 4) {
						return NULL;
					}
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
						return NULL;
					}
					digitHistory[digitCount++] = 9 + *first - _UT('9');
					break;

				case _UT(':'):
					/* Too many quads? */
					if (quadsDone > 8 - zipperEver) {
						return NULL;
					}

					/* "::"? */
					if (first + 1 >= afterLast) {
						return NULL;
					}
					if (first[1] == _UT(':')) {
						first++;
						if (zipperEver) {
							return NULL; /* "::.+::" */
						}
						zipperEver = 1;

						/* ":::+"? */
						if (first + 1 >= afterLast) {
							return NULL; /* No ']' yet */
						}
						if (first[1] == _UT(':')) {
							return NULL; /* ":::+ "*/
						}
					}
					if (digitCount > 0) {
						quadsDone++;
						digitCount = 0;
					}
					letterAmong = 0;
					break;

				case _UT('.'):
					if (!zipperEver
							|| (quadsDone > 6)
							|| letterAmong
							|| (digitCount == 0)
							|| (digitCount == 4)
							|| ((digitCount > 1)
								&& (digitHistory[0] == 0))
							|| ((digitCount > 2)
								&& (digitHistory[1] == 0))
							|| ((digitCount == 3)
								&& (100 * digitHistory[0]
									+ 10 * digitHistory[1]
									+ digitHistory[2] > 255))) {
						return NULL;
					}
					digitCount = 0;

					/* Switch over to IPv4 loop */
					ip4OctetsDone = 1;
					walking = 0;
					break;

				case _UT(']'):
					/* Too little quads? */
					if (!zipperEver && !((quadsDone == 7) && (digitCount > 0))) {
						return NULL;
					}
					parser->hostAfterLast = first; /* HOST END */
					return first + 1; /* Fine */

				default:
					return NULL;
				}
				first++;

				if (first >= afterLast) {
					return NULL; /* No ']' yet */
				}
			} while (walking);
		}
	}

	return NULL; /* We should never get here */
}



/*
 * [mustBeSegmentNzNc]->[pctEncoded][mustBeSegmentNzNc]
 * [mustBeSegmentNzNc]->[subDelims][mustBeSegmentNzNc]
 * [mustBeSegmentNzNc]->[unreserved][mustBeSegmentNzNc]
 * [mustBeSegmentNzNc]->[uriTail] // can take <NULL>
 * [mustBeSegmentNzNc]-></>[segment][zeroMoreSlashSegs][uriTail]
 * [mustBeSegmentNzNc]-><@>[mustBeSegmentNzNc]
 */
const URI_CHAR * URI_FUNC(ParseMustBeSegmentNzNc)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('%'):
		{
			const URI_CHAR * const afterPctEncoded
					= URI_FUNC(ParsePctEncoded)(parser, first, afterLast);
			if (afterPctEncoded == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseMustBeSegmentNzNc)(parser, afterPctEncoded, afterLast);
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
		return URI_FUNC(ParseMustBeSegmentNzNc)(parser, first + 1, afterLast);

	case _UT('/'):
		{
			const URI_CHAR * afterZeroMoreSlashSegs;
			const URI_CHAR * const afterSegment
					= URI_FUNC(ParseSegment)(parser, first + 1, afterLast);
			if (afterSegment == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(parser, first + 1, afterSegment); /* SEGMENT BOTH */
			afterZeroMoreSlashSegs
					= URI_FUNC(ParseZeroMoreSlashSegs)(parser, afterSegment, afterLast);
			if (afterZeroMoreSlashSegs == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseUriTail)(parser, afterZeroMoreSlashSegs, afterLast);
		}

	default:
		return URI_FUNC(ParseUriTail)(parser, first, afterLast);
	}
}



/*
 * [ownHost]-><[>[ipLit2][authorityTwo]
 * [ownHost]->[ownHost2] // can take <NULL>
 */
const URI_CHAR * URI_FUNC(ParseOwnHost)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('['):
		{
			const URI_CHAR * const afterIpLit2
					= URI_FUNC(ParseIpLit2)(parser, first + 1, afterLast);
			if (afterIpLit2 == NULL) {
				return NULL;
			}
			parser->hostFirst = first + 1; /* HOST BEGIN */
			return URI_FUNC(ParseAuthorityTwo)(parser, afterIpLit2, afterLast);
		}

	default:
		return URI_FUNC(ParseOwnHost2)(parser, first, afterLast);
	}
}



/*
 * [ownHost2]->[authorityTwo] // can take <NULL>
 * [ownHost2]->[pctSubUnres][ownHost2]
 */
const URI_CHAR * URI_FUNC(ParseOwnHost2)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
					= URI_FUNC(ParsePctSubUnres)(parser, first, afterLast);
			if (afterPctSubUnres == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseOwnHost2)(parser, afterPctSubUnres, afterLast);
		}

	default:
		return URI_FUNC(ParseAuthorityTwo)(parser, first, afterLast);
	}
}



/*
 * [ownHostUserInfo]->[ownHostUserInfoNz]
 * [ownHostUserInfo]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParseOwnHostUserInfo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
		return URI_FUNC(ParseOwnHostUserInfoNz)(parser, first, afterLast);

	default:
		return first;
	}
}



/*
 * [ownHostUserInfoNz]->[pctSubUnres][ownHostUserInfo]
 * [ownHostUserInfoNz]-><:>[ownPortUserInfo]
 * [ownHostUserInfoNz]-><@>[ownHost]
 */
const URI_CHAR * URI_FUNC(ParseOwnHostUserInfoNz)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
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
					= URI_FUNC(ParsePctSubUnres)(parser, first, afterLast);
			if (afterPctSubUnres == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseOwnHostUserInfo)(parser, afterPctSubUnres, afterLast);
		}

	case _UT(':'):
		return URI_FUNC(ParseOwnPortUserInfo)(parser, first + 1, afterLast);

	case _UT('@'):
		return URI_FUNC(ParseOwnHost)(parser, first + 1, afterLast);

	default:
		return NULL;
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
const URI_CHAR * URI_FUNC(ParseOwnPortUserInfo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
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
		return URI_FUNC(ParseOwnUserInfo)(parser, first + 1, afterLast);

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
		return URI_FUNC(ParseOwnPortUserInfo)(parser, first + 1, afterLast);

	default:
		return first;
	}
}



/*
 * [ownUserInfo]->[pctSubUnres][ownUserInfo]
 * [ownUserInfo]-><:>[ownUserInfo]
 * [ownUserInfo]-><@>[ownHost]
 */
const URI_CHAR * URI_FUNC(ParseOwnUserInfo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
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
					= URI_FUNC(ParsePctSubUnres)(parser, first, afterLast);
			if (afterPctSubUnres == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseOwnUserInfo)(parser, afterPctSubUnres, afterLast);
		}

	case _UT(':'):
		return URI_FUNC(ParseOwnUserInfo)(parser, first + 1, afterLast);

	case _UT('@'):
		return URI_FUNC(ParseOwnHost)(parser, first + 1, afterLast);

	default:
		return NULL;
	}
}



/*
 * [partHelperTwo]->[pathAbsNoLeadSlash] // can take <NULL>
 * [partHelperTwo]-></>[authority][pathAbsEmpty]
 */
const URI_CHAR * URI_FUNC(ParsePartHelperTwo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('/'):
		{
			const URI_CHAR * const afterAuthority
					= URI_FUNC(ParseAuthority)(parser, first + 1, afterLast);
			if (afterAuthority == NULL) {
				return NULL;
			}
			return URI_FUNC(ParsePathAbsEmpty)(parser, afterAuthority, afterLast);
		}

	default:
		return URI_FUNC(ParsePathAbsNoLeadSlash)(parser, first, afterLast);
	}
}



/*
 * [pathAbsEmpty]-></>[segment][pathAbsEmpty]
 * [pathAbsEmpty]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParsePathAbsEmpty)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('/'):
		{
			const URI_CHAR * const afterSegment
					= URI_FUNC(ParseSegment)(parser, first + 1, afterLast);
			if (afterSegment == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(parser, first + 1, afterSegment); /* SEGMENT BOTH */
			return URI_FUNC(ParsePathAbsEmpty)(parser, afterSegment, afterLast);
		}


	default:
		return first;
	}
}



/*
 * [pathAbsNoLeadSlash]->[segmentNz][zeroMoreSlashSegs]
 * [pathAbsNoLeadSlash]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParsePathAbsNoLeadSlash)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
					= URI_FUNC(ParseSegmentNz)(parser, first, afterLast);
			if (afterSegmentNz == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(parser, first, afterSegmentNz); /* SEGMENT BOTH */
			return URI_FUNC(ParseZeroMoreSlashSegs)(parser, afterSegmentNz, afterLast);
		}

	default:
		return first;
	}
}



/*
 * [pathRootless]->[segmentNz][zeroMoreSlashSegs]
 */
const URI_CHAR * URI_FUNC(ParsePathRootless)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	const URI_CHAR * const afterSegmentNz
			= URI_FUNC(ParseSegmentNz)(parser, first, afterLast);
	if (afterSegmentNz == NULL) {
		return NULL;
	}
	return URI_FUNC(ParseZeroMoreSlashSegs)(parser, afterSegmentNz, afterLast);
}



/*
 * [pchar]->[pctEncoded]
 * [pchar]->[subDelims]
 * [pchar]->[unreserved]
 * [pchar]-><:>
 * [pchar]-><@>
 */
const URI_CHAR * URI_FUNC(ParsePchar)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return NULL;
	}

	switch (*first) {
	case _UT('%'):
		return URI_FUNC(ParsePctEncoded)(parser, first, afterLast);

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
		return NULL;
	}
}



/*
 * [pctEncoded]-><%>[HEXDIG][HEXDIG]
 */
const URI_CHAR * URI_FUNC(ParsePctEncoded)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return NULL;
	}

	switch (*first) {
	case _UT('%'):
		if (first + 1 >= afterLast) {
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
				return NULL;
			}

		default:
			return NULL;
		}

	default:
		return NULL;
	}
}



/*
 * [pctSubUnres]->[pctEncoded]
 * [pctSubUnres]->[subDelims]
 * [pctSubUnres]->[unreserved]
 */
const URI_CHAR * URI_FUNC(ParsePctSubUnres)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return NULL;
	}

	switch (*first) {
	case _UT('%'):
		return URI_FUNC(ParsePctEncoded)(parser, first, afterLast);

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
		return NULL;
	}
}



/*
 * [port]->[DIGIT][port]
 * [port]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParsePort)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
		return URI_FUNC(ParsePort)(parser, first + 1, afterLast);

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
const URI_CHAR * URI_FUNC(ParseQueryFrag)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
					= URI_FUNC(ParsePchar)(parser, first, afterLast);
			if (afterPchar == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseQueryFrag)(parser, afterPchar, afterLast);
		}

	case _UT('/'):
	case _UT('?'):
		return URI_FUNC(ParseQueryFrag)(parser, first + 1, afterLast);

	default:
		return first;
	}
}



/*
 * [segment]->[pchar][segment]
 * [segment]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParseSegment)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
					= URI_FUNC(ParsePchar)(parser, first, afterLast);
			if (afterPchar == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseSegment)(parser, afterPchar, afterLast);
		}

	default:
		return first;
	}
}



/*
 * [segmentNz]->[pchar][segment]
 */
const URI_CHAR * URI_FUNC(ParseSegmentNz)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	const URI_CHAR * const afterPchar
			= URI_FUNC(ParsePchar)(parser, first, afterLast);
	if (afterPchar == NULL) {
		return NULL;
	}
	return URI_FUNC(ParseSegment)(parser, afterPchar, afterLast);
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
const URI_CHAR * URI_FUNC(ParseSegmentNzNcOrScheme2)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
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
		return URI_FUNC(ParseSegmentNzNcOrScheme2)(parser, first + 1, afterLast);

	case _UT('%'):
		{
			const URI_CHAR * const afterPctEncoded
					= URI_FUNC(ParsePctEncoded)(parser, first, afterLast);
			if (afterPctEncoded == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseMustBeSegmentNzNc)(parser, afterPctEncoded, afterLast);
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
		return URI_FUNC(ParseMustBeSegmentNzNc)(parser, first + 1, afterLast);

	case _UT('/'):
		{
			const URI_CHAR * afterZeroMoreSlashSegs;
			const URI_CHAR * const afterSegment
					= URI_FUNC(ParseSegment)(parser, first + 1, afterLast);
			if (afterSegment == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(parser, parser->schemeFirst, first); /* SEGMENT BOTH */
			parser->schemeFirst = NULL; /* Not a scheme, reset */
			URI_FUNC(PushPathSegment)(parser, first + 1, afterSegment); /* SEGMENT BOTH */
			afterZeroMoreSlashSegs
					= URI_FUNC(ParseZeroMoreSlashSegs)(parser, afterSegment, afterLast);
			if (afterZeroMoreSlashSegs == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseUriTail)(parser, afterZeroMoreSlashSegs, afterLast);
		}

	case _UT(':'):
		{
			const URI_CHAR * const afterHierPart
					= URI_FUNC(ParseHierPart)(parser, first + 1, afterLast);
			parser->schemeAfterLast = first; /* SCHEME END */
			if (afterHierPart == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseUriTail)(parser, afterHierPart, afterLast);
		}

	default:
		URI_FUNC(PushPathSegment)(parser, parser->schemeFirst, first); /* SEGMENT BOTH */
		parser->schemeFirst = NULL; /* Not a scheme, reset */
		return URI_FUNC(ParseUriTail)(parser, first, afterLast);
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
const URI_CHAR * URI_FUNC(ParseUriReference)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
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
		parser->schemeFirst = first; /* SCHEME BEGIN */
		return URI_FUNC(ParseSegmentNzNcOrScheme2)(parser, first + 1, afterLast);

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
		parser->schemeFirst = first; /* SCHEME BEGIN */
		return URI_FUNC(ParseMustBeSegmentNzNc)(parser, first + 1, afterLast);

	case _UT('%'):
		{
			const URI_CHAR * const afterPctEncoded
					= URI_FUNC(ParsePctEncoded)(parser, first, afterLast);
			if (afterPctEncoded == NULL) {
				return NULL;
			}
			parser->schemeFirst = first; /* SCHEME BEGIN */
			return URI_FUNC(ParseMustBeSegmentNzNc)(parser, afterPctEncoded, afterLast);
		}

	case _UT('/'):
		{
			const URI_CHAR * const afterPartHelperTwo
					= URI_FUNC(ParsePartHelperTwo)(parser, first + 1, afterLast);
			if (afterPartHelperTwo == NULL) {
				return NULL;
			}
			return URI_FUNC(ParseUriTail)(parser, afterPartHelperTwo, afterLast);
		}

	default:
		return URI_FUNC(ParseUriTail)(parser, first, afterLast);
	}
}



/*
 * [uriTail]-><#>[queryFrag]
 * [uriTail]-><?>[queryFrag][uriTailTwo]
 * [uriTail]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParseUriTail)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('#'):
		{
			const URI_CHAR * const afterQueryFrag = URI_FUNC(ParseQueryFrag)(parser, first + 1, afterLast);
			if (afterQueryFrag == NULL) {
				return NULL;
			}
			parser->fragmentFirst = first + 1; /* FRAGMENT BEGIN */
			parser->fragmentAfterLast = afterQueryFrag; /* FRAGMENT END */
			return afterQueryFrag;
		}

	case _UT('?'):
		{
			const URI_CHAR * const afterQueryFrag
					= URI_FUNC(ParseQueryFrag)(parser, first + 1, afterLast);
			if (afterQueryFrag == NULL) {
				return NULL;
			}
			parser->queryFirst = first + 1; /* QUERY BEGIN */
			parser->queryAfterLast = afterQueryFrag; /* QUERY END */
			return URI_FUNC(ParseUriTailTwo)(parser, afterQueryFrag, afterLast);
		}

	default:
		return first;
	}
}



/*
 * [uriTailTwo]-><#>[queryFrag]
 * [uriTailTwo]-><NULL>
 */
const URI_CHAR * URI_FUNC(ParseUriTailTwo)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('#'):
		{
			const URI_CHAR * const afterQueryFrag = URI_FUNC(ParseQueryFrag)(parser, first + 1, afterLast);
			if (afterQueryFrag == NULL) {
				return NULL;
			}
			parser->fragmentFirst = first + 1; /* FRAGMENT BEGIN */
			parser->fragmentAfterLast = afterQueryFrag; /* FRAGMENT END */
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
const URI_CHAR * URI_FUNC(ParseZeroMoreSlashSegs)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	if (first >= afterLast) {
		return afterLast;
	}

	switch (*first) {
	case _UT('/'):
		{
			const URI_CHAR * const afterSegment
					= URI_FUNC(ParseSegment)(parser, first + 1, afterLast);
			if (afterSegment == NULL) {
				return NULL;
			}
			URI_FUNC(PushPathSegment)(parser, first + 1, afterSegment); /* SEGMENT BOTH */
			return URI_FUNC(ParseZeroMoreSlashSegs)(parser, afterSegment, afterLast);
		}

	default:
		return first;
	}
}



void URI_FUNC(Reset)(URI_TYPE(Parser) * parser) {
	/* TODO Free path list here? Extra function? */
	memset(parser, 0, sizeof(URI_TYPE(Parser)));
}



void URI_FUNC(PushPathSegment)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	URI_TYPE(PathSegment) * segment = malloc(1 * sizeof(URI_TYPE(PathSegment)));
	memset(segment, 0, sizeof(URI_TYPE(PathSegment)));
	segment->first = first;
	segment->afterLast = afterLast;

	if (parser->pathHead == NULL) {
		parser->pathHead = segment;
		parser->pathTail = segment;
	} else {
		parser->pathTail->next = segment;
		parser->pathTail = segment;
	}
}



UriBool URI_FUNC(ParseUriEx)(URI_TYPE(Parser) * parser, const URI_CHAR * first, const URI_CHAR * afterLast) {
	const URI_CHAR * afterUriReference;
	URI_FUNC(Reset)(parser);
	afterUriReference = URI_FUNC(ParseUriReference)(parser, first, afterLast);
	if (afterUriReference == NULL) {
		return URI_ERROR;
	}
	if (afterUriReference != afterLast) {
		return URI_ERROR;
	}
	return URI_SUCCESS;
}

UriBool URI_FUNC(ParseUri)(URI_TYPE(Parser) * parser, const URI_CHAR * text) {
	return URI_FUNC(ParseUriEx)(parser, text, text + URI_STRLEN(text));
}


/* TODO */
UriBool URI_FUNC(ParseIpSix)(const URI_CHAR * text) {
	URI_TYPE(Parser) parser = { 0 };
	const URI_CHAR * const afterIpSix = text + URI_STRLEN(text);
	const URI_CHAR * const res = URI_FUNC(ParseIPv6address2)(&parser, text, afterIpSix);
	return res == afterIpSix ? URI_TRUE : URI_FALSE;
}



#endif /* URI_TWICE_C_ENABLE */
