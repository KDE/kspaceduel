
# just set the variable
appsdir = $(kde_appsdir)/Games
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
EXTRA_DIST = kspaceduel.desktop kspaceduel.xpm mini-kspaceduel.xpm

# the library search path. 
kspaceduel_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
kspaceduel_LDADD   = $(LIB_KDEUI)

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = mathroutines.h topwidget.h duelwidget.h playerinfo.h \
		 sprites.h dialogs.h mainview.h defines.h ai.h structs.h version.h

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(kspaceduel_METASOURCES)

# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) $(kspaceduel_SOURCES) -o $(podir)/kspaceduel.pot

KDE_ICON = kspaceduel

apps_DATA = kspaceduel.desktop

