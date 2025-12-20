cmake_minimum_required(VERSION 3.25)
project(DoomRPG LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (NOT ANDROID)
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
else()
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
endif()

if(MSVC)
	add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()


if (NOT ANDROID)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_mixer REQUIRED)
	find_package(ZLIB REQUIRED)
	find_package(FluidSynth REQUIRED)
endif ()

add_subdirectory("${PROJECT_SOURCE_DIR}/src")