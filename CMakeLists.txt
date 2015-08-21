project(kspaceduel)
cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.3.0")

find_package(ECM 1.7.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Qml Quick QuickWidgets Svg Test)
find_package(KF5 REQUIRED COMPONENTS 
    CoreAddons
    Config
    ItemModels
    WidgetsAddons
    WindowSystem
    Config
    DBusAddons
    I18n
    ConfigWidgets
    TextWidgets
    XmlGui
    KIO
    NotifyConfig
    NewStuff
    KDELibs4Support
    DocTools
)

find_package(KF5KDEGames 4.9.0 REQUIRED)
find_package(Phonon4Qt5 CONFIG REQUIRED)

include_directories(BEFORE ${PHONON_INCLUDES})

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DTRANSLATION_DOMAIN="kspaceduel")



add_subdirectory( sprites ) 
add_subdirectory( pics ) 
add_subdirectory( doc )

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

ki18n_wrap_ui(kspaceduel_SRCS general.ui )

kconfig_add_kcfg_files(kspaceduel_SRCS options.kcfgc )

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*-apps-kspaceduel.png")
ecm_add_app_icon(kspaceduel_SRCS ICONS ${ICONS_SRCS})
add_executable(kspaceduel ${kspaceduel_SRCS})

target_link_libraries(kspaceduel   KF5KDEGames KF5::KDELibs4Support Qt5::Svg)

install(TARGETS kspaceduel  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )
install(FILES default_theme.svgz DESTINATION ${KDE_INSTALL_DATADIR}/kspaceduel/sprites )


########### install files ###############

install( PROGRAMS org.kde.kspaceduel.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES kspaceduel.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )
install( FILES kspaceduelui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kspaceduel )

ecm_install_icons( ICONS  128-apps-kspaceduel.png  16-apps-kspaceduel.png  22-apps-kspaceduel.png  32-apps-kspaceduel.png  48-apps-kspaceduel.png  64-apps-kspaceduel.png DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor   )



feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

