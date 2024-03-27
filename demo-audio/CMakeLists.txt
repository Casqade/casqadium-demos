cmake_minimum_required(VERSION 3.15)

set(TARGET demo-audio)
set(${TARGET}_VERSION 1.0.0)
project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

include(GNUInstallDirs)
set(${TARGET}_BINARY_PATH "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${TARGET}")

add_executable(${TARGET})
target_sources(${TARGET} PRIVATE
  main.cpp
  icon.rc

  src/callbacks.cpp
  src/components.cpp
  src/systems.cpp
)

set_target_properties(${TARGET} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${${TARGET}_BINARY_PATH}
)


if(CMAKE_BUILD_TYPE MATCHES "Release")

  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_options(${TARGET} PUBLIC "/ENTRY:mainCRTStartup")
    set_target_properties(${TARGET} PROPERTIES
      CXX_STANDARD_REQUIRED ON
      CXX_STANDARD 20
    )
  else()
    target_compile_options(${TARGET} PUBLIC -O2)
  endif()

  set_target_properties(${TARGET} PROPERTIES
    WIN32_EXECUTABLE TRUE
  )

else()

  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_target_properties(${TARGET} PROPERTIES
      CXX_STANDARD_REQUIRED ON
      CXX_STANDARD 20
    )
  else()
    target_compile_options(${TARGET} PUBLIC -Og)
  endif()

  set_target_properties(${TARGET} PROPERTIES
    WIN32_EXECUTABLE FALSE
  )
endif()

if(NOT ${BUILD_SHARED_LIBS})
  target_link_libraries(${TARGET} PUBLIC -static)
endif()


include(FetchContent)

FetchContent_Declare(casqadium
  GIT_REPOSITORY https://github.com/Casqade/casqadium
  GIT_TAG        main
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(casqadium)


target_link_libraries(${TARGET} PRIVATE
  casqadium::casqadium
)

target_include_directories(${TARGET} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
)

file(MAKE_DIRECTORY "${${TARGET}_BINARY_PATH}")
file(CREATE_LINK "${CMAKE_CURRENT_LIST_DIR}/data" "${${TARGET}_BINARY_PATH}/data" SYMBOLIC)
