## Makefile.am
AUTOMAKE_OPTIONS = dist-zip dist-bzip2

lib_LIBRARIES = liburiparser.a
noinst_PROGRAMS = test/uriparser_test
TESTS = test/uriparser_test

## man_MANS = uriparser.3



include_uriparserdir = $(prefix)/include/uriparser
include_uriparser_HEADERS = \
	include/uriparser/Uri.h \
	include/uriparser/UriBase.h \
	include/uriparser/UriDefsAnsi.h \
	include/uriparser/UriDefsConfig.h \
	include/uriparser/UriDefsUnicode.h \
	include/uriparser/UriIp4.h \
	include/uriparser/UriIp4Base.h



includedir = $(prefix)/include

include_HEADERS = \
	include/uriparser.h



liburiparser_a_CFLAGS = -Iinclude

liburiparser_a_SOURCES = \
	lib/Uri.c \
	lib/UriBase.c \
	lib/UriIp4.c \
	lib/UriIp4Base.c \
	lib/uriparser.c



test_uriparser_test_CPPFLAGS = -Iinclude
test_uriparser_test_LDADD = -luriparser
test_uriparser_test_LDFLAGS = -L.

test_uriparser_test_SOURCES = \
	test/CppTest/collectoroutput.cpp \
	test/CppTest/compileroutput.cpp \
	test/CppTest/htmloutput.cpp \
	test/CppTest/missing.cpp \
	test/CppTest/source.cpp \
	test/CppTest/suite.cpp \
	test/CppTest/textoutput.cpp \
	test/CppTest/time.cpp \
	test/CppTest/utils.cpp \
	test/test.cpp



EXTRA_DIST = \
	test/CppTest/COPYING
	test/CppTest/cpptest-assert.h \
	test/CppTest/cpptest-collectoroutput.h \
	test/CppTest/cpptest-compileroutput.h \
	test/CppTest/cpptest.h \
	test/CppTest/cpptest-htmloutput.h \
	test/CppTest/cpptest-output.h \
	test/CppTest/cpptest-source.h \
	test/CppTest/cpptest-suite.h \
	test/CppTest/cpptest-textoutput.h \
	test/CppTest/cpptest-time.h \
	test/CppTest/missing.h \
	test/CppTest/utils.h \
	test/CppTest/winconfig.h
