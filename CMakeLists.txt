cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.103.0") # handled by release scripts
set(KF_DEP_VERSION "5.103.0") # handled by release scripts

project(kirigami2 VERSION ${KF_VERSION})

set(REQUIRED_QT_VERSION 5.15.2)

################# Disallow in-source build #################

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   message(FATAL_ERROR "kirigami requires an out of source build. Please create a separate build directory and run 'cmake path_to_kirigami [options]' there.")
endif()

option(BUILD_SHARED_LIBS "Build a shared module" ON)
option(DESKTOP_ENABLED "Build and install The Desktop style" ON)
option(BUILD_EXAMPLES "Build and install examples" OFF)
option(DISABLE_DBUS "Build without D-Bus support" OFF)
option(UBUNTU_TOUCH "Build for Ubuntu Touch" OFF)
if(DEFINED STATIC_LIBRARY)
    message(FATAL_ERROR "Use the BUILD_SHARED_LIBS=OFF option to build a static library, STATIC_LIBRARY is no longer a supported option")
endif()
find_package(ECM 5.103.0 NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

if (NOT ${BUILD_SHARED_LIBS})
    # Examples are not supported when building a static library, so force them
    # to OFF.
    set(BUILD_EXAMPLES OFF)
endif()

# Make CPack available to easy generate binary packages
include(CPack)
include(FeatureSummary)
include(KDEInstallDirs)
find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Core Quick Gui Svg QuickControls2 Concurrent)
if (QT_MAJOR_VERSION EQUAL "6")
    find_package(Qt6 REQUIRED NO_MODULE COMPONENTS ShaderTools)
endif()
if (BUILD_TESTING)
    find_package(Qt${QT_MAJOR_VERSION}QuickTest ${REQUIRED_QT_VERSION} CONFIG QUIET)
endif()
get_target_property(QtGui_Enabled_Features Qt${QT_MAJOR_VERSION}::Gui QT_ENABLED_PUBLIC_FEATURES)
if(QtGui_Enabled_Features MATCHES "opengl")
    set(HAVE_QTGUI_OPENGL 1)
else()
    set(HAVE_QTGUI_OPENGL 0)
endif()
add_feature_info(QtGuiOpenGL HAVE_QTGUI_OPENGL "QtGui built with support for OpenGL")
set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS -Muri=org.kde.kirigami)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DKIRIGAMI_BUILD_TYPE_STATIC)
    add_definitions(-DQT_PLUGIN)
    add_definitions(-DQT_STATICPLUGIN=1)
endif()

################# set KDE specific information #################
# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")

include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(CMakePackageConfigHelpers)
include(ECMPoQmTools)
include(ECMFindQmlModule)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEGitCommitHooks)
include(ECMQtDeclareLoggingCategory)
include(ECMAddQch)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDEPackageAppTemplates)
include(ECMGenerateQmlTypes)
include(ECMQmlModule)
include(ECMDeprecationSettings)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Kirigami2")

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5Kirigami2Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5Kirigami2Config.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS  CMAKE_INSTALL_PREFIX
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5Kirigami2Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5Kirigami2ConfigVersion.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5Kirigami2Macros.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KF5Kirigami2Targets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KF5Kirigami2Targets.cmake
    NAMESPACE KF5::
)


ecm_setup_version(${KF_VERSION}
    VARIABLE_PREFIX KIRIGAMI2
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kirigami_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5Kirigami2ConfigVersion.cmake"
    SOVERSION 5
)

#use dbus on linux, bsd etc, but not android and apple stuff
if (UNIX AND NOT ANDROID AND NOT(APPLE) AND NOT(DISABLE_DBUS))
    find_package(Qt${QT_MAJOR_VERSION}DBus)
    add_definitions(-DKIRIGAMI_ENABLE_DBUS)
endif()

find_package(OpenMP)
set_package_properties(OpenMP
    PROPERTIES DESCRIPTION "Multi-platform shared-memory parallel programming in C/C++ and Fortran"
    TYPE OPTIONAL
    PURPOSE "Accelerates palette generation in Kirigami.ImageColors"
)
if(OpenMP_CXX_FOUND)
    set(HAVE_OpenMP TRUE)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
configure_file(config-OpenMP.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-OpenMP.h)

if (UBUNTU_TOUCH)
    add_definitions(-DUBUNTU_TOUCH)
endif()

ecm_find_qmlmodule(QtGraphicalEffects 1.0)

ecm_set_disabled_deprecation_versions(
    QT 5.15.2
    KF 5.95
)

add_subdirectory(src)
if (NOT ANDROID)
    add_subdirectory(templates)
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

ecm_install_po_files_as_qm(poqm)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
