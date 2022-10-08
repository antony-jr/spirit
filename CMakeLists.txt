CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(spirit)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CONAN_BUILD ON)
set(MIN_QT_VERSION "5.6.0")

if (APPLE)
   enable_language(OBJC)
   set(CMAKE_EXE_LINKER_FLAGS 
       "-framework Cocoa -framework AppKit -framework CoreData -framework Foundation")
endif()

if (UNIX)
   set(CONAN_BUILD OFF)
endif()

if (CONAN_BUILD)
   list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
   list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
   include(conan.cmake)
   
   conan_cmake_autodetect(settings)
   conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}/conanfile.py
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})
endif()
find_package(Qt5 COMPONENTS Core Widgets Network Multimedia)

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

if (APPLE)
   list(APPEND source include/macos_p.h
      		      src/macos_p.m)
endif()

if (WIN32)
   add_executable(spirit WIN32 ${source})
else if(APPLE)
   add_executable(spirit MACOSX_BUNDLE ${source})
else()
   add_executable(spirit ${source})
endif()

target_link_libraries(spirit PUBLIC Qt5::Core Qt5::Widgets Qt5::Network Qt5::Multimedia)
if (UNIX AND NOT APPLE) 
 target_link_libraries(spirit PUBLIC KF5::WindowSystem)
endif()
if (UNIX)
   target_link_libraries(spirit PUBLIC QArchive)
else()
   target_link_libraries(spirit PUBLIC QArchive::QArchive)
endif()
target_link_libraries(spirit PUBLIC civetweb::civetweb civetweb::civetweb-cpp)

if (WIN32 AND CONAN_BUILD)
	include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
	add_custom_command(
        TARGET spirit POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CONAN_BIN_DIRS_QARCHIVE}/QArchive.dll
                ${CMAKE_CURRENT_BINARY_DIR}/Release/QArchive.dll)
	add_custom_command(
        TARGET spirit POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CONAN_BIN_DIRS_CIVETWEB}/civetweb.dll
                ${CMAKE_CURRENT_BINARY_DIR}/Release/civetweb.dll)
	add_custom_command(
		TARGET spirit POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CONAN_BIN_DIRS_CIVETWEB}/civetweb-cpp.dll
		${CMAKE_CURRENT_BINARY_DIR}/Release/civetweb-cpp.dll)
	
	add_custom_command(
		TARGET spirit POST_BUILD
		COMMAND ${CONAN_BIN_DIRS_QT}/windeployqt.exe
		${CMAKE_CURRENT_BINARY_DIR}/Release/spirit.exe)
endif()

# Hotfix for CI/CD
if (UNIX AND NOT APPLE) 
   target_link_libraries(spirit PUBLIC pthread dl util)
endif ()
