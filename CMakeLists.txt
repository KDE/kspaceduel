project(kspaceduel)

if(NOT COMPILING_KDEGAMES)
	find_package(KDE4 REQUIRED)
	include(KDE4Defaults)
	include(MacroLibrary)
	find_package(KDEGames REQUIRED)

	add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
	add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
	include_directories(${QDBUS_INCLUDE_DIRS} ${KDE4_INCLUDES} ${KDEGAMES_INCLUDE_DIRS})
endif(NOT COMPILING_KDEGAMES)

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

kde4_add_app_icon(kspaceduel_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-kspaceduel.png")
kde4_add_executable(kspaceduel ${kspaceduel_SRCS})

target_link_libraries(kspaceduel  ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS kspaceduel  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install(FILES default_theme.svgz DESTINATION ${DATA_INSTALL_DIR}/kspaceduel/sprites )


########### install files ###############

install( PROGRAMS kspaceduel.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kspaceduel.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kspaceduelui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kspaceduel )

kde4_install_icons( ${ICON_INSTALL_DIR}   )





