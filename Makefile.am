appsdir = $(kde_appsdir)/Games/Arcade
INCLUDES= $(all_includes)

SUBDIRS = sprites pics

# you can add here more. This one gets installed 
bin_PROGRAMS = 	kspaceduel

# Which sources should be compiled for kspaceduel.
kspaceduel_SOURCES = mathroutines.cpp topwidget.cpp duelwidget.cpp \
	playerinfo.cpp sprites.cpp mainview.cpp dialogs.cpp ai.cpp main.cpp

kspaceduel_METASOURCES = AUTO
EXTRA_DIST = kspaceduel.desktop kspaceduel.xpm mini-kspaceduel.xpm

kspaceduel_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kspaceduel_LDADD   = $(LIB_KDEUI)

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = mathroutines.h topwidget.h duelwidget.h playerinfo.h \
		 sprites.h dialogs.h mainview.h defines.h ai.h structs.h \
		 version.h

DISTCLEANFILES = $(kspaceduel_METASOURCES)

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kspaceduel.pot

KDE_ICON = kspaceduel

apps_DATA = kspaceduel.desktop

rcdir=$(kde_datadir)/kspaceduel
rc_DATA = kspaceduelui.rc

install-data-local:
	rm -rf $(kde_appsdir)/Games/kspaceduel.desktop
