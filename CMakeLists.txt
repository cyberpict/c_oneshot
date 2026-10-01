cmake_minimum_required(VERSION 3.20)
project(madlibs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets PrintSupport Test)

add_executable(madlibs
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/stories.cpp
    src/stories.h
)
target_link_libraries(madlibs PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
)

# ---- Headless unit tests (no display required; run under Qt's "offscreen" platform) ----
# The stories module is pure QtCore/C++, so the test target doesn't need the GUI stack.
enable_testing()
add_executable(test_stories tests/test_stories.cpp src/stories.cpp src/stories.h)
target_include_directories(test_stories PRIVATE src)
target_link_libraries(test_stories PRIVATE Qt6::Core Qt6::Test)
add_test(NAME test_stories COMMAND test_stories)
set_tests_properties(test_stories PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

# ---- Install rules ----
if(UNIX AND NOT APPLE)
    install(TARGETS madlibs RUNTIME DESTINATION bin)
    install(FILES madlibs.desktop DESTINATION share/applications)
endif()
