ecm_setup_version(${KSPACEDUEL_VERSION}
    VARIABLE_PREFIX KSPACEDUEL
    VERSION_HEADER kspaceduel_version.h
)

add_executable(kspaceduel)

target_sources(kspaceduel PRIVATE
    ai.cpp
    dialogs.cpp
    main.cpp
    mainview.cpp
    mathroutines.cpp
    playerinfo.cpp
    sprites.cpp
    spritebase.cpp
    topwidget.cpp

    kspaceduel.qrc
)

ki18n_wrap_ui(kspaceduel general.ui)

kconfig_add_kcfg_files(kspaceduel options.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_SOURCE_DIR}/icons/*-apps-kspaceduel.png")
ecm_add_app_icon(kspaceduel ICONS ${ICONS_SRCS})

target_link_libraries(kspaceduel
    KF5KDEGames
    KF5::Completion
    KF5::Crash
    KF5::DBusAddons
    KF5::XmlGui
    Qt::Svg
)

install(TARGETS kspaceduel  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES default_theme.svgz DESTINATION ${KDE_INSTALL_DATADIR}/kspaceduel/sprites)

install(PROGRAMS org.kde.kspaceduel.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES kspaceduel.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR})
install(FILES org.kde.kspaceduel.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
