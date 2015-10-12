AC_PREREQ(2.61)
AC_INIT([uriparser], [0.8.4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.10.1 foreign dist-zip dist-bzip2 no-dist-gzip subdir-objects])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_HEADER_STDC


## Toggle build options specific to Windows
case "${host}" in
	*-*-mingw*) WIN32="yes" ;;
	*cygwin*) WIN32="yes" ;;
	*) WIN32="no" ;;
esac
AM_CONDITIONAL([WIN32], test "$WIN32" = "yes")


## Check for wprintf
AC_MSG_CHECKING(for wprintf)
AC_LINK_IFELSE([AC_LANG_SOURCE([
	#include <stdio.h>
	#include <wchar.h>
	int main() {
	    wprintf((wchar_t *)0, 0);
	    return 0;
	}
])],[
	AC_DEFINE([HAVE_WPRINTF],, [Define if your C runtime provides the wprintf function.])
	AC_MSG_RESULT(yes)
],[
	AC_MSG_RESULT(no)
])


## Option for URI_SIZEDOWN
AC_ARG_ENABLE(sizedown, [
==============================================================================
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
		AC_MSG_ERROR([Please install libcpptest 1.1.0 or later.
			Alternatively, you could pass --disable-test to the call to configure.
			However, it is not recommended to disable the test suite.])
	])
fi


## Local headers must come very first. Otherwise we
## risk including headers of an already installed
## uriparser version if its path is in CPPFLAGS
CPPFLAGS="-I${srcdir}/include ${CPPFLAGS}"


## Character type selection
AC_ARG_ENABLE(char, [AS_HELP_STRING(
	[--disable-char], [disable code for type char *])], [
	CHAR_YESNO=${enableval}
],[
	CHAR_YESNO=yes
])
AC_ARG_ENABLE(wchar_t, [AS_HELP_STRING(
	[--disable-wchar_t], [disable code for type wchar_t *])], [
	WCHAR_T_YESNO=${enableval}
],[
	WCHAR_T_YESNO=yes
])
if test ${CHAR_YESNO} = no -a ${WCHAR_T_YESNO} = no ; then
	AC_MSG_ERROR([Parameters --disable-char and --disable-wchar_t cannot be used together.])
elif test ${CHAR_YESNO} = no -o ${WCHAR_T_YESNO} = no ; then
	if test ${CHAR_YESNO} = no ; then
		CPPFLAGS="${CPPFLAGS} -DURI_NO_ANSI"
		troublemaker='--disable-char'
	elif test ${WCHAR_T_YESNO} = no ; then
		CPPFLAGS="${CPPFLAGS} -DURI_NO_UNICODE"
		troublemaker='--disable-wchar_t'
	fi

	if test "${URI_TEST_YESNO}" = "yes"; then
		AC_MSG_ERROR([The test suite relies on code for both char * and wchar_t *.
			Either remove ${troublemaker} or add --disable-test, please.])
	fi
fi


## API documentation
AC_ARG_ENABLE(doc, [AS_HELP_STRING(
	[--disable-doc], [disable generation of API documentation with Doxygen])
==============================================================================], [
	URI_DOC_YESNO=${enableval}
],[
	URI_DOC_YESNO=yes
])
if test ${URI_DOC_YESNO} = yes ; then
	## Doxygen
	AC_CHECK_PROG(DOXY_CHECK, doxygen, found, missing)
	if test ${DOXY_CHECK} != "found" ; then
		AC_MSG_ERROR([Please install Doxygen first.])
	fi

	## Graphviz
	AC_CHECK_PROG(GRAPHVIZ_CHECK, dot, found, missing)
	if test ${GRAPHVIZ_CHECK} != "found" ; then
		AC_MSG_ERROR([Please install Graphviz first.])
	fi

	## HTML help, Qt Compressed Help
	host_triplet=`${ac_config_guess}`
	if echo "${host_triplet}" | grep cygwin &>/dev/null ; then
		GENERATE_HTMLHELP=YES
		QHG_LOCATION=../qhelpgenerator.exe
	else
		GENERATE_HTMLHELP=NO
		QHG_LOCATION=qhelpgenerator
	fi
	AC_SUBST(GENERATE_HTMLHELP)
	AC_SUBST(QHG_LOCATION)
	AC_SUBST(ac_abs_confdir, [${ac_abs_confdir}])
fi
AM_CONDITIONAL(URI_GENERATE_DOC, test ${URI_DOC_YESNO} = yes)


AC_CONFIG_FILES([
	liburiparser.pc
	Makefile
	doc/Makefile
	doc/Doxyfile
])
AC_CONFIG_FILES([doc/release.sh], [chmod a+x doc/release.sh])
AC_CONFIG_HEADERS([test/config.h])
AC_OUTPUT

cat <<CONFIG
===========================================================================

Configuration
  Prefix ............... ${prefix}
  Size down ............ ${URI_SIZE_DOWN_YESNO}
  Code for char * ...... ${CHAR_YESNO}
  Code for wchar_t * ... ${WCHAR_T_YESNO}
  Test suite ........... ${URI_TEST_YESNO}
  Documentation ........ ${URI_DOC_YESNO}

CONFIG

cat <<"INFO"
Continue with
  make
  make check
  sudo make install

INFO
