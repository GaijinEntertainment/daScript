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

bin_PROGRAMS = uriparse
uriparse_SOURCES = tool/uriparse.c

uriparse_CFLAGS = -Iinclude
uriparse_LDADD = $(top_builddir)/liburiparser.la
uriparse_DEPENDENCIES = $(top_builddir)/liburiparser.la


pkgconfig_DATA = liburiparser.pc

pkgconfigdir = $(libdir)/pkgconfig


pkginclude_HEADERS = \
	include/uriparser/Uri.h \
	include/uriparser/UriBase.h \
	include/uriparser/UriDefsAnsi.h \
	include/uriparser/UriDefsConfig.h \
	include/uriparser/UriDefsUnicode.h \
	include/uriparser/UriIp4.h


liburiparser_la_LDFLAGS = -version-info 1:20:0
if WIN32
liburiparser_la_LDFLAGS += -no-undefined
endif

liburiparser_la_SOURCES = \
	src/UriCommon.c \
	src/UriCommon.h \
	src/UriCompare.c \
	src/UriEscape.c \
	src/UriFile.c \
	src/UriIp4.c \
	src/UriIp4Base.c \
	src/UriIp4Base.h \
	src/UriNormalize.c \
	src/UriNormalizeBase.c \
	src/UriNormalizeBase.h \
	src/UriParse.c \
	src/UriParseBase.c \
	src/UriParseBase.h \
	src/UriQuery.c \
	src/UriRecompose.c \
	src/UriResolve.c \
	src/UriShorten.c


if URI_TEST_ENABLED
test_uriparser_test_LDFLAGS = @CPPTEST_LIBS@

test_uriparser_test_CFLAGS = @CPPTEST_CFLAGS@

test_uriparser_test_LDADD = liburiparser.la

test_uriparser_test_SOURCES = \
	test/FourSuite.cpp \
	test/FourSuite.h \
	test/VersionSuite.cpp \
	test/VersionSuite.h \
	test/test.cpp
endif


EXTRA_DIST = \
	doc/Doxyfile.in \
	doc/Mainpage \
	doc/Makefile.am \
	doc/Makefile.in \
	doc/preprocess.sh \
	doc/release.sh.in \
	doc/rule_dependencies.dot \
	doc/rule_dependencies.sh \
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
