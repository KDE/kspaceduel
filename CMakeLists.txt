project(kspaceduel)

add_subdirectory( sprites ) 
add_subdirectory( pics ) 

########### next target ###############

set(kspaceduel_SRCS
   ai.cpp
   dialogs.cpp
   main.cpp
   mainview.cpp
   mathroutines.cpp
   playerinfo.cpp
   sprites.cpp
   spritebase.cpp
   topwidget.cpp)

kde4_add_ui_files(kspaceduel_SRCS general.ui )

kde4_add_kcfg_files(kspaceduel_SRCS options.kcfgc )

kde4_add_executable(kspaceduel ${kspaceduel_SRCS})

target_link_libraries(kspaceduel  ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS kspaceduel  DESTINATION ${BIN_INSTALL_DIR} )
install(FILES default_theme.svgz DESTINATION ${DATA_INSTALL_DIR}/kspaceduel/sprites )


########### install files ###############

install( FILES kspaceduel.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kspaceduel.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kspaceduelui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kspaceduel )

kde4_install_icons( ${ICON_INSTALL_DIR}   )




#original Makefile.am contents follow:

#INCLUDES= -I$(top_srcdir)/libkdegames $(all_includes)
#
#SUBDIRS = sprites pics
#
## you can add here more. This one gets installed 
#bin_PROGRAMS = 	kspaceduel
#
## Which sources should be compiled for kspaceduel.
#kspaceduel_SOURCES = general.ui mathroutines.cpp topwidget.cpp \
#	playerinfo.cpp sprites.cpp mainview.cpp dialogs.cpp ai.cpp \
#	main.cpp options.kcfgc
#
#kspaceduel_METASOURCES = AUTO
#EXTRA_DIST = kspaceduel.desktop kspaceduel.xpm mini-kspaceduel.xpm
#
#kspaceduel_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#kspaceduel_LDADD   = $(LIB_KDEGAMES)
#kspaceduel_DEPENDENCIES = $(LIB_KDEGAMES_DEP)
#
## this option you can leave out. Just, if you use "make dist", you need it
#noinst_HEADERS = mathroutines.h topwidget.h playerinfo.h \
#		 sprites.h dialogs.h mainview.h defines.h ai.h structs.h \
#		 version.h
#
#DISTCLEANFILES = $(kspaceduel_METASOURCES)
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kspaceduel.pot
#
#KDE_ICON = kspaceduel
#
#xdg_apps_DATA = kspaceduel.desktop
#kde_kcfg_DATA = kspaceduel.kcfg
#
#rcdir=$(kde_datadir)/kspaceduel
#rc_DATA = kspaceduelui.rc
