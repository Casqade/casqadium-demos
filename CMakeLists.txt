cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW) 
endif()

set(TARGET casqadium-demos)
set(${TARGET}_VERSION 1.0.0)
project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_DEMO_AUDIO "Build demo-audio example" ON)

if(${BUILD_SHARED_LIBS})
  set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE INTERNAL "" FORCE)
else()
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE INTERNAL "" FORCE)
endif()

if(BUILD_DEMO_AUDIO)
  add_subdirectory(demo-audio)
endif()
