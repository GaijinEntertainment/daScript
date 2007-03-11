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

extern "C" {
	// Including C header from C++
	#include <uriparser/Uri.h>
}

#include "CppTest/cpptest.h"
#include <memory>
using namespace Test;
using namespace std;



class UriSuite : public Suite {

public:
	UriSuite() {
		TEST_ADD(UriSuite::test)
	}

private:
	void test() {
		UriParser parser = { 0 };
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&parser, "http://www.example.com/"));
		TEST_ASSERT(URI_SUCCESS == uriParseUriW(&parser, L"http://www.example.com/"));

		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&parser, "http://sourceforge.net/projects/uriparser/"));
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&parser, "http://sourceforge.net/project/platformdownload.php?group_id=182840"));
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&parser, "mailto:test@example.com"));
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&parser, "../../"));
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&parser, "file:///bin/bash"));
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&parser, "http://www.example.com/name%20with%20spaces/"));

		TEST_ASSERT(URI_ERROR == uriParseUriA(&parser, "http://www.example.com/name with spaces/"));
	}

};



int main() {
	Suite suite;
	suite.add(auto_ptr<Suite>(new UriSuite()));
    TextOutput output(TextOutput::Verbose);
    return suite.run(output, false) ? 0 : 1;
}
