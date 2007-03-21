## Makefile.am
lib_LIBRARIES = liburiparser.a
## man_MANS = uriparser.3
nobase_include_HEADERS = ../include/uriparser.h \
                  ../include/uriparser/Uri.h \
                  ../include/uriparser/UriBase.h \
                  ../include/uriparser/UriDefsAnsi.h \
                  ../include/uriparser/UriDefsConfig.h \
                  ../include/uriparser/UriDefsUnicode.h \
                  ../include/uriparser/UriIp4.h \
                  ../include/uriparser/UriIp4Base.h
liburiparser_a_SOURCES = \
                         ../include/uriparser/Uri.h \
                         ../include/uriparser/UriBase.h \
                         ../include/uriparser/UriDefsAnsi.h \
                         ../include/uriparser/UriDefsConfig.h \
                         ../include/uriparser/UriDefsUnicode.h \
                         ../include/uriparser/UriIp4.h \
                         ../include/uriparser/UriIp4Base.h \
                         ../lib/Uri.c \
                         ../lib/UriIp4.c \
                         ../lib/uriparser.c \
                         ../lib/UriIp4Base.c  \
                         ../include/uriparser.h 

## TESTPROG=test

CFLAGS+=-I../include

## test=${TESTPROG}
