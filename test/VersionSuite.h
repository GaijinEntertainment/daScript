/*
 * uriparser - RFC 3986 URI parsing library
 *
 * Copyright (C) 2014, Sebastian Pipping <webmaster@hartwork.org>
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

#ifndef TEST_VERSION_TEST_H
#define TEST_VERSION_TEST_H 1



#include <cpptest.h>

using namespace Test;



class VersionSuite : public Suite {

public:
	VersionSuite() {
		TEST_ADD(VersionSuite::ensure_version_defines_in_sync)
	}

private:
	void ensure_version_defines_in_sync();

};



#endif // TEST_VERSION_TEST_H
