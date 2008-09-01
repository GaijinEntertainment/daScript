#! /usr/bin/env bash
PWD_BACKUP=${PWD}
SCRIPT_DIR=`dirname "${PWD}/$0"`
cd "${SCRIPT_DIR}/.." || exit 1
function fail() { cd "${PWD_BACKUP}" ; exit 1; }
####################################################################

gedit \
	doc/configure.in \
	include/uriparser/UriBase.h \
	configure.in \
	ChangeLog \
	&

####################################################################
cd "${PWD_BACKUP}" || fail
exit 0

