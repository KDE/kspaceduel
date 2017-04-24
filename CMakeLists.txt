project(kspaceduel)
cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.3.0")
set (KF5_MIN_VERSION "5.3.0")

find_package(ECM 1.7.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Svg)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Completion
    Config
    ConfigWidgets
    CoreAddons
    Crash
    DBusAddons
    DocTools
    I18n
    XmlGui
)

find_package(KF5KDEGames 4.9.0 REQUIRED)

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_subdirectory(sprites)
add_subdirectory(pics)
add_subdirectory(doc)

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
   topwidget.cpp
)

ki18n_wrap_ui(kspaceduel_SRCS general.ui)

kconfig_add_kcfg_files(kspaceduel_SRCS options.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*-apps-kspaceduel.png")
ecm_add_app_icon(kspaceduel_SRCS ICONS ${ICONS_SRCS})
add_executable(kspaceduel ${kspaceduel_SRCS})

target_link_libraries(kspaceduel
    Qt5::Svg
    KF5::Completion
    KF5::Crash
    KF5::DBusAddons
    KF5::XmlGui
    KF5KDEGames
)

install(TARGETS kspaceduel  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES default_theme.svgz DESTINATION ${KDE_INSTALL_DATADIR}/kspaceduel/sprites)


########### install files ###############

install(PROGRAMS org.kde.kspaceduel.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES kspaceduel.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR})
install(FILES kspaceduelui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kspaceduel)

ecm_install_icons(ICONS
    128-apps-kspaceduel.png
    16-apps-kspaceduel.png
    22-apps-kspaceduel.png
    32-apps-kspaceduel.png
    48-apps-kspaceduel.png
    64-apps-kspaceduel.png
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
