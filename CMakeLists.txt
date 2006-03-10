kde4_header()

add_subdirectory( sprites ) 
add_subdirectory( pics ) 

include_directories( ${CMAKE_SOURCE_DIR}/libkdegames ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kspaceduel_SRCS 
   mathroutines.cpp 
   topwidget.cpp 
   playerinfo.cpp 
   sprites.cpp 
   mainview.cpp 
   dialogs.cpp 
   ai.cpp 
   main.cpp )

kde4_automoc(${kspaceduel_SRCS})

kde4_add_ui3_files(kspaceduel_SRCS general.ui )

kde4_add_kcfg_files(kspaceduel_SRCS options.kcfgc )

kde4_add_executable(kspaceduel ${kspaceduel_SRCS})

target_link_libraries(kspaceduel  ${LIB_KDEUI} kdegames)

install_targets(/bin kspaceduel )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kspaceduel.desktop )
install_files( ${KCFG_INSTALL_DIR} FILES kspaceduel.kcfg )
install_files( ${DATA_INSTALL_DIR}/kspaceduel FILES kspaceduelui.rc )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )

kde4_footer()



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
