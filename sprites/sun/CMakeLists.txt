kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kspaceduel/sprites/sun FILES sun.ppm sun.pbm )

kde4_footer()



#original Makefile.am contents follow:

#sundir = $(kde_datadir)/kspaceduel/sprites/sun
#sun_DATA = sun.ppm sun.pbm
#EXTRA_DIST = $(sun_DATA)
