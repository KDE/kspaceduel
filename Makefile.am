# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_sounddir      Where system sounds should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# just set the variable
APPSDIR = $(kde_appsdir)/Games
# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
# if you don't have subdirectories, comment it
SUBDIRS = sprites  

# you can add here more. This one gets installed 
bin_PROGRAMS = 	kspaceduel

# Which sources should be compiled for kspaceduel.
kspaceduel_SOURCES = mathroutines.cpp topwidget.cpp duelwidget.cpp playerinfo.cpp \
                sprites.cpp mainview.cpp dialogs.cpp ai.cpp main.cpp

kspaceduel_METASOURCES = USE_AUTOMOC
EXTRA_DIST = kspaceduel.kdelnk kspaceduel.xpm mini-kspaceduel.xpm

# the library search path. 
kspaceduel_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
kspaceduel_LDADD   = -lQwSpriteField $(LIB_KDEUI)

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = mathroutines.h topwidget.h duelwidget.h playerinfo.h \
		 sprites.h dialogs.h mainview.h defines.h ai.h structs.h version.h

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(kspaceduel_METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(kspaceduel_SOURCES) && mv messages.po ../po/kspaceduel.pot

# just install datas here. Use install-exec-local for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) $(srcdir)/kspaceduel.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) $(srcdir)/kspaceduel.xpm $(kde_icondir)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) $(srcdir)/mini-kspaceduel.xpm $(kde_minidir)/kspaceduel.xpm

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(APPSDIR)/kspaceduel.kdelnk
	-rm -f $(kde_icondir)/kspaceduel.xpm
	-rm -f $(kde_minidir)/kspaceduel.xpm
