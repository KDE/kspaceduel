INCLUDES= -I$(top_srcdir)/libkdegames $(all_includes)

SUBDIRS = sprites pics

# you can add here more. This one gets installed 
bin_PROGRAMS = 	kspaceduel

# Which sources should be compiled for kspaceduel.
kspaceduel_SOURCES = general.ui mathroutines.cpp topwidget.cpp \
	playerinfo.cpp sprites.cpp mainview.cpp dialogs.cpp ai.cpp \
	main.cpp options.kcfgc

kspaceduel_METASOURCES = AUTO
EXTRA_DIST = kspaceduel.desktop kspaceduel.xpm mini-kspaceduel.xpm

kspaceduel_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kspaceduel_LDADD   = $(LIB_KDEGAMES)
kspaceduel_DEPENDENCIES = $(LIB_KDEGAMES_DEP)

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = mathroutines.h topwidget.h playerinfo.h \
		 sprites.h dialogs.h mainview.h defines.h ai.h structs.h \
		 version.h

DISTCLEANFILES = $(kspaceduel_METASOURCES)

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kspaceduel.pot

KDE_ICON = kspaceduel

xdg_apps_DATA = kspaceduel.desktop
kde_kcfg_DATA = kspaceduel.kcfg

rcdir=$(kde_datadir)/kspaceduel
rc_DATA = kspaceduelui.rc
