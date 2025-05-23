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

if(APPLE)
    set_target_properties(kspaceduel PROPERTIES
	    MACOSX_BUNDLE_DISPLAY_NAME "KSpaceduel"
        MACOSX_BUNDLE_BUNDLE_NAME "KSpaceduel"
       MACOSX_BUNDLE_LONG_VERSION_STRING "KSpaceduel ${RELEASE_SERVICE_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${RELEASE_SERVICE_VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.kspaceduel"
        MACOSX_BUNDLE_COPYRIGHT "2024 The KSpaceduel Developers")
endif()

ki18n_wrap_ui(kspaceduel general.ui)

kconfig_add_kcfg_files(kspaceduel options.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_SOURCE_DIR}/icons/*-apps-kspaceduel.png")
ecm_add_app_icon(kspaceduel ICONS ${ICONS_SRCS})

target_link_libraries(kspaceduel
    KDEGames6
    KF6::CoreAddons
    KF6::Completion
    KF6::Crash
    KF6::DBusAddons
    KF6::I18n
    KF6::XmlGui
    Qt6::Svg
    Qt6::SvgWidgets
)

install(TARGETS kspaceduel  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES default_theme.svgz DESTINATION ${KDE_INSTALL_DATADIR}/kspaceduel/sprites)

install(PROGRAMS org.kde.kspaceduel.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES kspaceduel.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR})
install(FILES org.kde.kspaceduel.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
