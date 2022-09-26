CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(spirit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(MIN_QT_VERSION "5.6.0")

find_package(Qt5Core ${MIN_QT_VERSION})
find_package(Qt5Widgets ${MIN_QT_VERSION})
find_package(Qt5Network ${MIN_QT_VERSION})
find_package(Qt5Multimedia ${MIN_QT_VERSION})

# KWindowSystem
if (UNIX AND NOT APPLE)  
   find_package(KF5WindowSystem)
endif()

# QArchive
find_package(QArchive)

# CivetWeb
find_package(civetweb)

# Include Directories.
include_directories(.)
include_directories(include)
include_directories(${CMAKE_BINARY_DIR})

if(ENABLE_DEBUGGING)
   add_definitions(-DENABLE_DEBUGGING)
endif()

if(SPIRIT_VERSION)
   add_compile_definitions(SPIRIT_VERSION="${SPIRIT_VERSION}")
else()
   add_compile_definitions(SPIRIT_VERSION="v1")
endif()

if(SPIRIT_COMMIT) 
   add_compile_definitions(SPIRIT_COMMIT="${SPIRIT_COMMIT}")
else()
   add_compile_definitions(SPIRIT_COMMIT="none")
endif()

if(SPIRIT_BUILD)
   add_compile_definitions(SPIRIT_BUILD="${SPIRIT_BUILD}")
else()
   add_compile_definitions(SPIRIT_BUILD="1")
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
		   include/windowquirks.hpp
		   include/spiritsettings.hpp
		   include/spiritconfig.hpp
		   include/spiritdaemon_p.hpp
		   include/spiritdaemon.hpp
		   include/spiritapp.hpp
		   include/spiritmanager.hpp
		   include/guiapp.hpp
		   src/guiapp.cc
		   src/spiritmanager.cc
		   src/spiritapp.cc
		   src/activewindowtracker_p.cc
		   src/activewindowtracker.cc
		   src/spiritworker_p.cc
		   src/spiritworker.cc
		   src/spirit.cc
		   src/windowquirks.cc
		   src/spiritsettings.cc
		   src/spiritconfig.cc
		   src/spiritdaemon_p.cc
		   src/spiritdaemon.cc
		   src/helpers_p.cc
		   src/main.cc

		   # Resources
		   resources.qrc)

add_executable(spirit ${source})
target_link_libraries(spirit PUBLIC Qt5::Core Qt5::Widgets Qt5::Network Qt5::Multimedia)
if (UNIX AND NOT APPLE) 
 target_link_libraries(spirit PUBLIC KF5::WindowSystem)
endif()
target_link_libraries(spirit PUBLIC QArchive)
target_link_libraries(spirit PUBLIC civetweb civetweb-cpp)

# Hotfix for CI/CD
if (UNIX AND NOT APPLE) 
   target_link_libraries(spirit PUBLIC pthread dl util)
endif ()
