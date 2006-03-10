kde4_header()

add_subdirectory( ship1 ) 
add_subdirectory( ship2 ) 
add_subdirectory( playerinfo ) 
add_subdirectory( explosion ) 
add_subdirectory( sun ) 
add_subdirectory( powerups ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kspaceduel/sprites FILES backgr.png )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = ship1 ship2 playerinfo explosion sun powerups
#
#bgdir = $(kde_datadir)/kspaceduel/sprites
#bg_DATA = backgr.png
#
#EXTRA_DIST = $(bg_DATA)
