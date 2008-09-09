# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.61)
AC_INIT(Doxyfile.in)
AC_CONFIG_AUX_DIR([../build-aux])
AM_INIT_AUTOMAKE(uriparser, 0.7.3)


## Nested configure hacks
CALLED_BY_PARENT=0
AC_ARG_ENABLE(doc, [], [
	CALLED_BY_PARENT=1
], [])
AM_CONDITIONAL(CALLED_BY_PARENT, test x${CALLED_BY_PARENT} = x1)


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


## HTML help
host_triplet=`${ac_config_guess}`
if grep cygwin <<<"${host_triplet}" >/dev/null ; then
	GENERATE_HTMLHELP=YES
else
	GENERATE_HTMLHELP=NO
fi
AC_SUBST(GENERATE_HTMLHELP)


AC_CONFIG_FILES([
	Doxyfile
	Makefile
])
AC_OUTPUT
