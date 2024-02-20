cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(DoomRPG_vita LANGUAGES C)
include("${VITASDK}/share/vita.cmake" REQUIRED)

find_package(SDL2 REQUIRED)
find_package(ZLIB REQUIRED)

set(VITA_APP_NAME "Doom RPG")
set(VITA_TITLEID "DRPG00001")
set(VITA_VERSION "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -fno-short-enums")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE HEADER_FILES "src/*.h")
file(GLOB_RECURSE SOURCE_FILES "src/*.c")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 ZLIB::ZLIB)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
