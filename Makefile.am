## Makefile.am
AUTOMAKE_OPTIONS = dist-zip dist-bzip2

lib_LIBRARIES = liburiparser.a
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

liburiparser_a_SOURCES = \
	lib/Uri.c \
	lib/UriIp4.c \
	lib/UriIp4Base.c
	lib/uriparser.c

## TESTPROG=test

CFLAGS+=-Iinclude

## test=${TESTPROG}
