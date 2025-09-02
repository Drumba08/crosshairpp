
cmake_minimum_required(VERSION 3.16)

# standarts
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# target & version from ci
if(NOT DEFINED TARGET OR TARGEt STREQUAL "")
    set(TARGET "crosshairpp")
endif()

if(NOT DEFINED VERSION OR VERSION STREQUAL "")
    set(VERSION "0.0.0")
endif()

# project
project(${TARGET} VERSION ${VERSION} LANGUAGES CXX)

# windows resource
string(REPLACE "." "," VERSION_NUMERIC "${VERSION}.0")
configure_file(${CMAKE_SOURCE_DIR}/resources/resource.rc.in ${CMAKE_BINARY_DIR}/version.rc @ONLY)

if(WIN32)
    list(APPEND SOURCES ${CMAKE_BINARY_DIR}/version.rc)
endif()

# Qt6 configuration
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Qt6 executables path for Ubuntu GitHub runners
if(EXISTS "/usr/lib/qt6/libexec/uic")
    set(CMAKE_AUTOUIC_EXECUTABLE "/usr/lib/qt6/libexec/uic")
endif()

if(EXISTS "/usr/lib/qt6/libexec/moc")
    set(CMAKE_AUTOMOC_EXECUTABLE "/usr/lib/qt6/libexec/moc")
endif()

if(EXISTS "/usr/lib/qt6/libexec/rcc")
    set(CMAKE_AUTORCC_EXECUTABLE "/usr/lib/qt6/libexec/rcc")
endif()

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/resources/ui")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${TARGET}
    WIN32

    src/main.cpp
    src/mainwindow.cpp
    src/crosshair.cpp
    src/render.cpp

    resources/ui/preset.ui
    resources/resources.qrc

    ${CMAKE_BINARY_DIR}/version.rc
)

target_compile_definitions(${TARGET} PRIVATE VERSION=${VERSION})

target_link_libraries(${TARGET} PRIVATE Qt6::Widgets)
