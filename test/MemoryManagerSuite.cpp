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

static void * failingMalloc(UriMemoryManager * URI_UNUSED(memory),
		size_t size) {
	return NULL;
}



static void * failingCalloc(UriMemoryManager * URI_UNUSED(memory),
		size_t nmemb, size_t size) {
	return NULL;
}



static void * failingRealloc(UriMemoryManager * URI_UNUSED(memory),
		void * ptr, size_t size) {
	return NULL;
}



static void * failingReallocarray(UriMemoryManager * URI_UNUSED(memory),
		void * ptr, size_t nmemb, size_t size) {
	return NULL;
}



static void failingFree(UriMemoryManager * URI_UNUSED(memory), void * ptr) {
	/* no-op */
}



static UriUriA parse(const char * sourceUriString) {
	UriParserStateA state;
	UriUriA uri;
	state.uri = &uri;
	assert(uriParseUriA(&state, sourceUriString) == URI_SUCCESS);
	return uri;
}



static UriMemoryManager failingMemoryManager = {
	failingMalloc,
	failingCalloc,
	failingRealloc,
	failingReallocarray,
	failingFree,
	NULL  /* userData */
};

}  // namespace



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
