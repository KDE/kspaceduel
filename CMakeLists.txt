cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# Release Service version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "25")
set (RELEASE_SERVICE_VERSION_MINOR "12")
set (RELEASE_SERVICE_VERSION_MICRO "0")
set (RELEASE_SERVICE_COMPACT_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}${RELEASE_SERVICE_VERSION_MINOR}${RELEASE_SERVICE_VERSION_MICRO}")

# Bump KSPACEDUEL_BASE_VERSION once new features are added
set(KSPACEDUEL_BASE_VERSION "2.1")
set(KSPACEDUEL_VERSION "${KSPACEDUEL_BASE_VERSION}.${RELEASE_SERVICE_COMPACT_VERSION}")

project(kspaceduel VERSION ${KSPACEDUEL_VERSION})

set(QT_MIN_VERSION "6.5.0")
set(KF_MIN_VERSION "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(FeatureSummary)
include(ECMDeprecationSettings)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Svg
    SvgWidgets
    Widgets
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    Crash
    DBusAddons
    DocTools
    I18n
    XmlGui
)

find_package(KDEGames6 6.0.0 REQUIRED)

ecm_set_disabled_deprecation_versions(
    QT 6.9.0
    KF 6.19.0
    KDEGAMES 6.0
)

add_subdirectory(doc)
add_subdirectory(sprites)
add_subdirectory(pics)
add_subdirectory(icons)
add_subdirectory(src)

ki18n_install(po)
kdoctools_install(po)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
