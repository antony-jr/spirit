CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(spirit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(MIN_QT_VERSION "5.6.0")

find_package(Qt5Core ${MIN_QT_VERSION})
find_package(Qt5Widgets ${MIN_QT_VERSION})
find_package(Qt5Network ${MIN_QT_VERSION})

# Include Directories.
include_directories(.)
include_directories(include)
include_directories(${CMAKE_BINARY_DIR})

SET(source)
list(APPEND source include/termcolor.hpp
		   include/xdo_wrapper.hpp
		   include/windowinfo_p.hpp
		   include/windowinfo.hpp
		   include/helpers_p.hpp
		   include/spirit.hpp
		   src/spirit.cc
		   src/windowinfo_p.cc
		   src/windowinfo.cc
		   src/helpers_p.cc
		   src/main.cc
		   resources.qrc)

add_executable(spirit ${source})
target_link_libraries(spirit PUBLIC Qt5::Core Qt5::Widgets Qt5::Network xdo X11)
