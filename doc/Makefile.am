## Build doc files
all-local: html/index.html

html/index.html: $(srcdir)/../include/uriparser/*.h \
		$(srcdir)/../src/*.c \
		$(srcdir)/../src/*.h \
		$(srcdir)/Mainpage
	rm -Rf "$(builddir)/html"
	doxygen Doxyfile
	touch html/index.html


## Clean doc files
clean-local:
	rm -Rf "$(builddir)/html"
	rm -f *.qch


## Clean configure files
distclean-local:
	rm -f config.{log,status} release.sh Doxyfile


## Install doc files
install-data-local:
	$(MKDIR_P) "$(DESTDIR)$(docdir)/html/search"  ## Didn't work with installdirs-local
	$(INSTALL_DATA) html/*.css html/*.html html/*.js html/*.map html/*.md5 html/*.png "$(DESTDIR)$(docdir)/html/"
	$(INSTALL_DATA) html/search/* "$(DESTDIR)$(docdir)/html/search/"
	-$(INSTALL_DATA) *.qch "$(DESTDIR)$(docdir)/"


## Uninstall doc files
uninstall-local:
	rm -Rf "$(DESTDIR)$(docdir)/html"
	rm -f "$(DESTDIR)$(docdir)"/*.qch
