/*
 * uriparser - RFC 3986 URI parsing library
 *
 * Copyright (C) 2007, Weijia Song <songweijia@gmail.com>
 * Copyright (C) 2007, Sebastian Pipping <sebastian@pipping.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <cassert>
#include <gtest/gtest.h>

#include <uriparser/Uri.h>


namespace {

class CallCountLog {
public:
	unsigned int callCountMalloc;
	unsigned int callCountCalloc;
	unsigned int callCountRealloc;
	unsigned int callCountReallocarray;
	unsigned int callCountFree;

	CallCountLog() : callCountMalloc(0), callCountCalloc(0),
			callCountRealloc(0), callCountReallocarray(0), callCountFree(0) {
		// no-op
	}
};



static void * failingMalloc(UriMemoryManager * memory, size_t size) {
	static_cast<CallCountLog *>(memory->userData)->callCountMalloc++;
	return NULL;
}



static void * failingCalloc(UriMemoryManager * memory,
		size_t nmemb, size_t size) {
	static_cast<CallCountLog *>(memory->userData)->callCountCalloc++;
	return NULL;
}



static void * failingRealloc(UriMemoryManager * memory,
		void * ptr, size_t size) {
	static_cast<CallCountLog *>(memory->userData)->callCountRealloc++;
	return NULL;
}



static void * failingReallocarray(UriMemoryManager * memory,
		void * ptr, size_t nmemb, size_t size) {
	static_cast<CallCountLog *>(memory->userData)->callCountReallocarray++;
	return NULL;
}



static void failingFree(UriMemoryManager * memory, void * ptr) {
	static_cast<CallCountLog *>(memory->userData)->callCountFree++;
	/* no-op */
}



class FailingMemoryManager : public UriMemoryManager {
public:
	FailingMemoryManager() {
		this->malloc = failingMalloc;
		this->calloc = failingCalloc,
		this->realloc = failingRealloc,
		this->reallocarray = failingReallocarray,
		this->free = failingFree,

		this->userData = new CallCountLog();

		// Fail fast if deriving from C struct turns out a problem
		// Move from inheritance to composition in that case
		assert(sizeof(FailingMemoryManager) == sizeof(UriMemoryManager));
	}

	~FailingMemoryManager() {
		delete getCallCountLog();
		this->userData = NULL;
	}

	unsigned int getCallCountFree() const {
		return this->getCallCountLog()->callCountFree;
	}

private:
	const CallCountLog * getCallCountLog() const {
		return static_cast<CallCountLog *>(this->userData);
	}
};



static UriUriA parse(const char * sourceUriString) {
	UriParserStateA state;
	UriUriA uri;
	state.uri = &uri;
	assert(uriParseUriA(&state, sourceUriString) == URI_SUCCESS);
	return uri;
}



static UriQueryListA * parseQueryList(const char * queryString) {
	UriQueryListA * queryList;
	const char * const first = queryString;
	const char * const afterLast = first + strlen(first);
	assert(uriDissectQueryMallocA(&queryList, NULL, first, afterLast)
			== URI_SUCCESS);
	return queryList;
}

}  // namespace



TEST(FailingMemoryManagerSuite, AddBaseUriExMm) {
	UriUriA absoluteDest;
	UriUriA relativeSource = parse("foo");
	UriUriA absoluteBase = parse("http://example.org/bar");
	const UriResolutionOptions options = URI_RESOLVE_STRICTLY;
	FailingMemoryManager failingMemoryManager;

	ASSERT_EQ(uriAddBaseUriExMmA(&absoluteDest, &relativeSource,
			&absoluteBase, options, &failingMemoryManager),
			URI_ERROR_MALLOC);

	uriFreeUriMembersA(&relativeSource);
	uriFreeUriMembersA(&absoluteBase);
}



TEST(FailingMemoryManagerSuite, ComposeQueryMallocExMm) {
	char * dest = NULL;
	UriQueryListA * const queryList = parseQueryList("k1=v1");
	UriBool spaceToPlus = URI_TRUE;  // not of interest
	UriBool normalizeBreaks = URI_TRUE;  // not of interest
	FailingMemoryManager failingMemoryManager;

	ASSERT_EQ(uriComposeQueryMallocExMmA(&dest, queryList,
			spaceToPlus, normalizeBreaks, &failingMemoryManager),
			URI_ERROR_MALLOC);

	uriFreeQueryListA(queryList);
}



TEST(FailingMemoryManagerSuite, DissectQueryMallocExMm) {
	UriQueryListA * queryList;
	int itemCount;
	const char * const first = "k1=v1&k2=v2";
	const char * const afterLast = first + strlen(first);
	const UriBool plusToSpace = URI_TRUE;  // not of interest
	const UriBreakConversion breakConversion = URI_BR_DONT_TOUCH;  // not o. i.
	FailingMemoryManager failingMemoryManager;

	ASSERT_EQ(uriDissectQueryMallocExMmA(&queryList, &itemCount,
			first, afterLast, plusToSpace, breakConversion,
			&failingMemoryManager),
			URI_ERROR_MALLOC);
}



TEST(FailingMemoryManagerSuite, FreeQueryListMm) {
	UriQueryListA * const queryList = parseQueryList("k1=v1");
	FailingMemoryManager failingMemoryManager;
	ASSERT_EQ(failingMemoryManager.getCallCountFree(), 0);

	uriFreeQueryListMmA(queryList, &failingMemoryManager);

	ASSERT_GE(failingMemoryManager.getCallCountFree(), 1);
	uriFreeQueryListA(queryList);
}



TEST(FailingMemoryManagerSuite, FreeUriMembersMm) {
	UriUriA uri = parse("http://example.org/");
	FailingMemoryManager failingMemoryManager;
	ASSERT_EQ(failingMemoryManager.getCallCountFree(), 0);

	uriFreeUriMembersMmA(&uri, &failingMemoryManager);

	ASSERT_GE(failingMemoryManager.getCallCountFree(), 1);
	uriFreeUriMembersA(&uri);
}



TEST(FailingMemoryManagerSuite, NormalizeSyntaxExMm) {
	UriUriA uri = parse("hTTp://example.org/path");
	const unsigned int mask = URI_NORMALIZE_SCHEME;  // anything but URI_NORMALIZED
	FailingMemoryManager failingMemoryManager;

	ASSERT_EQ(uriNormalizeSyntaxExMmA(&uri, mask, &failingMemoryManager),
			URI_ERROR_MALLOC);

	uriFreeUriMembersA(&uri);
}



TEST(FailingMemoryManagerSuite, ParseUriExMm) {
	UriParserStateA state;
	UriUriA uri;
	state.uri = &uri;
	const char * const first = "k1=v1&k2=v2";
	const char * const afterLast = first + strlen(first);
	FailingMemoryManager failingMemoryManager;

	ASSERT_EQ(uriParseUriExMmA(&state, first, afterLast,
			&failingMemoryManager),
			URI_ERROR_MALLOC);
}



TEST(FailingMemoryManagerSuite, RemoveBaseUriMm) {
	UriUriA dest;
	UriUriA absoluteSource = parse("http://example.org/a/b/c/");
	UriUriA absoluteBase = parse("http://example.org/a/");
	const UriBool domainRootMode = URI_TRUE;  // not of interest
	FailingMemoryManager failingMemoryManager;

	ASSERT_EQ(uriRemoveBaseUriMmA(&dest, &absoluteSource, &absoluteBase,
			domainRootMode, &failingMemoryManager),
			URI_ERROR_MALLOC);

	uriFreeUriMembersA(&absoluteSource);
	uriFreeUriMembersA(&absoluteBase);
}
