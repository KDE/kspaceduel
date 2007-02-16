
add_subdirectory(playerinfo)

########### install files ###############

install( FILES backgr.png  DESTINATION  ${DATA_INSTALL_DIR}/kspaceduel/sprites )




#original Makefile.am contents follow:

#SUBDIRS = ship1 ship2 playerinfo explosion sun powerups
#
#bgdir = $(kde_datadir)/kspaceduel/sprites
#bg_DATA = backgr.png
#
#EXTRA_DIST = $(bg_DATA)
