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

#include "VersionSuite.h"

#ifndef _GNU_SOURCE
# define _GNU_SOURCE  // for asprintf
#endif
#include <stdio.h>


#include <config.h>  // for PACKAGE_VERSION
#include <uriparser/UriBase.h>


void VersionSuite::ensure_version_defines_in_sync() {
	char * INSIDE_VERSION = NULL;
	const int bytes_printed = asprintf(&INSIDE_VERSION, "%d.%d.%d%s",
			URI_VER_MAJOR, URI_VER_MINOR, URI_VER_RELEASE, URI_VER_SUFFIX_ANSI);
	TEST_ASSERT(bytes_printed != -1);
	printf("bytes_printed: %d\n", bytes_printed);

	const bool equal = !strcmp(INSIDE_VERSION, PACKAGE_VERSION);
	if (! equal) {
		printf("Inside/outside version mismatch detected:\n");
		printf("  Tarball version:         <%s>\n", PACKAGE_VERSION);
		printf("  Header defines version:  <%s>\n", INSIDE_VERSION);
	}
	free(INSIDE_VERSION);
	TEST_ASSERT(equal);
}
