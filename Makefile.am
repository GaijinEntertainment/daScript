## Makefile.am
if URI_GENERATE_DOC
SUBDIRS = . doc
endif

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = liburiparser.la

if URI_TEST_ENABLED
check_PROGRAMS = test/uriparser_test
TESTS = test/uriparser_test
endif


pkgconfig_DATA = liburiparser.pc

pkgconfigdir = $(libdir)/pkgconfig


pkginclude_HEADERS = \
	include/uriparser/Uri.h \
	include/uriparser/UriBase.h \
	include/uriparser/UriDefsAnsi.h \
	include/uriparser/UriDefsConfig.h \
	include/uriparser/UriDefsUnicode.h \
	include/uriparser/UriIp4.h


liburiparser_la_LDFLAGS = -version-info 1:10:0

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


if URI_TEST_ENABLED
test_uriparser_test_LDFLAGS = @CPPTEST_LIBS@

test_uriparser_test_CFLAGS = @CPPTEST_CFLAGS@

test_uriparser_test_LDADD = liburiparser.la

test_uriparser_test_SOURCES = \
	test/FourSuite.cpp \
	test/FourSuite.h \
	test/test.cpp
endif


## NOTE: Adding an empty doc/m4/pkg.m4 file
##   is done to workaround ACLOCAL_AMFLAGS
##   behaviour, that might be a bug or not.
EXTRA_DIST = \
	doc/aclocal.m4 \
	doc/configure \
	doc/configure.ac \
	doc/Doxyfile.in \
	doc/Mainpage \
	doc/Makefile.am \
	doc/Makefile.in \
	doc/preprocess.sh \
	doc/release.sh.in \
	doc/rfc1866.htm \
	doc/rfc3513.htm \
	doc/rfc3986.htm \
	doc/rfc3986_grammar_only.txt \
	doc/rule_dependencies.dot \
	doc/rule_dependencies.sh \
	\
	doc/m4/pkg.m4 \
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
