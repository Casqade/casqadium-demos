cmake_minimum_required(VERSION 3.14)

set(TARGET casqadium-demos)
set(${TARGET}_VERSION 1.0.0)
project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_DEMO_AUDIO "Build demo-audio example" ON)

if(BUILD_DEMO_AUDIO)
  add_subdirectory(demo-audio)
endif()
