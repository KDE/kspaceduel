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

set(kspaceduel_icons
	${CMAKE_CURRENT_SOURCE_DIR}/hi16-app-kspaceduel.png
	${CMAKE_CURRENT_SOURCE_DIR}/hi32-app-kspaceduel.png
	${CMAKE_CURRENT_SOURCE_DIR}/hi48-app-kspaceduel.png
	${CMAKE_CURRENT_SOURCE_DIR}/hi64-app-kspaceduel.png
	${CMAKE_CURRENT_SOURCE_DIR}/hi128-app-kspaceduel.png
)
kde4_add_win32_app_icon(kspaceduel_SRCS ${kspaceduel_icons})
kde4_add_executable(kspaceduel ${kspaceduel_SRCS})

target_link_libraries(kspaceduel  ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS kspaceduel  DESTINATION ${BIN_INSTALL_DIR} )
install(FILES default_theme.svgz DESTINATION ${DATA_INSTALL_DIR}/kspaceduel/sprites )


########### install files ###############

install( FILES kspaceduel.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kspaceduel.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kspaceduelui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kspaceduel )

kde4_install_icons( ${ICON_INSTALL_DIR}   )




