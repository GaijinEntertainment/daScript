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

EXTRA_DIST = \
	Doxyfile \
	\
	Mainpage \
	\
	html/*.css \
	html/*.gif \
	html/*.html \
	html/*.png
