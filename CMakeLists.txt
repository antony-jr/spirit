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

# KWindowSystem
find_package(KF5WindowSystem)

# QArchive
find_package(QArchive)

# CivetWeb
find_package(civetweb)

#set(CIVETWEB_ENABLE_CXX ON)
#set(CIVETWEB_SERVE_NO_FILES ON)
#set(CIVETWEB_DISABLE_CGI ON)
#set(CIVETWEB_ENABLE_SERVER_EXECUTABLE OFF)
#add_subdirectory(libs/civetweb)
#include_directories(libs/civetweb/include)

# Include Directories.
include_directories(.)
include_directories(include)
include_directories(${CMAKE_BINARY_DIR})

if(ENABLE_DEBUGGING)
   add_definitions(-DENABLE_DEBUGGING)
endif()

SET(source)
list(APPEND source include/termcolor.hpp
		   include/helpers_p.hpp
		   include/spiritenums.hpp
		   include/activewindowtracker_p.hpp
		   include/activewindowtracker.hpp
		   include/spiritworker_p.hpp
		   include/spiritworker.hpp
		   include/spirit.hpp
		   include/spiritdaemon_p.hpp
		   include/spiritdaemon.hpp
		   src/activewindowtracker_p.cc
		   src/activewindowtracker.cc
		   src/spiritworker_p.cc
		   src/spiritworker.cc
		   src/spirit.cc
		   src/spiritdaemon_p.cc
		   src/spiritdaemon.cc
		   src/helpers_p.cc
		   src/main.cc

		   # Resources
		   resources.qrc)

add_executable(spirit ${source})
target_link_libraries(spirit PUBLIC Qt5::Core Qt5::Widgets Qt5::Network)
target_link_libraries(spirit PUBLIC KF5::WindowSystem)
target_link_libraries(spirit PUBLIC QArchive)
target_link_libraries(spirit PUBLIC civetweb civetweb-cpp)
