doc_DATA = index.html index-1.html index-2.html index-3.html index-4.html\
           index-5.html logotp3.gif

docdir=$(kde_htmldir)/en/kspaceduel

EXTRA_DIST = $(doc_DATA) index.sgml index.nif

doc: index.sgml index.nif
	ksgml2html index en
