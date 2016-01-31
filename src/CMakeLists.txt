cmake_minimum_required (VERSION 2.6)
 
project (musicplayer)

find_package (PkgConfig REQUIRED)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${GLIBMM_INCLUDE_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

add_definitions(-std=c++11)

set (main
  channelslider.cpp
  main.cpp
  mainwindow.cpp
  pa-equalizer.cpp
)

add_executable (project ${main})
target_link_libraries (project ${GTKMM_LIBRARIES})
