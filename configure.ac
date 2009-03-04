AC_PREREQ(2.61)
AC_INIT([uriparser], [0.7.5])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.10.1 foreign dist-zip dist-bzip2 dist-lzma subdir-objects])


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
===========================================================================
Extra options:
AS_HELP_STRING(
	[--enable-sizedown], [save space whereever possible])
AS_HELP_STRING(
	[], [(resulting in slower code)])], [
	URI_SIZE_DOWN_YESNO=${enableval}
	if test ${enableval} = yes ; then
		CPPFLAGS="${CPPFLAGS} -DURI_SIZEDOWN"
	fi
],[
	URI_SIZE_DOWN_YESNO=no
])


## Option --disable-test
AC_ARG_ENABLE(test, [AS_HELP_STRING(
	[--disable-test], [disable 'make check' and libcpptest dependency]
)], [
	URI_TEST_YESNO=${enableval}
],[
	URI_TEST_YESNO=yes
])
AM_CONDITIONAL([URI_TEST_ENABLED], [test "${URI_TEST_YESNO}" = "yes"])


## Check for CppTest
if test "${URI_TEST_YESNO}" = "yes"; then
	PKG_CHECK_MODULES([CPPTEST], [
		libcpptest >= 1.1.0
	], [], [
		AC_MSG_ERROR(Please install libcpptest 1.1.0 or later.)
	])
fi


## Local headers must come very first. Otherwise we
## risk including headers of an already installed
## uriparser version if its path is in CPPFLAGS
CPPFLAGS="-I${srcdir}/include ${CPPFLAGS}"


## API documentation
AC_ARG_ENABLE(doc, [AS_HELP_STRING(
	[--enable-doc], [generate API documentation with Doxygen])
===========================================================================], [
	URI_DOC_YESNO=${enableval}
	if test ${enableval} = yes ; then
		AC_CONFIG_SUBDIRS([doc])
	fi
],[
	URI_DOC_YESNO=yes
])
AM_CONDITIONAL(URI_GENERATE_DOC, test ${URI_DOC_YESNO} = yes)


AC_CONFIG_FILES([
	liburiparser.pc
	Makefile
	doc/Makefile
])
AC_OUTPUT

cat <<CONFIG
===========================================================================

Configuration
  Prefix ........... ${prefix}
  Size down ........ ${URI_SIZE_DOWN_YESNO}
  Test suite ....... ${URI_TEST_YESNO}
  Documentation .... ${URI_DOC_YESNO}

CONFIG

cat <<"INFO"
Continue with
  make
  make check
  sudo make install

INFO
