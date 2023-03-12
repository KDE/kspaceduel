ecm_setup_version(${KSPACEDUEL_VERSION}
    VARIABLE_PREFIX KSPACEDUEL
    VERSION_HEADER kspaceduel_version.h
)

add_executable(kspaceduel)

target_sources(kspaceduel PRIVATE
    ai.cpp
    ai.h
    defines.h
    dialogs.cpp
    dialogs.h
    main.cpp
    mainview.cpp
    mainview.h
    mathroutines.cpp
    mathroutines.h
    playerinfo.cpp
    playerinfo.h
    spritebase.cpp
    spritebase.h
    sprites.cpp
    sprites.h
    structs.h
    topwidget.cpp
    topwidget.h

    kspaceduel.qrc
)

ki18n_wrap_ui(kspaceduel general.ui)

kconfig_add_kcfg_files(kspaceduel options.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_SOURCE_DIR}/icons/*-apps-kspaceduel.png")
ecm_add_app_icon(kspaceduel ICONS ${ICONS_SRCS})

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(kspaceduel KDEGames6)
else()
    target_link_libraries(kspaceduel KF5KDEGames)
endif()

target_link_libraries(kspaceduel
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::Completion
    KF${KF_MAJOR_VERSION}::Crash
    KF${KF_MAJOR_VERSION}::DBusAddons
    KF${KF_MAJOR_VERSION}::XmlGui
    Qt${QT_MAJOR_VERSION}::Svg
)

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(kspaceduel Qt6::SvgWidgets)
endif()

install(TARGETS kspaceduel  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES default_theme.svgz DESTINATION ${KDE_INSTALL_DATADIR}/kspaceduel/sprites)

install(PROGRAMS org.kde.kspaceduel.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES kspaceduel.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR})
install(FILES org.kde.kspaceduel.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
