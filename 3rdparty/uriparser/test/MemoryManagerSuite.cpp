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

#undef NDEBUG  // because we rely on assert(3) further down

#include <cassert>
#include <cerrno>
#include <cstring>  // memcpy
#include <gtest/gtest.h>

#include <uriparser/Uri.h>
#include "../src/UriMemory.h"


namespace {

class CallCountLog {
public:
	unsigned int callCountFree;

	CallCountLog() : callCountFree(0) {
		// no-op
	}
};



static void * failingMalloc(UriMemoryManager * URI_UNUSED(memory),
		size_t URI_UNUSED(size)) {
	return NULL;
}



static void * failingCalloc(UriMemoryManager * URI_UNUSED(memory),
		size_t URI_UNUSED(nmemb), size_t URI_UNUSED(size)) {
	return NULL;
}



static void * failingRealloc(UriMemoryManager * URI_UNUSED(memory),
		void * URI_UNUSED(ptr), size_t URI_UNUSED(size)) {
	return NULL;
}



static void * failingReallocarray(UriMemoryManager * URI_UNUSED(memory),
		void * URI_UNUSED(ptr), size_t URI_UNUSED(nmemb),
		size_t URI_UNUSED(size)) {
	return NULL;
}



static void countingFree(UriMemoryManager * memory, void * ptr) {
	static_cast<CallCountLog *>(memory->userData)->callCountFree++;
	free(ptr);
}



class FailingMemoryManager {
private:
	UriMemoryManager memoryManager;
	CallCountLog callCountLog;

public:
	FailingMemoryManager() {
		this->memoryManager.malloc = failingMalloc;
		this->memoryManager.calloc = failingCalloc;
		this->memoryManager.realloc = failingRealloc;
		this->memoryManager.reallocarray = failingReallocarray;
		this->memoryManager.free = countingFree;
		this->memoryManager.userData = &(this->callCountLog);
	}

	UriMemoryManager * operator&() {
		return &(this->memoryManager);
	}

