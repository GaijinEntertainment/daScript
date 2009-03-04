/*
 * uriparser - RFC 3986 URI parsing library
 *
 * Copyright (C) 2007, Weijia Song <songweijia@gmail.com>
 * Copyright (C) 2007, Sebastian Pipping <webmaster@hartwork.org>
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

#include "FourSuite.h"

#include <uriparser/Uri.h>



// All testcases in this file are coming from
// http://cvs.4suite.org/viewcvs/4Suite/test/Lib/test_uri.py



bool FourSuite::testAddOrRemoveBaseHelper(const char * ref, const char * base,
	const char * expected, bool add, bool domainRootMode) {
	UriParserStateA stateA;

	// Base
	UriUriA baseUri;
	stateA.uri = &baseUri;
	int res = uriParseUriA(&stateA, base);
	if (res != 0) {
		return false;
	}

	// Rel
	UriUriA relUri;
	stateA.uri = &relUri;
	res = uriParseUriA(&stateA, ref);
	if (res != 0) {
		uriFreeUriMembersA(&baseUri);
		return false;
	}

	// Expected result
	UriUriA expectedUri;
	stateA.uri = &expectedUri;
	res = uriParseUriA(&stateA, expected);
	if (res != 0) {
		uriFreeUriMembersA(&baseUri);
		uriFreeUriMembersA(&relUri);
		uriFreeUriMembersA(&expectedUri);
		return false;
	}

	// Transform
	UriUriA transformedUri;
	if (add) {
		res = uriAddBaseUriA(&transformedUri, &relUri, &baseUri);
	} else {
		res = uriRemoveBaseUriA(&transformedUri, &relUri, &baseUri,
				domainRootMode ? URI_TRUE : URI_FALSE);
	}
	if (res != 0) {
		uriFreeUriMembersA(&baseUri);
		uriFreeUriMembersA(&relUri);
		uriFreeUriMembersA(&expectedUri);
		uriFreeUriMembersA(&transformedUri);
		return false;
	}

	const bool equal = (URI_TRUE == uriEqualsUriA(&transformedUri, &expectedUri));
	if (!equal) {
		char transformedUriText[1024 * 8];
		char expectedUriText[1024 * 8];
		uriToStringA(transformedUriText, &transformedUri, 1024 * 8, NULL);
		uriToStringA(expectedUriText, &expectedUri, 1024 * 8, NULL);
		printf("\n\n\nExpected: \"%s\"\nReceived: \"%s\"\n\n\n", expectedUriText, transformedUriText);
	}

	uriFreeUriMembersA(&baseUri);
	uriFreeUriMembersA(&relUri);
	uriFreeUriMembersA(&expectedUri);
	uriFreeUriMembersA(&transformedUri);
	return equal;
}



void FourSuite::absolutize_test_cases() {
	const char * const BASE_URI[] = {
			"http://a/b/c/d;p?q",
			"http://a/b/c/d;p?q=1/2",
			"http://a/b/c/d;p=1/2?q",
			"fred:///s//a/b/c",
			"http:///s//a/b/c"};

	// ref, base, exptected

	// http://lists.w3.org/Archives/Public/uri/2004Feb/0114.html
	TEST_ASSERT(testAddOrRemoveBaseHelper("../c", "foo:a/b", "foo:c"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("foo:.", "foo:a", "foo:"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/foo/../../../bar", "zz:abc", "zz:/bar"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/foo/../bar", "zz:abc", "zz:/bar"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("foo/../../../bar", "zz:abc", "zz:bar"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("foo/../bar", "zz:abc", "zz:bar"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("zz:.", "zz:abc", "zz:"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/.", BASE_URI[0], "http://a/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/.foo", BASE_URI[0], "http://a/.foo"));
	TEST_ASSERT(testAddOrRemoveBaseHelper(".foo", BASE_URI[0], "http://a/b/c/.foo"));

	// http://gbiv.com/protocols/uri/test/rel_examples1.html
	// examples from RFC 2396
	TEST_ASSERT(testAddOrRemoveBaseHelper("g:h", BASE_URI[0], "g:h"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g", BASE_URI[0], "http://a/b/c/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./g", BASE_URI[0], "http://a/b/c/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g/", BASE_URI[0], "http://a/b/c/g/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/g", BASE_URI[0], "http://a/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("//g", BASE_URI[0], "http://g"));

	// changed with RFC 2396bis
	TEST_ASSERT(testAddOrRemoveBaseHelper("?y", BASE_URI[0], "http://a/b/c/d;p?y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g?y", BASE_URI[0], "http://a/b/c/g?y"));

	// changed with RFC 2396bis
	TEST_ASSERT(testAddOrRemoveBaseHelper("#s", BASE_URI[0], "http://a/b/c/d;p?q#s"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g#s", BASE_URI[0], "http://a/b/c/g#s"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g?y#s", BASE_URI[0], "http://a/b/c/g?y#s"));
	TEST_ASSERT(testAddOrRemoveBaseHelper(";x", BASE_URI[0], "http://a/b/c/;x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g;x", BASE_URI[0], "http://a/b/c/g;x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g;x?y#s", BASE_URI[0], "http://a/b/c/g;x?y#s"));

	// changed with RFC 2396bis
	TEST_ASSERT(testAddOrRemoveBaseHelper("", BASE_URI[0], "http://a/b/c/d;p?q"));
	TEST_ASSERT(testAddOrRemoveBaseHelper(".", BASE_URI[0], "http://a/b/c/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./", BASE_URI[0], "http://a/b/c/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("..", BASE_URI[0], "http://a/b/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../", BASE_URI[0], "http://a/b/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../g", BASE_URI[0], "http://a/b/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../..", BASE_URI[0], "http://a/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../", BASE_URI[0], "http://a/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../g", BASE_URI[0], "http://a/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../../g", BASE_URI[0], "http://a/g")); // http://a/../g
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../../../g", BASE_URI[0], "http://a/g")); // http://a/../../g

	// changed with RFC 2396bis
	TEST_ASSERT(testAddOrRemoveBaseHelper("/./g", BASE_URI[0], "http://a/g"));

	// changed with RFC 2396bis
	TEST_ASSERT(testAddOrRemoveBaseHelper("/../g", BASE_URI[0], "http://a/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g.", BASE_URI[0], "http://a/b/c/g."));
	TEST_ASSERT(testAddOrRemoveBaseHelper(".g", BASE_URI[0], "http://a/b/c/.g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g..", BASE_URI[0], "http://a/b/c/g.."));
	TEST_ASSERT(testAddOrRemoveBaseHelper("..g", BASE_URI[0], "http://a/b/c/..g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./../g", BASE_URI[0], "http://a/b/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./g/.", BASE_URI[0], "http://a/b/c/g/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g/./h", BASE_URI[0], "http://a/b/c/g/h"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g/../h", BASE_URI[0], "http://a/b/c/h"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g;x=1/./y", BASE_URI[0], "http://a/b/c/g;x=1/y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g;x=1/../y", BASE_URI[0], "http://a/b/c/y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g?y/./x", BASE_URI[0], "http://a/b/c/g?y/./x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g?y/../x", BASE_URI[0], "http://a/b/c/g?y/../x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g#s/./x", BASE_URI[0], "http://a/b/c/g#s/./x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g#s/../x", BASE_URI[0], "http://a/b/c/g#s/../x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("http:g", BASE_URI[0], "http:g")); // http://a/b/c/g
	TEST_ASSERT(testAddOrRemoveBaseHelper("http:", BASE_URI[0], "http:")); // BASE_URI[0]

	// not sure where this one originated
	TEST_ASSERT(testAddOrRemoveBaseHelper("/a/b/c/./../../g", BASE_URI[0], "http://a/a/g"));

	// http://gbiv.com/protocols/uri/test/rel_examples2.html
	// slashes in base URI's query args
	TEST_ASSERT(testAddOrRemoveBaseHelper("g", BASE_URI[1], "http://a/b/c/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./g", BASE_URI[1], "http://a/b/c/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g/", BASE_URI[1], "http://a/b/c/g/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/g", BASE_URI[1], "http://a/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("//g", BASE_URI[1], "http://g"));

	// changed in RFC 2396bis
	// TEST_ASSERT(testAddOrRemoveBaseHelper("?y", BASE_URI[1], "http://a/b/c/?y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("?y", BASE_URI[1], "http://a/b/c/d;p?y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g?y", BASE_URI[1], "http://a/b/c/g?y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g?y/./x", BASE_URI[1], "http://a/b/c/g?y/./x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g?y/../x", BASE_URI[1], "http://a/b/c/g?y/../x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g#s", BASE_URI[1], "http://a/b/c/g#s"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g#s/./x", BASE_URI[1], "http://a/b/c/g#s/./x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g#s/../x", BASE_URI[1], "http://a/b/c/g#s/../x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./", BASE_URI[1], "http://a/b/c/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../", BASE_URI[1], "http://a/b/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../g", BASE_URI[1], "http://a/b/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../", BASE_URI[1], "http://a/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../g", BASE_URI[1], "http://a/g"));

	// http://gbiv.com/protocols/uri/test/rel_examples3.html
	// slashes in path params
	// all of these changed in RFC 2396bis
	TEST_ASSERT(testAddOrRemoveBaseHelper("g", BASE_URI[2], "http://a/b/c/d;p=1/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./g", BASE_URI[2], "http://a/b/c/d;p=1/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g/", BASE_URI[2], "http://a/b/c/d;p=1/g/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g?y", BASE_URI[2], "http://a/b/c/d;p=1/g?y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper(";x", BASE_URI[2], "http://a/b/c/d;p=1/;x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g;x", BASE_URI[2], "http://a/b/c/d;p=1/g;x"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g;x=1/./y", BASE_URI[2], "http://a/b/c/d;p=1/g;x=1/y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g;x=1/../y", BASE_URI[2], "http://a/b/c/d;p=1/y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./", BASE_URI[2], "http://a/b/c/d;p=1/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../", BASE_URI[2], "http://a/b/c/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../g", BASE_URI[2], "http://a/b/c/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../", BASE_URI[2], "http://a/b/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../g", BASE_URI[2], "http://a/b/g"));

	// http://gbiv.com/protocols/uri/test/rel_examples4.html
	// double and triple slash, unknown scheme
	TEST_ASSERT(testAddOrRemoveBaseHelper("g:h", BASE_URI[3], "g:h"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g", BASE_URI[3], "fred:///s//a/b/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./g", BASE_URI[3], "fred:///s//a/b/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g/", BASE_URI[3], "fred:///s//a/b/g/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/g", BASE_URI[3], "fred:///g")); // may change to fred:///s//a/g
	TEST_ASSERT(testAddOrRemoveBaseHelper("//g", BASE_URI[3], "fred://g")); // may change to fred:///s//g
	TEST_ASSERT(testAddOrRemoveBaseHelper("//g/x", BASE_URI[3], "fred://g/x")); // may change to fred:///s//g/x
	TEST_ASSERT(testAddOrRemoveBaseHelper("///g", BASE_URI[3], "fred:///g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./", BASE_URI[3], "fred:///s//a/b/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../", BASE_URI[3], "fred:///s//a/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../g", BASE_URI[3], "fred:///s//a/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../", BASE_URI[3], "fred:///s//")); // may change to fred:///s//a/../
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../g", BASE_URI[3], "fred:///s//g")); // may change to fred:///s//a/../g
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../../g", BASE_URI[3], "fred:///s/g")); // may change to fred:///s//a/../../g
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../../../g", BASE_URI[3], "fred:///g")); // may change to fred:///s//a/../../../g

	// http://gbiv.com/protocols/uri/test/rel_examples5.html
	// double and triple slash, well-known scheme
	TEST_ASSERT(testAddOrRemoveBaseHelper("g:h", BASE_URI[4], "g:h"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g", BASE_URI[4], "http:///s//a/b/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./g", BASE_URI[4], "http:///s//a/b/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("g/", BASE_URI[4], "http:///s//a/b/g/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/g", BASE_URI[4], "http:///g")); // may change to http:///s//a/g
	TEST_ASSERT(testAddOrRemoveBaseHelper("//g", BASE_URI[4], "http://g")); // may change to http:///s//g
	TEST_ASSERT(testAddOrRemoveBaseHelper("//g/x", BASE_URI[4], "http://g/x")); // may change to http:///s//g/x
	TEST_ASSERT(testAddOrRemoveBaseHelper("///g", BASE_URI[4], "http:///g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./", BASE_URI[4], "http:///s//a/b/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../", BASE_URI[4], "http:///s//a/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../g", BASE_URI[4], "http:///s//a/g"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../", BASE_URI[4], "http:///s//")); // may change to http:///s//a/../
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../g", BASE_URI[4], "http:///s//g")); // may change to http:///s//a/../g
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../../g", BASE_URI[4], "http:///s/g")); // may change to http:///s//a/../../g
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../../../g", BASE_URI[4], "http:///g")); // may change to http:///s//a/../../../g

	// from Dan Connelly's tests in http://www.w3.org/2000/10/swap/uripath.py
	TEST_ASSERT(testAddOrRemoveBaseHelper("bar:abc", "foo:xyz", "bar:abc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../abc", "http://example/x/y/z", "http://example/x/abc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("http://example/x/abc", "http://example2/x/y/z", "http://example/x/abc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../r", "http://ex/x/y/z", "http://ex/x/r"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("q/r", "http://ex/x/y", "http://ex/x/q/r"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("q/r#s", "http://ex/x/y", "http://ex/x/q/r#s"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("q/r#s/t", "http://ex/x/y", "http://ex/x/q/r#s/t"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("ftp://ex/x/q/r", "http://ex/x/y", "ftp://ex/x/q/r"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("", "http://ex/x/y", "http://ex/x/y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("", "http://ex/x/y/", "http://ex/x/y/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("", "http://ex/x/y/pdq", "http://ex/x/y/pdq"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("z/", "http://ex/x/y/", "http://ex/x/y/z/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("#Animal", "file:/swap/test/animal.rdf", "file:/swap/test/animal.rdf#Animal"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../abc", "file:/e/x/y/z", "file:/e/x/abc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/example/x/abc", "file:/example2/x/y/z", "file:/example/x/abc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../r", "file:/ex/x/y/z", "file:/ex/x/r"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/r", "file:/ex/x/y/z", "file:/r"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("q/r", "file:/ex/x/y", "file:/ex/x/q/r"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("q/r#s", "file:/ex/x/y", "file:/ex/x/q/r#s"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("q/r#", "file:/ex/x/y", "file:/ex/x/q/r#"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("q/r#s/t", "file:/ex/x/y", "file:/ex/x/q/r#s/t"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("ftp://ex/x/q/r", "file:/ex/x/y", "ftp://ex/x/q/r"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("", "file:/ex/x/y", "file:/ex/x/y"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("", "file:/ex/x/y/", "file:/ex/x/y/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("", "file:/ex/x/y/pdq", "file:/ex/x/y/pdq"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("z/", "file:/ex/x/y/", "file:/ex/x/y/z/"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("file://meetings.example.com/cal#m1", "file:/devel/WWW/2000/10/swap/test/reluri-1.n3", "file://meetings.example.com/cal#m1"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("file://meetings.example.com/cal#m1", "file:/home/connolly/w3ccvs/WWW/2000/10/swap/test/reluri-1.n3", "file://meetings.example.com/cal#m1"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./#blort", "file:/some/dir/foo", "file:/some/dir/#blort"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./#", "file:/some/dir/foo", "file:/some/dir/#"));

	// Ryan Lee
	TEST_ASSERT(testAddOrRemoveBaseHelper("./", "http://example/x/abc.efg", "http://example/x/"));

	// Graham Klyne's tests
	// http://www.ninebynine.org/Software/HaskellUtils/Network/UriTest.xls
	// 01-31 are from Connelly's cases

	// 32-49
	TEST_ASSERT(testAddOrRemoveBaseHelper("./q:r", "http://ex/x/y", "http://ex/x/q:r"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./p=q:r", "http://ex/x/y", "http://ex/x/p=q:r"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("?pp/rr", "http://ex/x/y?pp/qq", "http://ex/x/y?pp/rr"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("y/z", "http://ex/x/y?pp/qq", "http://ex/x/y/z"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("local/qual@domain.org#frag", "mailto:local", "mailto:local/qual@domain.org#frag"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("more/qual2@domain2.org#frag", "mailto:local/qual1@domain1.org", "mailto:local/more/qual2@domain2.org#frag"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("y?q", "http://ex/x/y?q", "http://ex/x/y?q"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/x/y?q", "http://ex?p", "http://ex/x/y?q"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("c/d", "foo:a/b", "foo:a/c/d"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/c/d", "foo:a/b", "foo:/c/d"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("", "foo:a/b?c#d", "foo:a/b?c"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("b/c", "foo:a", "foo:b/c"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../b/c", "foo:/a/y/z", "foo:/a/b/c"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("./b/c", "foo:a", "foo:b/c"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/./b/c", "foo:a", "foo:/b/c"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../d", "foo://a//b/c", "foo://a/d"));
	TEST_ASSERT(testAddOrRemoveBaseHelper(".", "foo:a", "foo:"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("..", "foo:a", "foo:"));

	// 50-57 (cf. TimBL comments --
	// http://lists.w3.org/Archives/Public/uri/2003Feb/0028.html,
	// http://lists.w3.org/Archives/Public/uri/2003Jan/0008.html)
	TEST_ASSERT(testAddOrRemoveBaseHelper("abc", "http://example/x/y%2Fz", "http://example/x/abc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../../x%2Fabc", "http://example/a/x/y/z", "http://example/a/x%2Fabc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../x%2Fabc", "http://example/a/x/y%2Fz", "http://example/a/x%2Fabc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("abc", "http://example/x%2Fy/z", "http://example/x%2Fy/abc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("q%3Ar", "http://ex/x/y", "http://ex/x/q%3Ar"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/x%2Fabc", "http://example/x/y%2Fz", "http://example/x%2Fabc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/x%2Fabc", "http://example/x/y/z", "http://example/x%2Fabc"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("/x%2Fabc", "http://example/x/y%2Fz", "http://example/x%2Fabc"));

	// 70-77
	TEST_ASSERT(testAddOrRemoveBaseHelper("local2@domain2", "mailto:local1@domain1?query1", "mailto:local2@domain2"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("local2@domain2?query2", "mailto:local1@domain1", "mailto:local2@domain2?query2"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("local2@domain2?query2", "mailto:local1@domain1?query1", "mailto:local2@domain2?query2"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("?query2", "mailto:local@domain?query1", "mailto:local@domain?query2"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("local@domain?query2", "mailto:?query1", "mailto:local@domain?query2"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("?query2", "mailto:local@domain?query1", "mailto:local@domain?query2"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("http://example/a/b?c/../d", "foo:bar", "http://example/a/b?c/../d"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("http://example/a/b#c/../d", "foo:bar", "http://example/a/b#c/../d"));

	// 82-88
	TEST_ASSERT(testAddOrRemoveBaseHelper("http:this", "http://example.org/base/uri", "http:this"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("http:this", "http:base", "http:this"));
	// Whole in the URI spec, see http://lists.w3.org/Archives/Public/uri/2007Aug/0003.html
	// TEST_ASSERT(testAddOrRemoveBaseHelper(".//g", "f:/a", "f://g")); // ORIGINAL
	TEST_ASSERT(testAddOrRemoveBaseHelper(".//g", "f:/a", "f:/.//g")); // FIXED ONE
	TEST_ASSERT(testAddOrRemoveBaseHelper("b/c//d/e", "f://example.org/base/a", "f://example.org/base/b/c//d/e"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("m2@example.ord/c2@example.org", "mid:m@example.ord/c@example.org", "mid:m@example.ord/m2@example.ord/c2@example.org"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("mini1.xml", "file:///C:/DEV/Haskell/lib/HXmlToolbox-3.01/examples/", "file:///C:/DEV/Haskell/lib/HXmlToolbox-3.01/examples/mini1.xml"));
	TEST_ASSERT(testAddOrRemoveBaseHelper("../b/c", "foo:a/y/z", "foo:a/b/c"));
}



void FourSuite::relativize_test_cases() {
	const bool REMOVE_MODE = false;
	const bool DOMAIN_ROOT_MODE = true;

	// to convert, base, exptected

	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c", "s://ex/a/d", "b/c", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/b/b/c", "s://ex/a/d", "/b/b/c", REMOVE_MODE, DOMAIN_ROOT_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c", "s://ex/a/b/", "c", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://other.ex/a/b/", "s://ex/a/d", "//other.ex/a/b/", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c", "s://other.ex/a/d", "//ex/a/b/c", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("t://ex/a/b/c", "s://ex/a/d", "t://ex/a/b/c", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c", "t://ex/a/d", "s://ex/a/b/c", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a", "s://ex/b/c/d", "/a", REMOVE_MODE, DOMAIN_ROOT_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/b/c/d", "s://ex/a", "b/c/d", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c?h", "s://ex/a/d?w", "b/c?h", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c#h", "s://ex/a/d#w", "b/c#h", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c?h#i", "s://ex/a/d?w#j", "b/c?h#i", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a#i", "s://ex/a", "#i", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a?i", "s://ex/a", "?i", REMOVE_MODE));

	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/", "s://ex/a/b/", "", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b", "s://ex/a/b", "", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/", "s://ex/", "", REMOVE_MODE));

	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c", "s://ex/a/d/c", "../b/c", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c/", "s://ex/a/d/c", "../b/c/", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c/d", "s://ex/a/d/c/d", "../../b/c/d", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/c", "s://ex/d/e/f", "/a/b/c", REMOVE_MODE, DOMAIN_ROOT_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b/", "s://ex/a/c/d/e", "../../b/", REMOVE_MODE));

	// Some tests to ensure that empty path segments don't cause problems.
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/b", "s://ex/a//b/c", "../../b", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a///b", "s://ex/a/", ".///b", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a/", "s://ex/a///b", "../../", REMOVE_MODE));
	TEST_ASSERT(testAddOrRemoveBaseHelper("s://ex/a//b/c", "s://ex/a/b", ".//b/c", REMOVE_MODE));
}



int FourSuite::testParseUri(const char * uriText) {
	UriParserStateA state;
	UriUriA uri;
	state.uri = &uri;
	int res = uriParseUriA(&state, uriText);
	uriFreeUriMembersA(&uri);
	return res;
}



bool FourSuite::testGoodUri(const char * uriText) {
	return (testParseUri(uriText) == 0);
}



bool FourSuite::testBadUri(const char * uriText) {
	return (testParseUri(uriText) == URI_ERROR_SYNTAX);
}



void FourSuite::good_URI_references() {
	TEST_ASSERT(testGoodUri("file:///foo/bar"));
	TEST_ASSERT(testGoodUri("mailto:user@host?subject=blah"));
	TEST_ASSERT(testGoodUri("dav:")); // empty opaque part / rel-path allowed by RFC 2396bis
	TEST_ASSERT(testGoodUri("about:")); // empty opaque part / rel-path allowed by RFC 2396bis

	// the following test cases are from a Perl script by David A. Wheeler
	// at http://www.dwheeler.com/secure-programs/url.pl
	TEST_ASSERT(testGoodUri("http://www.yahoo.com"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com/"));
	TEST_ASSERT(testGoodUri("http://1.2.3.4/"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com/stuff"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com/stuff/"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com/hello%20world/"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com?name=obi"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com?name=obi+wan&status=jedi"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com?onery"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com#bottom"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com/yelp.html#bottom"));
	TEST_ASSERT(testGoodUri("https://www.yahoo.com/"));
	TEST_ASSERT(testGoodUri("ftp://www.yahoo.com/"));
	TEST_ASSERT(testGoodUri("ftp://www.yahoo.com/hello"));
	TEST_ASSERT(testGoodUri("demo.txt"));
	TEST_ASSERT(testGoodUri("demo/hello.txt"));
	TEST_ASSERT(testGoodUri("demo/hello.txt?query=hello#fragment"));
	TEST_ASSERT(testGoodUri("/cgi-bin/query?query=hello#fragment"));
	TEST_ASSERT(testGoodUri("/demo.txt"));
	TEST_ASSERT(testGoodUri("/hello/demo.txt"));
	TEST_ASSERT(testGoodUri("hello/demo.txt"));
	TEST_ASSERT(testGoodUri("/"));
	TEST_ASSERT(testGoodUri(""));
	TEST_ASSERT(testGoodUri("#"));
	TEST_ASSERT(testGoodUri("#here"));

	// Wheeler's script says these are invalid, but they aren't
	TEST_ASSERT(testGoodUri("http://www.yahoo.com?name=%00%01"));
	TEST_ASSERT(testGoodUri("http://www.yaho%6f.com"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com/hello%00world/"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com/hello+world/"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com?name=obi&"));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com?name=obi&type="));
	TEST_ASSERT(testGoodUri("http://www.yahoo.com/yelp.html#"));
	TEST_ASSERT(testGoodUri("//"));

	// the following test cases are from a Haskell program by Graham Klyne
	// at http://www.ninebynine.org/Software/HaskellUtils/Network/URITest.hs
	TEST_ASSERT(testGoodUri("http://example.org/aaa/bbb#ccc"));
	TEST_ASSERT(testGoodUri("mailto:local@domain.org"));
	TEST_ASSERT(testGoodUri("mailto:local@domain.org#frag"));
	TEST_ASSERT(testGoodUri("HTTP://EXAMPLE.ORG/AAA/BBB#CCC"));
	TEST_ASSERT(testGoodUri("//example.org/aaa/bbb#ccc"));
	TEST_ASSERT(testGoodUri("/aaa/bbb#ccc"));
	TEST_ASSERT(testGoodUri("bbb#ccc"));
	TEST_ASSERT(testGoodUri("#ccc"));
	TEST_ASSERT(testGoodUri("#"));
	TEST_ASSERT(testGoodUri("A'C"));

	// escapes
	TEST_ASSERT(testGoodUri("http://example.org/aaa%2fbbb#ccc"));
	TEST_ASSERT(testGoodUri("http://example.org/aaa%2Fbbb#ccc"));
	TEST_ASSERT(testGoodUri("%2F"));
	TEST_ASSERT(testGoodUri("aaa%2Fbbb"));

	// ports
	TEST_ASSERT(testGoodUri("http://example.org:80/aaa/bbb#ccc"));
	TEST_ASSERT(testGoodUri("http://example.org:/aaa/bbb#ccc"));
	TEST_ASSERT(testGoodUri("http://example.org./aaa/bbb#ccc"));
	TEST_ASSERT(testGoodUri("http://example.123./aaa/bbb#ccc"));

	// bare authority
	TEST_ASSERT(testGoodUri("http://example.org"));

	// IPv6 literals (from RFC2732):
	TEST_ASSERT(testGoodUri("http://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]:80/index.html"));
	TEST_ASSERT(testGoodUri("http://[1080:0:0:0:8:800:200C:417A]/index.html"));
	TEST_ASSERT(testGoodUri("http://[3ffe:2a00:100:7031::1]"));
	TEST_ASSERT(testGoodUri("http://[1080::8:800:200C:417A]/foo"));
	TEST_ASSERT(testGoodUri("http://[::192.9.5.5]/ipng"));
	TEST_ASSERT(testGoodUri("http://[::FFFF:129.144.52.38]:80/index.html"));
	TEST_ASSERT(testGoodUri("http://[2010:836B:4179::836B:4179]"));
	TEST_ASSERT(testGoodUri("//[2010:836B:4179::836B:4179]"));

	// Random other things that crop up
	TEST_ASSERT(testGoodUri("http://example/Andr&#567;"));
	TEST_ASSERT(testGoodUri("file:///C:/DEV/Haskell/lib/HXmlToolbox-3.01/examples/"));
}



void FourSuite::bad_URI_references() {
	TEST_ASSERT(testBadUri("beepbeep\x07\x07"));
	TEST_ASSERT(testBadUri("\n"));
	TEST_ASSERT(testBadUri("::")); // not OK, per Roy Fielding on the W3C uri list on 2004-04-01

	// the following test cases are from a Perl script by David A. Wheeler
	// at http://www.dwheeler.com/secure-programs/url.pl
	TEST_ASSERT(testBadUri("http://www yahoo.com"));
	TEST_ASSERT(testBadUri("http://www.yahoo.com/hello world/"));
	TEST_ASSERT(testBadUri("http://www.yahoo.com/yelp.html#\""));

	// the following test cases are from a Haskell program by Graham Klyne
	// at http://www.ninebynine.org/Software/HaskellUtils/Network/URITest.hs
	TEST_ASSERT(testBadUri("[2010:836B:4179::836B:4179]"));
	TEST_ASSERT(testBadUri(" "));
	TEST_ASSERT(testBadUri("%"));
	TEST_ASSERT(testBadUri("A%Z"));
	TEST_ASSERT(testBadUri("%ZZ"));
	TEST_ASSERT(testBadUri("%AZ"));
	TEST_ASSERT(testBadUri("A C"));
	TEST_ASSERT(testBadUri("A\\'C")); // r"A\'C"
	TEST_ASSERT(testBadUri("A`C"));
	TEST_ASSERT(testBadUri("A<C"));
	TEST_ASSERT(testBadUri("A>C"));
	TEST_ASSERT(testBadUri("A^C"));
	TEST_ASSERT(testBadUri("A\\\\C")); // r'A\\C'
	TEST_ASSERT(testBadUri("A{C"));
	TEST_ASSERT(testBadUri("A|C"));
	TEST_ASSERT(testBadUri("A}C"));
	TEST_ASSERT(testBadUri("A[C"));
	TEST_ASSERT(testBadUri("A]C"));
	TEST_ASSERT(testBadUri("A[**]C"));
	TEST_ASSERT(testBadUri("http://[xyz]/"));
	TEST_ASSERT(testBadUri("http://]/"));
	TEST_ASSERT(testBadUri("http://example.org/[2010:836B:4179::836B:4179]"));
	TEST_ASSERT(testBadUri("http://example.org/abc#[2010:836B:4179::836B:4179]"));
	TEST_ASSERT(testBadUri("http://example.org/xxx/[qwerty]#a[b]"));

	// from a post to the W3C uri list on 2004-02-17
	TEST_ASSERT(testBadUri("http://w3c.org:80path1/path2"));
}



bool FourSuite::normalizeAndCompare(const char * uriText,
		const char * expectedNormalized) {
	UriParserStateA stateA;
	int res;

	UriUriA testUri;
	stateA.uri = &testUri;
	res = uriParseUriA(&stateA, uriText);
	if (res != 0) {
		uriFreeUriMembersA(&testUri);
		return false;
	}

	// Expected result
	UriUriA expectedUri;
	stateA.uri = &expectedUri;
	res = uriParseUriA(&stateA, expectedNormalized);
	if (res != 0) {
		uriFreeUriMembersA(&testUri);
		uriFreeUriMembersA(&expectedUri);
		return false;
	}

	res = uriNormalizeSyntaxA(&testUri);
	if (res != 0) {
		uriFreeUriMembersA(&testUri);
		uriFreeUriMembersA(&expectedUri);
		return false;
	}

	const bool equalAfter = (URI_TRUE == uriEqualsUriA(&testUri, &expectedUri));
	uriFreeUriMembersA(&testUri);
	uriFreeUriMembersA(&expectedUri);
	return equalAfter;
}



void FourSuite::caseNormalizationTests() {
	TEST_ASSERT(normalizeAndCompare("HTTP://www.EXAMPLE.com/", "http://www.example.com/"));
	TEST_ASSERT(normalizeAndCompare("example://A/b/c/%7bfoo%7d", "example://a/b/c/%7Bfoo%7D"));
}



void FourSuite::pctEncNormalizationTests() {
	TEST_ASSERT(normalizeAndCompare("http://host/%7Euser/x/y/z", "http://host/~user/x/y/z"));
	TEST_ASSERT(normalizeAndCompare("http://host/%7euser/x/y/z", "http://host/~user/x/y/z"));
}



void FourSuite::pathSegmentNormalizationTests() {
	TEST_ASSERT(normalizeAndCompare("/a/b/../../c", "/c"));
	// TEST_ASSERT(normalizeAndCompare("a/b/../../c", "a/b/../../c"));
	// Fixed:
	TEST_ASSERT(normalizeAndCompare("a/b/../../c", "c"));
	TEST_ASSERT(normalizeAndCompare("/a/b/././c", "/a/b/c"));
	// TEST_ASSERT(normalizeAndCompare("a/b/././c", "a/b/././c"));
	// Fixed:
	TEST_ASSERT(normalizeAndCompare("a/b/././c", "a/b/c"));
	TEST_ASSERT(normalizeAndCompare("/a/b/../c/././d", "/a/c/d"));
	// TEST_ASSERT(normalizeAndCompare("a/b/../c/././d", "a/b/../c/././d"));
	// Fixed:
	TEST_ASSERT(normalizeAndCompare("a/b/../c/././d", "a/c/d"));
}
