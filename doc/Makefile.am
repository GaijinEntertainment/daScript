## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = 1.10 foreign dist-zip no-dist-gzip


noinst_SCRIPTS = phony_html_doc

phony_html_doc :
	rm -f html/*.css
	rm -f html/*.gif
	rm -f html/*.html
	rm -f html/*.png
	rm -f html/*.map
	rm -f html/*.md5
	rm -f html/graph_legend.dot
	rm -f html/index.*
	doxygen Doxyfile

URI_DOC_FILES = \
	html/*.css \
	html/*.gif \
	html/*.html \
	html/*.png


## Nested configure hacks
if CALLED_BY_PARENT

## Make doc get installed
nobase_nodist_doc_DATA = $(URI_DOC_FILES)

## Kill automake error
DIST_COMMON = $(DIST_COMMON)

else

## Make Automake not ship package sources
DIST_COMMON =

## Make doc go into dist when run from doc folder
EXTRA_DIST = $(URI_DOC_FILES)

endif