	unsigned int getCallCountFree() const {
		return this->callCountLog.callCountFree;
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



TEST(MemoryManagerCompletenessSuite, AllFunctionMembersRequired) {
	UriUriA uri = parse("whatever");
	UriMemoryManager memory;

	memcpy(&memory, &defaultMemoryManager, sizeof(UriMemoryManager));
	memory.malloc = NULL;
	ASSERT_EQ(uriFreeUriMembersMmA(&uri, &memory),
			  URI_ERROR_MEMORY_MANAGER_INCOMPLETE);

	memcpy(&memory, &defaultMemoryManager, sizeof(UriMemoryManager));
	memory.calloc = NULL;
	ASSERT_EQ(uriFreeUriMembersMmA(&uri, &memory),
			  URI_ERROR_MEMORY_MANAGER_INCOMPLETE);

	memcpy(&memory, &defaultMemoryManager, sizeof(UriMemoryManager));
	memory.realloc = NULL;
	ASSERT_EQ(uriFreeUriMembersMmA(&uri, &memory),
			  URI_ERROR_MEMORY_MANAGER_INCOMPLETE);

	memcpy(&memory, &defaultMemoryManager, sizeof(UriMemoryManager));
	memory.reallocarray = NULL;
	ASSERT_EQ(uriFreeUriMembersMmA(&uri, &memory),
			  URI_ERROR_MEMORY_MANAGER_INCOMPLETE);

	memcpy(&memory, &defaultMemoryManager, sizeof(UriMemoryManager));
	memory.free = NULL;
	ASSERT_EQ(uriFreeUriMembersMmA(&uri, &memory),
			  URI_ERROR_MEMORY_MANAGER_INCOMPLETE);

	memcpy(&memory, &defaultMemoryManager, sizeof(UriMemoryManager));
	ASSERT_EQ(uriFreeUriMembersMmA(&uri, &memory), URI_SUCCESS);
}



TEST(MemoryManagerCompletenessSuite, MallocAndFreeRequiredOnly) {
	UriMemoryManager memory;
	UriMemoryManager backend;

	memcpy(&backend, &defaultMemoryManager, sizeof(UriMemoryManager));
	backend.malloc = NULL;
	ASSERT_EQ(uriCompleteMemoryManager(&memory, &backend),
			  URI_ERROR_MEMORY_MANAGER_INCOMPLETE);

	memcpy(&backend, &defaultMemoryManager, sizeof(UriMemoryManager));
	backend.free = NULL;
	ASSERT_EQ(uriCompleteMemoryManager(&memory, &backend),
			  URI_ERROR_MEMORY_MANAGER_INCOMPLETE);
}



TEST(MemoryManagerTestingSuite, DefaultMemoryManager) {
	ASSERT_EQ(uriTestMemoryManager(&defaultMemoryManager), URI_SUCCESS);
}



TEST(MemoryManagerTestingSuite, CompleteMemoryManager) {
	UriMemoryManager memory;
	UriMemoryManager backend;

	memset(&backend, 0, sizeof(UriMemoryManager));
	backend.malloc = defaultMemoryManager.malloc;
	backend.free = defaultMemoryManager.free;

	ASSERT_EQ(uriCompleteMemoryManager(&memory, &backend),
			URI_SUCCESS);

	ASSERT_EQ(uriTestMemoryManager(&memory), URI_SUCCESS);
}



TEST(MemoryManagerTestingSuite, EmulateCalloc) {
	UriMemoryManager partialEmulationMemoryManager;
	memcpy(&partialEmulationMemoryManager, &defaultMemoryManager,
			sizeof(UriMemoryManager));
	partialEmulationMemoryManager.calloc = uriEmulateCalloc;

	ASSERT_EQ(uriTestMemoryManager(&partialEmulationMemoryManager),
			URI_SUCCESS);
}



TEST(MemoryManagerTestingSuite, EmulateReallocarray) {
	UriMemoryManager partialEmulationMemoryManager;
	memcpy(&partialEmulationMemoryManager, &defaultMemoryManager,
			sizeof(UriMemoryManager));
	partialEmulationMemoryManager.reallocarray = uriEmulateReallocarray;

	ASSERT_EQ(uriTestMemoryManager(&partialEmulationMemoryManager),
			URI_SUCCESS);
}



TEST(MemoryManagerTestingOverflowDetectionSuite, EmulateCalloc) {
	EXPECT_GT(2 * sizeof(size_t), sizeof(void *));

	errno = 0;
	ASSERT_EQ(NULL, uriEmulateCalloc(
			&defaultMemoryManager, (size_t)-1, (size_t)-1));
	ASSERT_EQ(errno, ENOMEM);
}



TEST(MemoryManagerTestingOverflowDetectionSuite, EmulateReallocarray) {
	EXPECT_GT(2 * sizeof(size_t), sizeof(void *));

	errno = 0;
	ASSERT_EQ(NULL, uriEmulateReallocarray(
			&defaultMemoryManager, NULL, (size_t)-1, (size_t)-1));
	ASSERT_EQ(errno, ENOMEM);
}



TEST(MemoryManagerTestingSuite, EmulateCallocAndReallocarray) {
	UriMemoryManager partialEmulationMemoryManager;
	memcpy(&partialEmulationMemoryManager, &defaultMemoryManager,
			sizeof(UriMemoryManager));
	partialEmulationMemoryManager.calloc = uriEmulateCalloc;
	partialEmulationMemoryManager.reallocarray = uriEmulateReallocarray;

	ASSERT_EQ(uriTestMemoryManager(&partialEmulationMemoryManager),
			URI_SUCCESS);
}



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



TEST(FailingMemoryManagerSuite, ParseSingleUriExMm) {
	UriUriA uri;
	const char * const first = "k1=v1&k2=v2";
	const char * const afterLast = first + strlen(first);
	FailingMemoryManager failingMemoryManager;

	ASSERT_EQ(uriParseSingleUriExMmA(&uri, first, afterLast, NULL,
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
