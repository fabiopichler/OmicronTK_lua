cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME OmicronTK_lua)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib)

option(USE_CUSTOM_LUA "Use a custom Lua build" OFF)
option(USE_LUAJIT "Use LuaJIT" ON)

if(USE_CUSTOM_LUA)
    set(LUA_INCLUDE_DIR "" CACHE STRING "Lua include directory")
    set(LUA_LIBRARIES "" CACHE STRING "Lua library")
else()
    if(USE_LUAJIT)
        find_package(LuaJIT 2.1 REQUIRED)
    else()
        find_package(Lua 2.1 REQUIRED)
    endif()
endif()

include_directories(include ${LUA_INCLUDE_DIR})

add_definitions(-DOTKLUA_BUILD_CORE_LIB)

set(SRC_FILES
    include/OmicronTK/lua/global.h
    include/OmicronTK/lua/LuaBase.hpp
    include/OmicronTK/lua/LuaState.hpp
    include/OmicronTK/lua/version.h
    src/LuaBase.cpp
    src/LuaState.cpp)

if(WIN32)
    set(SRC_FILES ${SRC_FILES} rc/lua.rc)
endif()

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
endif()