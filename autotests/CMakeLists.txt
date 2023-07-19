if(NOT TARGET Qt${QT_MAJOR_VERSION}::QuickTest)
    message(STATUS "Qt${QT_MAJOR_VERSION}QuickTest not found, autotests will not be built.")
    return()
endif()

add_executable(qmltest qmltest.cpp)
target_link_libraries(qmltest PRIVATE Qt::Qml Qt${QT_MAJOR_VERSION}::QuickTest)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(qmltest PRIVATE STATIC_MODULE)
    target_link_libraries(qmltest PRIVATE KirigamiPlugin)
endif()

macro(kirigami_add_tests)
    if (WIN32)
        set(_extra_args -platform offscreen)
    endif()

    foreach(test ${ARGV})
        add_test(NAME ${test}
                 COMMAND qmltest
                        ${_extra_args}
                        -import ${CMAKE_BINARY_DIR}/bin
                        -input ${test}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endforeach()
endmacro()

kirigami_add_tests(
    tst_actiontoolbar.qml
    tst_avatar.qml
    tst_basiclistitem_tooltip.qml
    tst_formlayout.qml
    tst_icon.qml
    tst_keynavigation.qml
    tst_listskeynavigation.qml
    tst_mnemonicdata.qml
    tst_pagerouter.qml
    tst_pagerow.qml
    tst_routerwindow.qml
    tst_theme.qml

    pagepool/tst_layers.qml
    pagepool/tst_pagepool.qml

    wheelhandler/tst_filterMouseEvents.qml
    wheelhandler/tst_invokables.qml
    wheelhandler/tst_onWheel.qml
    wheelhandler/tst_scrolling.qml
)

set_tests_properties(tst_theme.qml PROPERTIES
    ENVIRONMENT "QT_QUICK_CONTROLS_STYLE=default;KIRIGAMI_FORCE_STYLE=1"
)
