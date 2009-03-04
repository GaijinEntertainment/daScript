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

#ifndef TEST_FOUR_SUITE_H
#define TEST_FOUR_SUITE_H 1



#include <cpptest.h>

using namespace Test;



class FourSuite : public Suite {

public:
	FourSuite() {
		TEST_ADD(FourSuite::absolutize_test_cases)
		TEST_ADD(FourSuite::relativize_test_cases)
		TEST_ADD(FourSuite::good_URI_references)
		TEST_ADD(FourSuite::bad_URI_references)
		TEST_ADD(FourSuite::caseNormalizationTests)
		TEST_ADD(FourSuite::pctEncNormalizationTests)
		TEST_ADD(FourSuite::pathSegmentNormalizationTests)
	}

private:
	bool testAddOrRemoveBaseHelper(const char * ref,
			const char * base, const char * expected, bool add = true,
			bool domainRootMode = false);

	void absolutize_test_cases();
	void relativize_test_cases();

	int testParseUri(const char * uriText);
	bool testGoodUri(const char * uriText);
	bool testBadUri(const char * uriText);
	void good_URI_references();
	void bad_URI_references();

	bool normalizeAndCompare(const char * uriText,
		const char * expectedNormalized);
	void caseNormalizationTests();
	void pctEncNormalizationTests();
	void pathSegmentNormalizationTests();

};



#endif // TEST_FOUR_SUITE_H
