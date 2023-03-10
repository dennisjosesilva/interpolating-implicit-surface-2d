project(interpolating_implicit_function_2d)

# CMAKE MINIMUM VERSION
cmake_minimum_required(VERSION 3.10)

# INCLUDE DIRECTORIES
include_directories(include)

# CXX STANDARD
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QT CONFIG
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# OPENGL
find_package(OpenGL REQUIRED)

# CONAN CONFIG
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# SOURCES
file(GLOB_RECURSE PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# INCLUDE QT MACROS TO HEADER FILES
qt5_wrap_cpp(PROJECT_SOURCE
  include/App/MainWidget.hpp
  include/App/PropertiesPanel.hpp
  include/App/GraphicsViewWidget.hpp
  include/App/MainWindow.hpp)

# SET RESOURCE FILES
qt5_add_resources(PROJECT_SOURCE resources.qrc)

# EXECUTABLES
add_executable(interpolating_implicit_function_2d ${PROJECT_SOURCE})
target_link_libraries(interpolating_implicit_function_2d PUBLIC
  ${CONAN_LIBS}
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
  OpenGL::OpenGL)