kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kspaceduel/sprites/powerups FILES pbullet.pbm pbullet.ppm penergy.pbm penergy.ppm pmine.pbm pmine.ppm  pshield.pbm pshield.ppm )

kde4_footer()



#original Makefile.am contents follow:

#powerdir = $(kde_datadir)/kspaceduel/sprites/powerups
#power_DATA = \
#pbullet.pbm pbullet.ppm penergy.pbm penergy.ppm pmine.pbm pmine.ppm \
#pshield.pbm pshield.ppm
#EXTRA_DIST = $(power_DATA)
