cmake_minimum_required (VERSION 3.14.2...3.18)
project(vetinari VERSION 1.0.0 LANGUAGES CXX)
set(PROJECT_DESCRIPTION "Lord Vetinari's clock")
set(AUTHOR "Matthew Chandler <tardarsauce@gmail.com>")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(PULSEAUDIO libpulse-simple)

add_executable(vetinari vetinari.cpp)

option(TERMUX_VIBRATE "use the termux-vibrate command if your phone's speaker is broken" OFF)
find_program(TERMUX_VIBRATE_FOUND "termux-vibrate")

if(TERMUX_VIBRATE AND TERMUX_VIBRATE_FOUND)
    target_compile_definitions(vetinari PRIVATE TERMUX_VIBRATE=${TERMUX_VIBRATE_FOUND})
elseif(PULSEAUDIO_FOUND)
    target_compile_definitions(vetinari PRIVATE "-DPULSEAUDIO_FOUND")
    target_include_directories(vetinari PRIVATE ${PULSEAUDIO_INCLUDE_DIRS})
    target_link_libraries(vetinari ${PULSEAUDIO_LIBRARIES})
endif()
