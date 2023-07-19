find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets)

set(applicationitemapp_SRCS
    main.cpp
)

qt_add_resources(RESOURCES resources.qrc)

add_executable(applicationitemapp ${applicationitemapp_SRCS} ${RESOURCES})
target_link_libraries(applicationitemapp
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Svg
    Qt${QT_MAJOR_VERSION}::Widgets
)

install(TARGETS applicationitemapp ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

include(${CMAKE_SOURCE_DIR}/KF5Kirigami2Macros.cmake)
