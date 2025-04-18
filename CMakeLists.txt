cmake_minimum_required(VERSION 3.16)

project(ubntai VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

# Core Qt components
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    Network
    Sql
    Multimedia
    Concurrent
)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ubntai
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resource.qrc
        networking.h networking.cpp
        clickablelabel.h clickablelabel.cpp
        user.h user.cpp

        asistant.h asistant.cpp


        welcome.h welcome.cpp

        urls.h urls.cpp
        db.h db.cpp
        pastqueries.h pastqueries.cpp
        loading.h loading.cpp
        snapcraft.yaml

        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        README.md
        troubleshoot.h troubleshoot.cpp
        installation.h installation.cpp
        flowlayout.h flowlayout.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ubntai APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ubntai SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ubntai
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link all required Qt modules to your target
target_link_libraries(ubntai PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Concurrent
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Ubntai)
endif()
set_target_properties(ubntai PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ubntai DESTINATION bin)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ubntai)
endif()
