## Makefile.am
lib_LTLIBRARIES = liburiparser.la
check_PROGRAMS = test/uriparser_test
TESTS = test/uriparser_test

## man_MANS = uriparser.3



include_uriparserdir = $(prefix)/include/uriparser

include_uriparser_HEADERS = \
	include/uriparser/Uri.h \
	include/uriparser/UriBase.h \
	include/uriparser/UriDefsAnsi.h \
	include/uriparser/UriDefsConfig.h \
	include/uriparser/UriDefsUnicode.h \
	include/uriparser/UriIp4.h



liburiparser_la_LDFLAGS = -version-info 1:5:0

liburiparser_la_SOURCES = \
	lib/UriCommon.c \
	lib/UriCommon.h \
	lib/UriCompare.c \
	lib/UriEscape.c \
	lib/UriFile.c \
	lib/UriIp4.c \
	lib/UriIp4Base.c \
	lib/UriIp4Base.h \
	lib/UriNormalize.c \
	lib/UriNormalizeBase.c \
	lib/UriNormalizeBase.h \
	lib/UriParse.c \
	lib/UriParseBase.c \
	lib/UriParseBase.h \
	lib/UriQuery.c \
	lib/UriRecompose.c \
	lib/UriResolve.c \
	lib/UriShorten.c



test_uriparser_test_LDADD = liburiparser.la

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
	\
	test/FourSuite.cpp \
	test/FourSuite.h \
	test/test.cpp



EXTRA_DIST = \
	doc/aclocal.m4 \
	doc/configure \
	doc/configure.in \
	doc/Doxyfile.in \
	doc/Mainpage \
	doc/Makefile.am \
	doc/Makefile.in \
	doc/preprocess.sh \
	\
	doc/rfc1866.htm \
	doc/rfc3513.htm \
	doc/rfc3986.htm \
	doc/rfc3986_grammar_only.txt \
	doc/rule_dependencies.dot \
	doc/rule_dependencies.sh \
	\
	test/CppTest/COPYING \
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
	test/CppTest/winconfig.h \
	\
	win32/Code_Blocks/test.cbp \
	win32/Code_Blocks/uriparser.cbp \
	win32/Code_Blocks/uriparser.workspace \
	\
	win32/MinGW/Makefile \
	win32/MinGW/Readme.txt \
	\
	win32/Visual_Studio_2005/test.vcproj \
	win32/Visual_Studio_2005/uriparser.sln \
	win32/Visual_Studio_2005/uriparser.vcproj

