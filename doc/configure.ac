# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.61)
AC_INIT(Doxyfile.in)
AC_CONFIG_AUX_DIR([../build-aux])
AM_INIT_AUTOMAKE(uriparser-doc, 0.7.5)


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


AC_CONFIG_FILES([
	Doxyfile
	Makefile
])
AC_CONFIG_FILES([release.sh], [chmod a+x release.sh])
AC_OUTPUT
