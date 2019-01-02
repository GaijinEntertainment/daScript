AC_PREREQ(2.61)
AC_INIT([uriparser], [0.9.1], [https://github.com/uriparser/uriparser/issues],
		[uriparser], [https://uriparser.github.io/])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.10.1 foreign dist-zip dist-bzip2 no-dist-gzip subdir-objects])

AC_CONFIG_MACRO_DIR([m4])

AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AC_PROG_CXX
LT_INIT
AC_HEADER_STDC


## Toggle build options specific to Windows
case "${host}" in
	*-*-mingw*) WIN32="yes" ;;
	*cygwin*) WIN32="yes" ;;
	*) WIN32="no" ;;
esac
AM_CONDITIONAL([WIN32], test "$WIN32" = "yes")


## Check for functions
AC_CHECK_FUNCS([wprintf])
AC_CHECK_FUNCS([reallocarray])  # needs AC_USE_SYSTEM_EXTENSIONS (#define _GNU_SOURCE)


## Option --disable-test
AC_ARG_ENABLE(test, [
==============================================================================
Extra options:
AS_HELP_STRING(
	[--disable-test], [disable 'make check' and libgtest dependency]
)], [
	URI_TEST_YESNO=${enableval}
],[
	URI_TEST_YESNO=yes
])
AM_CONDITIONAL([URI_TEST_ENABLED], [test "${URI_TEST_YESNO}" = "yes"])


## Check for Googletest
if test "${URI_TEST_YESNO}" = "yes"; then
	PKG_CHECK_MODULES([GTEST], [
		gtest >= 1.8.1
	], [], [
		AC_MSG_ERROR([Please install Googletest 1.8.1 or later.
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
AC_CONFIG_HEADERS([config.h])
AC_OUTPUT

cat <<INFO
===========================================================================

Configuration
  Prefix ............... ${prefix}
  Code for char * ...... ${CHAR_YESNO}
  Code for wchar_t * ... ${WCHAR_T_YESNO}
  Test suite ........... ${URI_TEST_YESNO}
  Documentation ........ ${URI_DOC_YESNO}

Continue with
  make
  make check
  sudo make install

INFO
