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

if(NOT MSVC)
	# the code is decompiled Java: its signed arithmetic wraps, as Java's does, it type-puns, and it takes char as
	# signed: none of which the optimiser may assume otherwise (miscompiles and crashes with -Os/LTO, on arm
	# especially, where char is unsigned)
	add_compile_options(-fwrapv -fno-strict-aliasing -fsigned-char)
endif()


if (NOT ANDROID)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_mixer REQUIRED)
	find_package(ZLIB REQUIRED)
	find_package(FluidSynth REQUIRED)
endif ()

add_subdirectory("${PROJECT_SOURCE_DIR}/src")