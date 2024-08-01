cmake_minimum_required(VERSION 3.18)

project(AssimpSceneImportExPlugin 
    VERSION 1.0
    DESCRIPTION "A improvement version of assimp scene import plugin by Qt"
    HOMEPAGE_URL "https://github.com/1995zyl/AssimpSceneImportPluginEx"
    LANGUAGES CXX
)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
link_directories(${LIBRARY_OUTPUT_PATH})

if(WIN32)
    set(SYSTEM_TYPE "win")
else()
    set(SYSTEM_TYPE "linux")
endif(WIN32)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/assimp_install/${SYSTEM_TYPE}/${CMAKE_BUILD_TYPE}")

message(STATUS "CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_INSTALL_PREFIX = " ${CMAKE_INSTALL_PREFIX})

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/assimpSceneImportEx.cmake)
find_qt_package()

add_subdirectory(3rdparty/assimp)
add_subdirectory(src)
