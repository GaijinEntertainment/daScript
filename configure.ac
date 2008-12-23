AC_PREREQ(2.61)
AC_INIT([uriparser], [0.7.4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.10.1 foreign dist-zip dist-bzip2 dist-lzma subdir-objects])
AM_CONFIG_HEADER(test/CppTest/config.h)


AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_HEADER_STDC


## Check for wprintf
AC_MSG_CHECKING(for wprintf)
AC_LINK_IFELSE([
	#include <stdio.h>
	#include <wchar.h>
	int main() {
	    wprintf((wchar_t *)0, 0);
	    return 0;
	}
],[
	AC_DEFINE([HAVE_WPRINTF],, [Define if your CRT provides the wprintf function.])
	AC_MSG_RESULT(yes)
],[
	AC_MSG_RESULT(no)
])


## Option for URI_SIZEDOWN
AC_ARG_ENABLE(sizedown, [
Extra options:
  --enable-sizedown       save space whereever possible (resulting in slower code)], [
	CPPFLAGS="${CPPFLAGS} -DURI_SIZEDOWN"
], [])


## Checks for and from CppTest
AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_FUNCS([gettimeofday])
AC_MSG_CHECKING(for round)
AC_TRY_COMPILE(
	[ #include <math.h> ],
	[ double d = round(1.0); ],
	[ round=yes ],[ round=no ])
AC_MSG_RESULT($round)
if test "$round" = "yes"; then
	AC_DEFINE([HAVE_ROUND],,[Defined if the function exists])
fi


## Local headers must come very first. Otherwise we
## risk including headers of an already installed
## uriparser version if its path is in CPPFLAGS
CPPFLAGS="-I${srcdir}/include ${CPPFLAGS}"


## API documentation
URI_GENERATE_DOC=0
AC_ARG_ENABLE(doc, [
  --enable-doc            generate API documentation with Doxygen (off by default)], [
	AC_CONFIG_SUBDIRS([doc])
	URI_GENERATE_DOC=1
], [])
AM_CONDITIONAL(URI_GENERATE_DOC, test x${URI_GENERATE_DOC} = x1)


AC_CONFIG_FILES([
	Makefile
	doc/Makefile
])
AC_OUTPUT
