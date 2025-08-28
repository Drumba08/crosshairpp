cmake_minimum_required(VERSION 3.16)
project(crosshair++ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# debug flags for GCC/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
endif()

# debug flags for MSVC
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od /MDd")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Fix Qt6 executables path for Ubuntu/Debian GitHub runners
if(EXISTS "/usr/lib/qt6/libexec/uic")
    set(CMAKE_AUTOUIC_EXECUTABLE "/usr/lib/qt6/libexec/uic")
endif()

if(EXISTS "/usr/lib/qt6/libexec/moc")
    set(CMAKE_AUTOMOC_EXECUTABLE "/usr/lib/qt6/libexec/moc")
endif()

if(EXISTS "/usr/lib/qt6/libexec/rcc")
    set(CMAKE_AUTORCC_EXECUTABLE "/usr/lib/qt6/libexec/rcc")
endif()

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/resources")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(crosshair++
    WIN32

    src/main.cpp
    src/mainwindow.cpp
    src/crosshair.cpp
    src/render.cpp

    resources/preset.ui
)

if(WIN32)
    set_target_properties(crosshair++ PROPERTIES
        OUTPUT_NAME "crosshair++"
        RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resources/win.rc"
    )
endif()

target_link_libraries(crosshair++ PRIVATE Qt6::Widgets)
